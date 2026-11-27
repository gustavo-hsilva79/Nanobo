# Redesenho da interface (#245) + arte (#246) — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Levar ao jogo o HUD com coluna de vida, a mira em sprite, o aviso de objetivo, a ficha do scanner nova, a cena "como jogar", e menu, Pausa, Resultados, abertura, Vitória e Game Over com os fundos e textos aprovados.

**Architecture:** As regras novas que dá para testar (segmentos, pulso, quadro da mira, meta e aviso de objetivo, transições de cena) ficam em `gameplay.c` e `game.c`, sem Allegro, cobertas pelo harness; o `main.c` só carrega bitmaps, desenha e repassa teclas. Duas ondas: a 1ª é o que aparece durante a fase (Tasks 1–5), a 2ª as telas fora dela (Tasks 6–10); cada onda fecha com capturas para o dono.

**Tech Stack:** C11, Allegro 5.2 (só no `main.c`), MSVC v143, harness `../Nanobo-testes/harness_combate.c`.

**Spec:** `docs/superpowers/specs/2026-11-28-redesenho-interface-design.md`. **Textos:** `.hermes/inventario-interface.json` (local, aprovado em 26/11; cada task cita os IDs que aplica — o campo `proposto` é o texto final).

## Global Constraints

- Só compilam `src/main.c`, `src/core/game.c`, `src/entities/entities.c`, `src/gameplay/gameplay.c`; nenhum `.c` ou pasta nova; `gameplay.c` e `game.c` não incluem Allegro.
- C11, nível de aviso 4: **0 avisos** em Debug e Release x64 (`"C:/Program Files/Microsoft Visual Studio/18/Community/MSBuild/Current/Bin/MSBuild.exe" Nanobo.vcxproj -p:Configuration=Debug -p:Platform=x64 -t:Build -v:minimal -nologo`).
- Arquivos CRLF e UTF-8; editar por âncora (nunca reescrever arquivo inteiro, nunca `sed -i`); depois de cada arquivo, `python -c "import pathlib; b=pathlib.Path('ARQ').read_bytes(); print(b.count(b'\r\n'), b.count(b'\n'))"` com os dois números iguais.
- Tela 640×360, fonte embutida de 8 px por caractere; sem maiúscula acentuada; hífen ASCII; botões em caixa alta ASCII.
- "Arma", nunca "ferramenta", em todo texto desenhado.
- `PLAY_H` 320 e `PANEL_H` 40 não mudam.
- Todo texto desenhado novo ou mudado vai para `docs/game-design/textos-do-jogo.md` com ID e contagem; `python ../Nanobo-testes/conferir_textos.py .` termina com 0 e 0.
- Harness: `bash ../Nanobo-testes/build_harness.sh` (copia as fontes para `%TEMP%/nanobo-harness`), depois no PowerShell `cmd /c "cd /d $env:TEMP\nanobo-harness && .\build.bat"; & "$env:TEMP\nanobo-harness\harness.exe" | Select-Object -Last 3`. Antes do plano: **513 passaram, 0 falharam**. O harness fica fora do git e é CRLF.
- Commits: Conventional Commits, assunto em português, **uma linha só, sem corpo e sem `Co-Authored-By`**; branch `feat/redesenho-interface`.
- Capturas: `powershell -NoProfile -ExecutionPolicy Bypass -File ..\Nanobo-testes\qa_jogo.ps1 -Exe <x64\Debug\Nanobo.exe> -Out <pasta no scratchpad> -Steps "..."` (passos: teclas, `waitNNN`, `shot_NOME`, `click_X_Y`, `rclick_X_Y`); fechar o jogo depois (`Stop-Process -Name Nanobo -Force`). Teclas Debug: F conclui a fase, G força Game Over, J revela camadas, K põe biofilme e nuvem.

## Review Focus

