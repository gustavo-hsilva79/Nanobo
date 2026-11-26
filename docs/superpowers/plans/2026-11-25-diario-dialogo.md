# Diário do Nanobô em caixa de diálogo — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Uma caixa de diálogo paginada única para as anotações do Diário, a abertura e os interlúdios do Léo, mais as trocas de conteúdo decididas (espécies e "Ataque T").

**Architecture:** A máquina de páginas e revelação é lógica pura em `gameplay.c` (testável no harness, sem Allegro); o `main.c` só desenha e repassa teclas. As anotações e os interlúdios continuam na fila de cards do `gameplay.c` (que pausa o jogo); a abertura usa a mesma caixa com um estado próprio do `main.c` na cena `BRIEFING`.

**Tech Stack:** C11, Allegro 5.2 (`main.c` apenas), MSVC v143, harness `../Nanobo-testes/harness_combate.c`.

**Spec:** `docs/superpowers/specs/2026-11-25-diario-dialogo-design.md`

## Global Constraints

- Só compilam `src/main.c`, `src/core/game.c`, `src/entities/entities.c`, `src/gameplay/gameplay.c`; nenhum `.c` ou pasta nova; `gameplay.c` não inclui Allegro.
- C11, nível de aviso 4: **0 avisos** em Debug e Release x64.
- Arquivos CRLF e UTF-8; editar por âncora; conferir CR = LF depois de cada arquivo.
- Tela lógica 640×360, fonte embutida de 8 px por caractere; sem maiúscula acentuada; hífen ASCII.
- Texto a **~40 caracteres/s**; **trava de 0,2 s** no início de cada página; **até 3 linhas por página, 1 a 4 páginas por entrada**.
- Enter, Espaço ou clique esquerdo: completa a página → próxima → fecha na última. Esc fecha a entrada (no jogo) ou pula a abertura inteira (na `BRIEFING`).
- Rodapé: contador `n/N`; triângulo piscando (primitiva, não glifo) com a página completa; `fechar` na última página.
- Retrato = spritesheet do Nanobô, linha 1 (frente), quadro 0; área de texto com a mesma largura em todas as entradas.
- Um termo-chave por página, em âmbar, marcado no texto entre chaves `{termo}`; as chaves não contam na largura nem aparecem na tela.
- Todo texto desenhado registrado em `docs/game-design/textos-do-jogo.md` com ID e contagem; `python ../Nanobo-testes/conferir_textos.py .` com 0 divergências.
- Commits: Conventional Commits, assunto em português, uma linha só, sem corpo nem `Co-Authored-By`.
- Harness: `bash ../Nanobo-testes/build_harness.sh`, depois no PowerShell `$d = Join-Path $env:TEMP "nanobo-harness"; cmd /c "`"$d\build.bat`""; & "$d\harness.exe" | Select-Object -Last 3`. Antes deste plano: **482 passaram, 0 falharam**. O harness fica fora do git: manter CRLF.

## Review Focus

1. **Toque duplo na virada de página**: um Enter que completa a página seguido de outro em menos de 0,2 s não pode virar duas páginas — teste na Task 1.
2. **Clique com a caixa aberta**: o clique esquerdo avança a caixa e **não dispara** a arma nem escaneia — verificação na Task 2.
3. **Duas entradas na fila ao mesmo tempo** (interlúdio da Fase 3 e CC-05 entram juntos): fechar a primeira abre a segunda já do início (página 1, revelação zerada) — teste na Task 1 e verificação na Task 2.
4. **Reinício pelo Game Over**: a abertura e os interlúdios aparecem de novo na partida seguinte — teste na Task 1 (interlúdios) e verificação na Task 3 (abertura).
5. **Perda de foco da janela com a caixa aberta** (pausa automática): ao voltar, a caixa continua na mesma página e o jogo continua pausado por ela — verificação na Task 2.

---

### Task 1: Máquina da caixa e interlúdios na fila

**Files:**
- Modify: `src/gameplay/gameplay.h` (enum `ConceptCard`, novos protótipos), `src/gameplay/gameplay.c` (fila de cards, `start_phase`)
- Test: `../Nanobo-testes/harness_combate.c`