1. **Meta que volta a subir** (esporo germina, rajada de vírions depois de zerar): o aviso volta para a meta da fase, e duas viradas a menos de 1 s não geram dois avisos — testes na Task 1.
2. **Aviso com o Diário aberto**: um aviso disparado com anotação pendente só começa a contar quando ela fecha — teste na Task 1.
3. **"Como jogar" aberto pela Pausa**: voltar leva à Pausa (não ao menu) e a partida continua parada; do menu, volta ao menu — testes na Task 6.
4. **Reinício pelo Game Over/Vitória**: o aviso de objetivo dispara de novo no primeiro quadro da nova partida — teste na Task 1.
5. **Vitais e integridade fora da faixa** (0, negativo, acima do máximo): segmentos entre 0 e 10, pulso entre 0,4 e 1,0 s — testes na Task 1.

---

## Onda 1 — durante a fase

### Task 1: Regras puras da interface (segmentos, pulso, mira, aviso de objetivo)

**Files:**
- Modify: `src/gameplay/gameplay.h`, `src/gameplay/gameplay.c` (inclusive `reset_match`/início de fase e `update_match`)
- Test: `../Nanobo-testes/harness_combate.c`

**Interfaces:**
- Produces (em `gameplay.h`):
  - `#define LIFE_SEGMENTS 10`, `#define OBJECTIVE_NOTICE_TIME 4.0f`, `#define OBJECTIVE_REPEAT_GAP 1.0f`, `#define HEART_PERIOD_FULL 1.0f`, `#define HEART_PERIOD_EMPTY 0.4f`
  - `int life_segments(float value, float max);` — `ceil(value/max*10)`, limitado a 0..10 (`max <= 0` → 0).
  - `float heart_period(float vitals);` — 1,0 s com `PATIENT_VITALS_MAX`, linear até 0,4 s em 0, limitado aos extremos.
  - `typedef enum { AIM_READY = 0, AIM_RELOAD = 1, AIM_LOCKED = 2 } AimFrame;` e `AimFrame aim_frame(void);` — `AIM_LOCKED` se `defense_state() == 1`, `fire_locked()`, ou arma Reserva com `reserve_charges_left() <= 0`; senão `AIM_RELOAD` se `!attack_ready()`; senão `AIM_READY`.
  - `typedef enum { GOAL_PHASE = 0, GOAL_DESTINATION = 1 } ObjectiveGoal;`, `ObjectiveGoal objective_goal(void);` (`threats_left() == 0` → destino), `float objective_notice_left(void);` (segundos restantes do aviso visível, 0 se nenhum) e `ObjectiveGoal objective_notice_goal(void);` (a meta que o aviso mostra).
- Regras do aviso (em `update_match`, com estado interno em `gameplay.c`): dispara no primeiro quadro da fase e a cada virada de `objective_goal()`; uma virada a menos de `OBJECTIVE_REPEAT_GAP` da anterior troca a meta mostrada sem reiniciar o tempo; enquanto `pending_card() != CARD_NONE` o tempo não corre; `reset_match` e a entrada numa fase rearmam o disparo inicial.

- [ ] **Step 1: Escrever os testes que falham** (bloco novo no harness):
  - `check("segmentos cheios", life_segments(100.0f, 100.0f) == 10)`; `check("segmentos 55 -> 6", life_segments(55.0f, 100.0f) == 6)`; `check("segmentos 0", life_segments(0.0f, 100.0f) == 0)`; `check("segmentos negativo", life_segments(-5.0f, 100.0f) == 0)`; `check("segmentos acima", life_segments(130.0f, 100.0f) == 10)`
  - `check("pulso cheio 1 s", near(heart_period(PATIENT_VITALS_MAX), 1.0f))`; `check("pulso zero 0,4 s", near(heart_period(0.0f), 0.4f))`; `check("pulso metade 0,7 s", near(heart_period(PATIENT_VITALS_MAX / 2.0f), 0.7f))`; `check("pulso negativo limitado", near(heart_period(-10.0f), 0.4f))`
  - `check("mira pronta no inicio", aim_frame() == AIM_READY)` após `reset_match()`; depois de um disparo, `check("mira em recarga", aim_frame() == AIM_RELOAD)`; com a defesa ativa, `check("mira travada na defesa", aim_frame() == AIM_LOCKED)`; com Reserva e 0 cargas, `check("mira travada sem Reserva", aim_frame() == AIM_LOCKED)`
  - `check("aviso no primeiro quadro", objective_notice_left() > 3.9f && objective_notice_goal() == GOAL_PHASE)` após `reset_match(); update_match(0.016f)` (dispensando a anotação de abertura, se houver, com `dismiss_card()`)
  - `check("aviso some em 4 s", objective_notice_left() == 0.0f)` após `run(4.1)`
  - zerando as ameaças: `check("aviso do destino", objective_notice_goal() == GOAL_DESTINATION && objective_notice_left() > 3.9f)`
  - meta que volta: depois de zerar, `spawn_ground_spore` + germinação (ou ameaça reposta) com `run` até `threats_left() > 0`: `check("meta volta a fase", objective_notice_goal() == GOAL_PHASE)`
  - duas viradas em 0,5 s: `check("viradas rapidas nao reiniciam o aviso", objective_notice_left() < OBJECTIVE_NOTICE_TIME - 0.4f)`
  - com anotação pendente: `queue_card(CARD_GRAM); run(2.0)` → `check("aviso espera o diario", objective_notice_left() > 3.9f)`
  - `check("aviso volta depois do reset", ...)` repetindo o primeiro check após `reset_match()`
- [ ] **Step 2: Rodar o harness e ver os checks novos falharem** (não compila ou FAIL).
- [ ] **Step 3: Implementar** as funções e o estado em `gameplay.c`.
- [ ] **Step 4: Rodar o harness** — esperado: 513 + novos, 0 falharam; build Debug 0 avisos.
- [ ] **Step 5: Commit** — `feat(gameplay): adiciona as regras da mira, das barras e do aviso de objetivo`

### Task 2: Carga dos bitmaps novos

**Files:**
- Modify: `src/main.c` (`load_resources`, `free_resources`, globais de bitmap)

**Interfaces:**
- Produces (globais `ALLEGRO_BITMAP*` no `main.c`, mesmo padrão dos existentes): `nanobo_icon` (`assets/sprites/icone_nanobo.png`), `heart_pulse` (`coracao_pulso.png`), `battery_bar` (`barra_bateria.png`), `ecg_bar` (`barra_ecg.png`), `bar_segment` (`segmento.png`), `aim_sprite` (`mira.png`), `menu_background` (`assets/backgrounds/fundo_menu.png`), `menu_cells` (`hemacias_deriva.png`), `howto_background` (`fundo_como_jogar.png`), `hospital_opening`, `hospital_victory`, `hospital_game_over` (`hospital_abertura/vitoria/gameover.png`).
- Falta de arquivo = `load_resources()` retorna false (regra atual); liberar todos em `free_resources()`.

- [ ] **Step 1: Carregar e liberar os 12 bitmaps.**
- [ ] **Step 2: Verificar** — build Debug 0 avisos; `qa_jogo.ps1` com `wait1500,shot_menu` abre o jogo e a captura do menu sai igual à de antes (nada novo é desenhado ainda); com um PNG novo renomeado temporariamente, o jogo sai com código 1 (desfazer o renome).
- [ ] **Step 3: Atualizar** `assets/catalogo-recursos.md` (contagem de bitmaps em `load_resources()`: 27 → 39) e o `AGENTS.md` local (mesma contagem).
- [ ] **Step 4: Commit** — `feat(ui): carrega a arte nova da interface`

### Task 3: HUD com a coluna de vida

**Files:**
- Modify: `src/main.c` (`draw_hud`, linhas ~2473–2662), `docs/game-design/textos-do-jogo.md`

**Interfaces:**
- Consumes: `life_segments`, `heart_period` (Task 1); `nanobo_icon`, `heart_pulse`, `battery_bar`, `ecg_bar`, `bar_segment` (Task 2).
- Produces: `void draw_life_bar(ALLEGRO_BITMAP* frame, float x, float y, int segments, ALLEGRO_COLOR color);` — moldura em (x, y) e cada segmento `i < segments` em `(x + 2 + 6*i, y + 2)` com `al_draw_tinted_bitmap`. Reutilizada pela Task 6 (página 4).