**Interfaces:**
- Produces:
  - `ConceptCard` ganha `CARD_INTERLUDE_2 = 0` e `CARD_INTERLUDE_3 = 1` como **os dois primeiros valores**; os outros sobem dois números, na mesma ordem (`CARD_GRAM` … `CARD_FUNGUS`), e `CARD_COUNT` cresce 2.
  - `typedef struct { int pages; int page; float shown; float page_age; int page_chars[4]; bool open; } DialogState;` em `gameplay.h` — `page_chars[p]` = caracteres visíveis da página `p` (sem as chaves de marcação), preenchido por quem abre.
  - `void dialog_open(DialogState* d, int pages, const int page_chars[4]);` — página 0, `shown = 0`, `page_age = 0`, `open = true`.
  - `void dialog_tick(DialogState* d, float dt);` — `shown += DIALOG_CHARS_PER_SECOND * dt` até `page_chars[page]`; `page_age += dt`.
  - `bool dialog_page_complete(const DialogState* d);` e `bool dialog_last_page(const DialogState* d);`
  - `void dialog_press(DialogState* d);` — se `page_age < DIALOG_DEBOUNCE` não faz nada; senão, página incompleta → completa; completa e não última → próxima página (`shown = 0`, `page_age = 0`); completa e última → `open = false`.
  - Constantes em `gameplay.h`: `DIALOG_CHARS_PER_SECOND 40.0f`, `DIALOG_DEBOUNCE 0.2f`, `DIALOG_MAX_PAGES 4`.
- Consumes: `queue_card`, `pending_card`, `dismiss_card`, `start_phase` existentes.

- [ ] **Step 1: Escrever os testes que falham** — bloco novo no harness:
  - `check("caixa abre na pagina 1 sem texto", d.page == 0 && d.shown == 0.0f && d.open)`
  - `check("40 caracteres em 1 s", near(d.shown, 40.0f))` após `dialog_tick(&d, 1.0f)` numa página de 100 caracteres
  - `check("toque na trava de 0,2 s nao faz nada", ...)` — `dialog_tick(&d, 0.1f); dialog_press(&d);` mantém `page == 0` e página incompleta
  - `check("primeiro toque completa a pagina", dialog_page_complete(&d) && d.page == 0)`
  - `check("toque duplo rapido nao pula duas paginas", d.page == 1)` — completar, avançar, e um terceiro `dialog_press` com `page_age` < 0,2 s deixa `page == 1`
  - `check("ultimo toque na ultima pagina fecha", !d.open)`
  - `check("interludio 2 entra no inicio da Fase 2", pending_card() == CARD_INTERLUDE_2)` após `reset_match(); start_next_phase();`
  - `check("interludio 3 sai antes do CC-05", pending_card() == CARD_INTERLUDE_3)` e, após `dismiss_card()`, `pending_card() == CARD_WEAKENED`
  - `check("interludio volta depois do reset", ...)` — repetir a passagem após `reset_match()`
  - `check("jogo pausado com interludio pendente", ...)` — posição de um agente não muda após `run(1.0)` com a entrada pendente
- [ ] **Step 2: Rodar o harness e ver os checks novos falharem** (não compila ou FAIL nos novos).
- [ ] **Step 3: Implementar** o enum, `DialogState` e as funções em `gameplay.c`; em `start_phase`, `queue_card(CARD_INTERLUDE_2)` ao entrar na Fase 2 e `queue_card(CARD_INTERLUDE_3)` ao entrar na Fase 3 (antes do `CARD_WEAKENED`). Ajuste os testes antigos que dependiam do primeiro card pendente nas Fases 2 e 3 chamando `dismiss_card()` para o interlúdio.
- [ ] **Step 4: Rodar o harness** — esperado: todos passam (482 + novos), 0 falharam; build Debug com 0 avisos (o `main.c` ainda compila: os arrays `card_titles`/`card_lines` ganham 2 entradas provisórias com o texto do interlúdio do design).
- [ ] **Step 5: Commit** — `feat(gameplay): adiciona a caixa de diálogo e os interlúdios na fila`