- [ ] **Step 1: Coluna de vida** — linha 0: `nanobo_icon` em (8, centrado na linha) + `draw_life_bar(battery_bar, 28, y, life_segments(player.health, PLAYER_MAX_HEALTH), UI_CYAN)`; linha 1: quadro de `heart_pulse` + `draw_life_bar(ecg_bar, 28, y, life_segments(patient_vitals, PATIENT_VITALS_MAX), cor da faixa atual)`. Quadro do coração: com `p = heart_period(patient_vitals)` e `t = fmod(match_time, p)`, quadro 1 se `t < 0.15*p`, 2 se `t < 0.30*p`, senão 0.
- [ ] **Step 2: Linha 1** — `pontos N · mm:ss` (HD-01) a partir de x 100 em `UI_TEXT`/`UI_DIM`; o contador `ameaças x/y · identificados a/b` continua à direita; o objetivo âmbar sai do HUD. Linha 0 à direita fica como está (arma, ícone, recarga, estado), com os textos HD-01.
- [ ] **Step 3: Registrar** os textos HD-01 em `textos-do-jogo.md`; `conferir_textos.py` 0/0.
- [ ] **Step 4: Verificar** — build 0 avisos; harness verde; capturas: HUD no início (vitais verdes), depois de ferir células até âmbar e vermelho (atirar em células do Léo com clique), e o pior caso da linha 0 (Fase 2, Neutralização em recarga): nada se sobrepõe.
- [ ] **Step 5: Commit** — `feat(ui): mostra a integridade e os vitais em barras com ícones`

### Task 4: Mira em sprite, aviso de objetivo e termo destacado

**Files:**
- Modify: `src/main.c` (bloco da mira em `draw_game`, ~3154–3166; `draw_game` para o aviso), `src/gameplay/gameplay.c` se os textos OB ficarem lá (preferir `main.c`), `docs/game-design/textos-do-jogo.md`

**Interfaces:**
- Consumes: `aim_frame`, `objective_notice_left`, `objective_notice_goal` (Task 1); `aim_sprite` (Task 2).
- Produces: `void draw_marked_text(float x, float y, ALLEGRO_COLOR base, const char* text);` — desenha uma linha com `{termo}` em `UI_AMBER` e o resto em `base`, sem desenhar as chaves; `int marked_text_width(const char* text);` — largura em px sem as chaves. Reutilizadas pela Task 6. Se o Diário já tiver uma função equivalente no `main.c`, extrair dela em vez de duplicar.

- [ ] **Step 1: Mira** — quadro `aim_frame()` de `aim_sprite` (16×16, origem no quadro × 16), centrado no mouse, `al_draw_tinted_bitmap` com a cor da arma atual. Tabela (uma por `Laser`): Ruptura `UI_PURPLE`, Dissolução `UI_PINK`, Reserva `UI_AMBER`, Neutralização `UI_CYAN`, Ataque T `UI_GREEN`, Perfuração `UI_WARM`. Mesma condição de hoje (só em `GAME`, dentro da área de jogo).
- [ ] **Step 2: Aviso de objetivo** — com `objective_notice_left() > 0`: texto OB-01/02/03 pela fase (meta `GOAL_PHASE`) ou OB-04 (`GOAL_DESTINATION`), via `draw_marked_text` em `UI_WARM`, numa placa `UI_INK` com borda `UI_AMBER`, centrada em y 8; esmaece no último 0,5 s (alfa = `left / 0.5`); com a ficha do scanner no topo, desce para logo abaixo dela (mesma regra do feedback).
- [ ] **Step 3: Registrar** OB-01..04 (contando sem as chaves); `conferir_textos.py` 0/0.
- [ ] **Step 4: Verificar** — capturas: aviso no início da Fase 1 com `destino` em âmbar; aviso OB-04 depois de zerar (tecla F conclui a fase, então zerar atirando ou com H/J); mira nos 3 quadros (parada, logo após um clique, com X apertado) em duas armas diferentes.
- [ ] **Step 5: Commit** — `feat(ui): desenha a mira em sprite e o aviso de objetivo`

### Task 5: Ficha do scanner e feedback

**Files:**
- Modify: `src/main.c` (`draw_scan_card` ~1106 e helpers de texto da ficha), `src/gameplay/gameplay.c` (textos FB que mudam), `docs/game-design/textos-do-jogo.md`
- Test: `../Nanobo-testes/harness_combate.c` (checks que comparam os textos FB alterados)

**Interfaces:**
- Consumes: bitmaps dos agentes já carregados (`coco_a`, `bacilo_a`, `celula_paciente_a`, `virus_a`, `levedura_a`, `bolor`, `esporo`, `silhueta`, conforme os nomes no `main.c`).

- [ ] **Step 1: Layout SC-01** — retrato 24×24 em (14, y+5): bitmap do agente escalado com `al_draw_scaled_bitmap`, `silhueta` antes da camada 1 (para fungo sem leitura, a silhueta como no mundo); texto a partir de x 46; o ícone do scanner sai. Marcadores 6×6 à direita da linha de estado com rótulos SC-11 `forma` e SC-12 `Gram e espécie` (ou SC-13 `estrutura` em vírus, célula e fungo): apagado `UI_SLOT`, aceso `UI_AMBER`, o 2º verde com a identificação. Estado + origem até 50 caracteres.
- [ ] **Step 2: Textos** — SC-01 e SC-07 na ficha; FB-08 (Fase 1) e FB-08b (Fase 3, escolhido por `phase_has_fungi()`), FB-03/FB-04 com "eliminada", e as demais linhas do FB-08 do inventário. Ajustar os checks do harness que comparam esses textos.
- [ ] **Step 3: Registrar** SC e FB em `textos-do-jogo.md`; `conferir_textos.py` 0/0.
- [ ] **Step 4: Verificar** — harness verde; build 0 avisos; capturas da ficha: alvo sem leitura (silhueta, marcadores apagados), depois de 1 camada (botão direito; `rclick`), identificado (J), num vírus (Fase 2) e num fungo (Fase 3).
- [ ] **Step 5: Commit** — `feat(ui): mostra retrato e camadas na ficha do scanner`

### Checkpoint da Onda 1

- [ ] Build Debug e Release 0 avisos; harness verde; `conferir_textos.py` 0/0.
- [ ] Enviar ao dono as capturas das Tasks 3–5 e esperar o ok antes da Onda 2 (ajustes pedidos entram como commit `fix(ui): ...` antes de seguir).

---

## Onda 2 — fora da fase

### Task 6: Cena "como jogar"

**Files:**
- Modify: `src/scenes/scenes.h` (enum), `src/core/game.c`, `src/core/game.h`, `src/main.c` (entrada, desenho, botões do menu e da Pausa), `docs/planejamento/fluxo-estados.md`, `docs/game-design/textos-do-jogo.md`, `AGENTS.md` (local, parágrafo de transições)
- Modify: `../Nanobo-testes/build_harness.sh` (copiar `src/core/game.c`, `game.h` e `src/scenes/scenes.h` e compilar `game.c` junto) — fora do git
- Test: `../Nanobo-testes/harness_combate.c`

**Interfaces:**
- Produces:
  - `GAME_SCENE_HOWTO` no enum, **depois de `GAME_SCENE_GAME_OVER` e antes de `GAME_SCENE_EXIT`**; o limite de `game_set_scene` passa a `GAME_SCENE_HOWTO`.
  - Transições novas em `game_set_scene`: `MENU→HOWTO`, `PAUSE→HOWTO`, `HOWTO→MENU` e `HOWTO→PAUSE` — a volta só vale para a cena de origem.
  - `GameScene game_howto_origin(void);` em `game.h` — a cena de onde o "como jogar" abriu.
  - No `main.c`: `int howto_page` (0..3), zerado a cada entrada; `void draw_howto(void);`.