### Task 2: Caixa desenhada nas anotações e nos interlúdios

**Files:**
- Modify: `src/main.c` (tabela de entradas, `draw_concept_card` → caixa, entrada de teclado e mouse com card pendente, laço do timer)
- Modify: `docs/game-design/textos-do-jogo.md`; `../Nanobo-testes/conferir_textos.py` (ignorar `{` `}`)

**Interfaces:**
- Consumes: `DialogState`, `dialog_open/tick/press/page_complete/last_page`, `CARD_INTERLUDE_2/3` (Task 1); `pending_card()`, `dismiss_card()`.
- Produces:
  - `typedef struct { const char* header; int pages; const char* lines[DIALOG_MAX_PAGES][3]; bool portrait; } DialogEntry;` em `main.c`.
  - `static const DialogEntry diary_entries[CARD_COUNT];` — anotações (header `Nanobô · Anotação N`) e interlúdios (header `Enquanto isso, no hospital`, `portrait = false`).
  - `void draw_dialog(const DialogEntry* e, const DialogState* d);` — reutilizada pela Task 3.
  - `int dialog_visible_chars(const char* line);` — conta sem as chaves.

- [ ] **Step 1: Montar `diary_entries`** com os textos atuais de cada card como página 1 (título vira parte do header ou primeira linha) e os dois interlúdios com o texto do design (Fase 2: a infecção no sangue cedeu, mas Léo pegou um vírus respiratório na enfermaria; Fase 3: com a defesa fraca, esporos de bolor que estão no ar chegam ao pulmão), marcando um `{termo}` por página. Textos finais virão da Task 5.
- [ ] **Step 2: Implementar `draw_dialog`** conforme Global Constraints (caixa de x = 16 a 624 na faixa de baixo, véu escuro, retrato 48×48, header, até 3 linhas, termo em `UI_AMBER`, contador `n/N`, triângulo piscando a 0,5 s só com a página completa, `fechar` na última).
- [ ] **Step 3: Ligar o estado**: quando `pending_card()` muda para uma entrada nova, `dialog_open` com os `page_chars` daquela entrada; `dialog_tick` a cada tique do timer enquanto houver card pendente (mesmo com `update_match` parado); Enter/Espaço/clique esquerdo → `dialog_press`, e se `!d.open` → `dismiss_card()`; Esc → `dismiss_card()` direto. O clique com card pendente **retorna antes** do código de tiro e de scan. A perda de foco não fecha nem reabre a caixa.
- [ ] **Step 4: Ensinar o `conferir_textos.py` a remover `{` e `}`** antes de comparar e contar; registrar os textos e os IDs novos (interlúdios) em `textos-do-jogo.md`.
- [ ] **Step 5: Verificar** — build Debug/Release 0 avisos; harness verde; capturas com `../Nanobo-testes/qa_jogo.ps1` (Debug): uma anotação no meio da revelação, completa com triângulo, última página com `fechar`; interlúdio da Fase 2 (F na Fase 1, ENTER) e da Fase 3 seguido do CC-05; um clique com a caixa aberta não gera tiro (contador de munição/cooldown intacto na captura seguinte).
- [ ] **Step 6: Commit** — `feat(ui): mostra o diário e os interlúdios na caixa de diálogo`

### Task 3: Abertura na caixa (ficha do hospital + Anotação 0)

**Files:**
- Modify: `src/main.c` (cena `BRIEFING`: tabelas `briefing_titles/briefing_lines`, `draw_briefing`, entrada da cena), `docs/game-design/textos-do-jogo.md` (NR-01..14 substituídos)

**Interfaces:**
- Consumes: `DialogEntry`, `draw_dialog` (Task 2); `DialogState` e funções (Task 1).
- Produces: `static const DialogEntry opening_entries[2];` e o estado da abertura no `main.c`, zerado ao entrar em `BRIEFING`.