- Consumes: `draw_marked_text` (Task 4), `draw_life_bar` (Task 3), `howto_background`, `nanobo_icon`, `heart_pulse` (Task 2).

- [ ] **Step 1: Escrever os testes que falham** (harness):
  - `check("menu abre o como jogar", (game_set_scene(GAME_SCENE_HOWTO), game_get_scene() == GAME_SCENE_HOWTO))` partindo do menu
  - `check("como jogar volta ao menu", ...)` com `game_set_scene(GAME_SCENE_MENU)`; `check("do menu nao volta para a pausa", ...)` (pedir `PAUSE` a partir do HOWTO aberto pelo menu não muda a cena)
  - da Pausa: `check("pausa abre o como jogar", ...)`, `check("origem e a pausa", game_howto_origin() == GAME_SCENE_PAUSE)`, `check("como jogar volta a pausa", ...)`, `check("partida parada no como jogar", ...)` (posição de um agente igual antes e depois de um tique do laço com a cena em HOWTO — só `GAME` avança)
  - `check("game nao abre o como jogar direto", ...)` (`GAME→HOWTO` recusado)
- [ ] **Step 2: Rodar e ver falhar.**
- [ ] **Step 3: Implementar** enum, transições e `game_howto_origin` (a origem é gravada no momento da entrada).
- [ ] **Step 4: Cena no `main.c`** — fundo `howto_background`; painel central; coluna de ícone 48×48 (p. 1 Nanobô do spritesheet linha 1 quadro 0 em ×1; p. 2 `icone_scanner` ×2 + 2 marcadores em código; p. 3 os 6 ícones de arma ×1 em grade 2×3; p. 4 `nanobo_icon` e `heart_pulse` ×2); textos HW-00..HW-04 com `draw_marked_text` em `UI_TEXT`; rodapé `n/4` e HW-00b (HW-00c na última página). Teclas: Enter, Espaço ou clique avançam (na última, voltam); ←/→ trocam de página; Esc volta. Menu ganha o botão COMO JOGAR (MN-01) entre INICIAR e RECORDES; Pausa ganha o botão COMO JOGAR (o resto da Pausa é a Task 8).
- [ ] **Step 5: Docs** — `fluxo-estados.md` com as 4 transições; textos HW registrados; `conferir_textos.py` 0/0; parágrafo de transições do `AGENTS.md` local.
- [ ] **Step 6: Verificar** — harness verde; build 0 avisos; capturas das 4 páginas pelo menu; Esc da página 2 volta ao menu; Pausa → COMO JOGAR → Esc volta à Pausa com a fase parada.
- [ ] **Step 7: Commit** — `feat(ui): adiciona a cena como jogar`

### Task 7: Menu com fundo e hemácias

**Files:**
- Modify: `src/main.c` (`draw_menu` ~1205, painel RECORDES ~3421), `docs/game-design/textos-do-jogo.md`

**Interfaces:**
- Consumes: `menu_background`, `menu_cells` (Task 2); botão COMO JOGAR (Task 6).

- [ ] **Step 1: Fundo** — `menu_background`; `menu_cells` deslizando para a esquerda a 8 px/s (deslocamento = `fmod(tempo_real * 8, 640)`), desenhada em `-offset` e `640 - offset`; o Nanobô do spritesheet (linha 1, quadros parados 0–3 a 220 ms) à direita do título, com `y += 2*sin(2π·t/2)`. O tempo vem de `al_get_time()` (o menu não tem `match_time`).
- [ ] **Step 2: Conteúdo** — saem os painéis "Como jogar" e "Objetivo da missão" (MN-05); título, subtítulo `DEFENDA O ORGANISMO`, botões INICIAR / COMO JOGAR / RECORDES e as linhas MN-01; RECORDES com os textos MN-06.
- [ ] **Step 3: Registrar** MN-01, MN-05 (saem) e MN-06; `conferir_textos.py` 0/0.
- [ ] **Step 4: Verificar** — capturas do menu em dois momentos (as hemácias andaram), com foco em cada botão, e o painel RECORDES aberto.
- [ ] **Step 5: Commit** — `feat(ui): desenha o menu sobre o vaso sanguíneo`

### Task 8: Pausa e Resultados sobre o mapa

**Files:**
- Modify: `src/main.c` (`draw_pause` ~3199, `draw_results` ~3321, `screen_buttons`), `docs/game-design/textos-do-jogo.md`

**Interfaces:**
- Consumes: mapa da fase atual já carregado por `load_phase_map()`; COMO JOGAR na Pausa (Task 6).

- [ ] **Step 1: Pausa** — continua sobre o quadro congelado do jogo, com véu mais escuro; painel com PA-01: título, linha da fase, objetivo completo da fase atual (3 linhas por fase), lembrete de teclas e os botões CONTINUAR e COMO JOGAR; `esc continua`.
- [ ] **Step 2: Resultados** — antes dos painéis, o mapa da fase recém-concluída desenhado como em `draw_game` (mesmo tinte) e um véu escuro por cima; textos CT-45 (na última fase, a linha `vitais do Léo: %s (%d)`, sem "a seguir") e rótulos com "eliminadas"; CT-37 já aprovado não muda.
- [ ] **Step 3: Registrar** PA e CT-45; `conferir_textos.py` 0/0.
- [ ] **Step 4: Verificar** — capturas: Pausa nas 3 fases; Resultados das 3 fases (F em cada fase, ENTER para seguir).
- [ ] **Step 5: Commit** — `feat(ui): mostra a pausa e os resultados sobre o mapa da fase`

### Task 9: Abertura, Vitória e Game Over no hospital

**Files:**
- Modify: `src/main.c` (`draw_briefing` ~2465, `draw_victory` ~3531, `draw_game_over` ~3611), `docs/game-design/textos-do-jogo.md`

**Interfaces:**
- Consumes: `hospital_opening`, `hospital_victory`, `hospital_game_over` (Task 2).

- [ ] **Step 1: Abertura** — `hospital_opening` atrás da caixa do Diário (o véu de 60% fica).
- [ ] **Step 2: Vitória** — `hospital_victory`; textos VI-01 (com a linha do Nanobô VI-03); painéis de estatística, Top 10 e nome descendo o necessário para caber a linha nova; campo de nome, limite de 10 caracteres e NOVA PARTIDA preservados.
- [ ] **Step 3: Game Over** — `hospital_game_over`; textos GO-01 (causa pelo vitais ou pela integridade, tratamento convencional, linha do Nanobô GO-04); painel descendo o necessário.
- [ ] **Step 4: Registrar** VI e GO; `conferir_textos.py` 0/0.
- [ ] **Step 5: Verificar** — capturas: abertura (ficha do paciente), Vitória (F nas 3 fases, ENTER), com o campo de nome vazio **sem digitar nome**, Game Over pelos vitais e pela integridade (G força um; para o outro, deixar o Nanobô apanhar ou usar o caminho Debug equivalente).
- [ ] **Step 6: Commit** — `feat(ui): conta a abertura, a vitória e o game over no quarto do hospital`

### Task 10: Fechamento

**Files:**
- Modify: `docs/game-design/textos-do-jogo.md` (conferência final), `assets/catalogo-recursos.md` (arte da #246 marcada "em uso"), `docs/planejamento/kanban.md`, `docs/planejamento/qa-memoria.md`

- [ ] **Step 1: Memória** — Release x64; `../Nanobo-testes/medir_memoria.ps1` iniciado **antes** do jogo, partida pelas 3 fases; pico de working set < 100 MB (se passar de 90 MB, carregar os três fundos do hospital sob demanda, como o mapa da fase, e medir de novo). Registrar o número em `qa-memoria.md`.
- [ ] **Step 2: Docs** — catálogo com a arte em uso e a contagem de bitmaps; kanban com o estado da #245 e da #246.
- [ ] **Step 3: Verificação final** — build Debug e Release 0 avisos; harness verde; `conferir_textos.py` 0/0; capturas de todas as telas listadas na seção Verificação da spec.
- [ ] **Step 4: Commit** — `docs: registra a memória e o estado do redesenho da interface`