- [ ] **Step 1: Escrever as 2 entradas**: ficha (`Hospital · ficha do paciente`, sem retrato: Léo Martins Ferreira, 16 anos, febre alta, infecção no sangue) e Anotação 0 (`Nanobô · Anotação 0`: sistema ligado, memória vazia, paciente Léo, vai descobrir o que está atacando ele) — rascunho; texto final na Task 5.
- [ ] **Step 2: Trocar os 4 cartões e os botões SEGUIR/PULAR** pela caixa: Enter/Espaço/clique avançam; o fim da última entrada entra na Fase 1 (`GAME_SCENE_GAME`); Esc pula para a Fase 1; o rodapé `enter ou clique avança · esc pula` (BR-04) continua. Ao entrar em `BRIEFING` (menu ou Game Over), o estado volta à primeira entrada.
- [ ] **Step 3: Verificar** — build 0 avisos; harness verde; capturas: ficha, Anotação 0 no fim, Esc pulando, e Game Over → nova partida mostrando a ficha de novo.
- [ ] **Step 4: Commit** — `feat(ui): conta a abertura na caixa de diálogo`

### Task 4: Espécies novas e "Ataque T"

**Files:**
- Modify: `src/entities/entities.c` (`phase_one_agents`), `src/main.c` e `src/gameplay/gameplay.c` (textos com "Lise"), `docs/game-design/textos-do-jogo.md`, `docs/game-design/fase2-virus.md`
- Test: `../Nanobo-testes/harness_combate.c` (checks que citem textos da Lise)

**Interfaces:**
- Consumes: `diary_entries` (Task 2) para as páginas novas.
- Produces: nada que outras tarefas usem.

- [ ] **Step 1: Trocar as espécies** em `phase_one_agents`: `"Neisseria meningitidis"` → `"Moraxella catarrhalis"` (continua `SPECIES_GRAM_NEGATIVE`, `SHAPE_COCCUS`); `"Bacillus cereus"` → `"Clostridium tetani"` (continua `SPECIES_GRAM_POSITIVE`, `SHAPE_BACILLUS`); atualizar o comentário e a linha RT-05c do registro.
- [ ] **Step 2: Renomear "Lise" para "Ataque T"** em todo texto desenhado (feedback, HUD `Ataque T · célula infectada`, relatório) e nos checks do harness que comparam esses textos; medir a linha 0 do HUD (27 caracteres, a mesma largura de `Neutralização · vírus livre`). Comentários e nomes de código (`LASER_LYSE`) ficam.
- [ ] **Step 3: Páginas novas** em `diary_entries`: na anotação de Gram, uma página dizendo que no jogo todas as bactérias aparecem juntas no sangue para simplificar e que as armas são regra do jogo inspirada em antibióticos reais; na anotação do ciclo, uma página dizendo que lisogênico e lítico foram descritos em vírus de bactérias e servem de comparação. Registrar.
- [ ] **Step 4: Verificar** — harness verde (ajustados os checks de texto), `conferir_textos.py` 0/0, build 0 avisos, captura da ficha do scanner com `Moraxella catarrhalis`.
- [ ] **Step 5: Commit** — `feat(gameplay): troca espécies e renomeia a lise para ataque t`

### Task 5: Textos aprovados pelo dono

**Files:**
- Modify: `src/main.c` (`diary_entries`, `opening_entries`), `docs/game-design/textos-do-jogo.md`

**Interfaces:**
- Consumes: o inventário "atual × proposto" aprovado pelo dono (artifact separado; a parte de cards, abertura e interlúdios).

- [x] **Step 1: Copiar os textos aprovados** para as tabelas, uma página por linha aprovada, respeitando 3 linhas × largura medida e um `{termo}` por página.
- [x] **Step 1b: Painel de fim de fase na voz do Diário** — em `draw_results`, o título do painel passa de `o que a partida mostrou` para `O que anotei nesta fase` e as linhas de `results_report_line` usam o texto aprovado (mesmos números, voz do Nanobô), medindo a largura do painel.
- [x] **Step 2: Verificar** — `conferir_textos.py` 0/0; build 0 avisos; harness verde; capturas de todas as entradas revisadas pelo dono.
- [x] **Step 3: Commit** — `feat(ui): aplica os textos do diário aprovados`
