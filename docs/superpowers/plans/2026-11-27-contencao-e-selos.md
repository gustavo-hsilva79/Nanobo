# Contenção e selos do protocolo — plano de implementação (27/11/2026)

**Spec:** a auditoria de game design de 27/11 (relatório em `C:\Users\GIGABYTE\.claude\plans\pasted-content-id-3beb-audito-optimized-cosmos-of-e8620048-884f-4e0e-b104-4205ab39c94b.md`, §D1, §D2, §D3), com as decisões do dono de 27/11:
- **D1 = opção a:** sem Reserva e só resistentes vivas, a equipe isola as resistentes;
- **D2:** a filha da divisão e o broto de levedura valem 0;
- **D3 = variante (i):** os selos só aparecem, sem pontos.

**Branch:** `feat/contencao-selos`, criada a partir de `refactor/modulos-720p`. Ela não sai da `master`, porque a refatoração de 27/11 ainda não foi mesclada e o plano depende dela. Nada de merge nem push sem o dono.

## Contexto compartilhado (vale para todas as tarefas)

**Arquivos compilados** (`<ClCompile>` de `Nanobo.vcxproj`):
- `src/main.c`, `src/core/game.c`, `src/entities/entities.c`;
- `src/gameplay/gameplay.c`, `src/gameplay/ranking.c`;
- `src/render/world.c`, `src/render/feel.c`;
- `src/ui/ui_kit.c`, `src/ui/hud.c`, `src/ui/screens.c`.

Não crie arquivo novo. As regras ficam em `gameplay.c` (sem Allegro); o desenho fica em `src/render/` e `src/ui/`.

**Estilo de código:**
- C11, WarningLevel 4: o build termina com **0 avisos**;
- 4 espaços, chaves Allman, identificadores em inglês `snake_case`;
- comentários novos em português, minúsculos, `//` curtos;
- não reformate linhas existentes.

**Codificação e quebras de linha:**
- os fontes são UTF-8 e todo arquivo é **CRLF**. Edite por âncora, nunca reescreva o arquivo inteiro;
- confira depois com `python -c "import pathlib; b=pathlib.Path('<arquivo>').read_bytes(); print(b.count(b'\r\n'), b.count(b'\n'))"`: os dois números precisam ser iguais.

**Texto desenhado:**
- português com acento correto, mas **nenhuma maiúscula acentuada** e **nenhum `U+2212`** (use o hífen ASCII);
- a fonte embutida tem 8×8 e é desenhada em 2×, ou seja, 16 px por caractere na tela de 1280×720. Toda coordenada da interface é nativa de 1280×720;
- marcas de cor no texto:
  - `<g:texto>` verde (ganho), `<x:texto>` vermelho (perda);
  - `<wS:Reserva>` na cor da arma;
  - `{termo}` âmbar;
  - `[tecla]` desenha a caixinha de tecla, então **não use colchetes** para outra coisa;
  - as marcas não contam nos caracteres.

**Registro de textos:** toda string nova que o jogo desenha entra em `docs/game-design/textos-do-jogo.md`, numa linha com ID, situação, texto entre crases, número de caracteres (sem as marcas) e origem. `python -I ../Nanobo-testes/conferir_textos.py .` (da raiz do repo) precisa fechar com **0 e 0**.

**Build:**
- `"/c/Program Files/Microsoft Visual Studio/18/Community/MSBuild/Current/Bin/MSBuild.exe" Nanobo.vcxproj -p:Configuration=Debug -p:Platform=x64 -t:Build -v:minimal -nologo`, e o mesmo com `-p:Configuration=Release`;
- feche o `Nanobo.exe` antes de compilar (`Stop-Process -Name Nanobo -Force`), senão dá `LNK1104`.

**Harness (fora do repo, em `../Nanobo-testes/harness_combate.c`, que não está sob git):**
- compilar: `TMPDIR="$TEMP" bash ../Nanobo-testes/build_harness.sh` copia os fontes para `$TEMP/nanobo-harness`. O `cmd` dele falha por causa do caminho; depois rode `cd "$TEMP/nanobo-harness" && cmd.exe //c "$(cygpath -w "$TEMP/nanobo-harness/build.bat")" && ./harness.exe | tail -2`;
- linha de base de 27/11 às 21h40: **570 passaram, 0 falharam**;
- os casos novos usam a função `check(nome, condição)` que já existe, no estilo dos casos vizinhos;
- como o harness não tem git, o relatório da tarefa **lista os casos acrescentados**, com nome e linha.

**Bots (linha de base de 27/11 às 21h40, sobre o código desta branch):**
- `harness.exe bots 1000`: leitor 60% (605 vitórias, 25 tempos esgotados com 1,1 resistente sobrando), chute 2%;
- `bots2 1000 77`: leitor 100%, chute 7%;
- `bots2 1000 60`: leitor 98%, chute 0%;
- metas: leitor da Fase 1 entre 55% e 75%, chute ≤ 10%.

**Commits:**
- Conventional Commits em **uma linha só**: `tipo(escopo): descrição`, com a descrição em português, minúscula, imperativa e sem ponto;
- **sem corpo, sem `Refs`, sem `Co-Authored-By`**;
- autor local `gustavo-hsilva79` (já configurado).

**Não mexer:**
- o orçamento de 15 divisões por fase (`divide_bacteria`, regra do dono de 27/11);
- a IA, as velocidades e os danos;
- os pontos dos vírus da rajada (continuam +10);
- o `main.c`, a não ser que a tarefa diga.

---

### Task 1: filha da divisão e broto de levedura valem 0 pontos (D2)

**Regra:**
- Em `hit_agent` (`src/gameplay/gameplay.c`, perto da linha 633), a eliminação só soma `SCORE_PER_THREAT` quando o agente é um **agente inicial** (`index < agents_initial()`). Hoje a regra já exclui o esporo e a colônia germinada.
- **Exceção:** o vírus da Fase 2 nascido na rajada (`SPECIES_VIRUS` com índice ≥ `agents_initial()`) **continua valendo +10**.
- Em resumo, valem 0 o esporo, a colônia germinada, a filha da divisão (Gram+ e Gram−) e o broto de levedura.
- `run_progress.neutralized` continua somando em todos os casos, e a filha e o broto seguem ameaças para a conclusão.

**Pontos do último abate:**
- Acrescente em `gameplay.c/.h` `int last_kill_points(void)`, que devolve quantos pontos o último abate rendeu (10 ou 0). O valor é gravado em `hit_agent` no abate e zerado em `reset_match`.
- Em `src/render/feel.c`, o número que sobe na eliminação (`snprintf(pop, ..., "+%d", SCORE_PER_THREAT)`) passa a aparecer **só quando `last_kill_points() > 0`**, e mostra esse valor. Isso também corrige o "+10" que hoje aparece no esporo e na colônia germinada.

**Feedback** (em `hit_projectiles`, no ramo `agents[j].identified || j >= agents_initial()`, que hoje diz `ameaça <g:eliminada>: <g:+10 pontos>`):
- filha de bactéria (`species_is_bacterium` e `j >= agents_initial()`): `filha eliminada: sem pontos`;
- broto de levedura (`SPECIES_YEAST` e `j >= agents_initial()`): `broto eliminado: sem pontos`;
- agente inicial: não muda.

**Registro:**
- em `textos-do-jogo.md` §1, depois da FB-40: **FB-41** `filha eliminada: sem pontos` (27) e **FB-42** `broto eliminado: sem pontos` (27), com origem "decisão do dono de 27/11 (contenção)";
- em `docs/game-design/gdd.md`, a linha "+10 por invasor eliminado" ganha a exceção: "(filhas da divisão, brotos, esporos e colônias germinadas não pontuam; vírus da rajada sim)";
- em `docs/game-design/fase3-fungos.md`, a frase "O broto de levedura continua valendo +10, como a filha da divisão na Fase 1" vira "O broto de levedura não pontua, como a filha da divisão na Fase 1 (decisão do dono de 27/11)";
- em `docs/game-design/decisoes.md`, crie a seção `### Contenção e selos (27/11/2026)` logo antes de `### Resolução 1280×720 e módulos (27/11/2026)`, com o item 1 ("Filhas e brotos não pontuam", com o porquê: a pontuação premiava deixar a infecção crescer; o mesmo critério da colônia germinada de 25/11; os recordes antigos do Top 10 ficam acima do alcançável com a regra nova). As tarefas 2 e 3 acrescentam os itens 2 e 3.

**Casos no harness:**
- matar uma bactéria inicial soma 10 e `last_kill_points() == 10`;
- matar uma filha (forçada por `divide_bacteria` ou montada no array com índice ≥ `agents_initial()`) não muda `score` e `last_kill_points() == 0`;
- matar um broto de levedura não muda `score`;
- neutralizar um vírus de rajada (índice ≥ `agents_initial()`, Fase 2) soma 10;
- matar um esporo dá `last_kill_points() == 0`;
- os casos antigos de pontuação que esperavam +10 por filha ou broto são **ajustados para a regra nova**, e o relatório os lista.

**Commits:**
- `feat(gameplay): tira os pontos da filha da divisão e do broto de levedura`
- `docs: registra que filhas e brotos não pontuam`

### Task 2: a equipe isola as resistentes quando a Reserva acaba (D1)

**Regra:** acrescente em `gameplay.c/.h` `bool resistant_isolation_pending(void)`. Ela é verdadeira quando **tudo** abaixo vale:
- `phase_has_bacteria()`;
- `reserve_charges_left() == 0`;
- não há ameaça em voo (`threats_in_flight() == 0`);
- existe pelo menos 1 agente ativo não-célula;
- **todo** agente ativo não-célula é bactéria (`species_is_bacterium`) com `resistant == true`.

**Efeitos:**
- `objective_goal()` devolve `GOAL_DESTINATION` quando `threats_left() == 0` **ou** quando `resistant_isolation_pending()`;
- em `conclude_phase_if_ready()`, a guarda `if (threats_left() > 0) return;` vira "sai se houver ameaça e não houver isolamento pendente";
- quando o Nanobô chega ao destino com o isolamento pendente, **antes** de pagar o bônus de vitais:
  - cada bactéria resistente ativa vira `active = false`, e `patient_vitals` perde `RESISTANT_ISOLATION_VITALS` por bactéria (novo `#define RESISTANT_ISOLATION_VITALS 15.0f` em `gameplay.h`, perto de `CELL_LOSS_VITALS`, com comentário), com piso em 0;
  - o novo contador `progress.resistants_isolated` (em `MatchProgress`, zerado em `start_phase` junto dos outros) soma 1 por bactéria;
  - **nada** muda em `score` nem em `run_progress.neutralized`;
  - se os vitais chegarem a 0 com isso, a fase **não** conclui (não ponha `phase_finished`) e o fluxo normal de Game Over pelos vitais assume;
  - se não chegarem, a conclusão segue normal (bônus de vitais e de identificação) e o feedback é `a equipe isolou as resistentes: <x:-15 vitais cada>`.

**Aviso de objetivo** (`draw_objective_notice` em `src/render/world.c`):
- quando a meta é `GOAL_DESTINATION` e `resistant_isolation_pending()` é verdadeiro, o texto é `Sem {Reserva}: vá ao {destino} e a equipe isola as resistentes` no lugar da OB-04;
- confira a largura: o texto inteiro mais `2 * UI_PAD` cabe em 1280 px.

**Registro:**
- **OB-05** (aviso) e **FB-43** (feedback) em `textos-do-jogo.md`, com a contagem de caracteres sem as marcas;
- `decisoes.md` §"Contenção e selos (27/11/2026)", item 2 ("Resistentes isoladas"), com o porquê: a fase podia ficar sem fim e sem derrota quando só sobravam resistentes e a Reserva acabava (2,5% das partidas do bot leitor); a filha continua herdando a resistência, porque é a seleção que o OA4 ensina;
- `gdd.md` §"Regras centrais" › Tratamento ganha a frase da saída.

**Casos no harness:**
- Fase 1 com Reserva zerada e só 1 resistente ativa: `resistant_isolation_pending()` verdadeiro e `objective_goal() == GOAL_DESTINATION`;
- com o Nanobô posto em `DESTINATION_X/Y` e um `update_match` curto, a fase conclui (`phase_completed()`), a resistente fica inativa, os vitais caem 15 antes do bônus, `score` só sobe pelo bônus de vitais e `progress.resistants_isolated == 1`;
- com 1 carga de Reserva sobrando, fica falso;
- com uma bactéria comum viva junto da resistente, fica falso;
- com os vitais em 10 e 1 resistente, chegar ao destino **não** conclui a fase e os vitais ficam em 0.

**Bots:** se o bot da Fase 1 em `harness_combate.c` só vai ao destino com `threats_left() == 0`, ensine-o a ir também quando `objective_goal() == GOAL_DESTINATION`. Rode `harness.exe bots 1000` e registre no relatório (meta: leitor entre 55% e 75%, chute ≤ 10%, tempos esgotados com resistentes perto de 0).

**Commits:**
- `fix(gameplay): isola as resistentes quando a reserva acaba`
- `docs: registra o isolamento das resistentes`

### Task 3: regras dos selos do protocolo (D3, parte de regra)

**Contadores novos em `MatchProgress`** (`gameplay.h`), com comentário, zerados em `start_phase` junto dos outros:
- `cells_lost`: soma 1 em `lose_patient_cell`;
- `antibody_on_cell`: soma 1 em `virus_tool_hit` quando a Neutralização acerta uma célula, infectada ou saudável;
- `spore_bursts`: soma 1 cada vez que uma colônia solta um leque de esporos em `mold_ai`.

Confira se o rompimento da célula infectada (`burst_cell`) chama `lose_patient_cell`. Se chamar, documente no relatório que `cells_lost` também conta os rompimentos; o selo da Fase 2 usa `cells_burst`.

**Funções novas em `gameplay.c/.h`:**
- `#define SEAL_COUNT 3`;
- `#define SEAL_VITALS_MIN 70.0f`;
- `bool seal_achieved(int k)`, para `k` de 0 a 2, na fase atual:

| fase | k = 0 | k = 1 | k = 2 |
|---|---|---|---|
| bactérias (`phase_has_bacteria()`) | `progress.resistances == 0` | `progress.cells_lost == 0` | `reserve_charges_left() == RESERVE_CHARGES` |
| vírus (`phase_has_viruses()`) | `progress.cells_burst == 0` | `progress.antibody_on_cell == 0` | `patient_vitals >= SEAL_VITALS_MIN` |
| fungos (`phase_has_fungi()`) | `progress.antibiotic_on_fungus == 0` | `progress.spore_bursts == 0` | `progress.cell_attacks == 0` |

- `k` fora de 0 a 2 devolve falso;
- teste a fase de fungos primeiro, depois a de vírus, depois a de bactérias, como `select_tool` faz;
- os selos **não** mudam `score` nem nenhuma outra regra.

**Registro:** `decisoes.md` §"Contenção e selos", item 3 ("Selos do protocolo"):
- a tabela acima, em palavras;
- só exibidos, sem pontos, porque não mudam o ranking e substituem a ideia do bônus de run perfeito (#208) como meta visível;
- cada selo lê um contador que já alimentava o relatório.

**Casos no harness:** para cada fase, um caso com os três selos verdadeiros logo depois de `start_phase` e um caso que quebra cada selo:
- Fase 1: uma resistência (dose e `update_match` de mais de 6 s), uma célula perdida (`lose_patient_cell`) e um disparo da Reserva;
- Fase 2: uma célula rompida, a Neutralização numa célula e os vitais abaixo de 70;
- Fase 3: o antibiótico num fungo, um leque de esporos e um tiro numa célula;
- e `seal_achieved(3) == false`.

**Commits:**
- `feat(gameplay): adiciona as regras dos selos do protocolo`
- `docs: registra os selos do protocolo`

### Task 4: selos no aviso de objetivo e nos Resultados (D3, parte de exibição)

**Rótulos dos selos**, que ficam no `src/ui/` ou no `src/render/` que os desenha, na ordem `k = 0, 1, 2`:

| fase | k = 0 | k = 1 | k = 2 |
|---|---|---|---|
| bactérias | `sem resistência` | `nenhuma célula perdida` | `<wS:Reserva> intacta` |
| vírus | `nenhuma célula rompida` | `anticorpo só em vírus` | `vitais 70+` |
| fungos | `só <wF:Perfuração> no fungo` | `bolor sem esporos` | `células intactas` |

**Aviso de objetivo** (`draw_objective_notice`, `src/render/world.c`):
- quando o aviso mostra a meta da fase (`GOAL_PHASE`), ganha uma **segunda linha** em `UI_DIM`, abaixo do objetivo: `Protocolo: <r0> · <r1> · <r2>`;
- a placa cresce para as duas linhas (use `NOTICE_H` e a altura de linha que o arquivo já usa) e as duas ficam centradas;
- a mais longa (Fase 3, cerca de 72 caracteres sem as marcas) precisa caber em 1280 px com o `UI_PAD` dos dois lados;
- as variantes de `GOAL_DESTINATION` (OB-04 e OB-05) não mudam;
- a regra de descer a placa para baixo da ficha do scanner continua valendo com a altura nova.

**Resultados** (`draw_results`, `src/ui/screens.c`):
- um painel novo, `Protocolo da fase` (título em `UI_AMBER`), com 3 linhas, uma por selo:
  - selo cumprido: `<rótulo>: <g:cumprido>`;
  - selo não cumprido: `<rótulo>: <x:não cumprido>`;
- tudo o que já existe continua: título, interlúdio, painel principal, objetivos secundários, as 5 linhas de "O que anotei nesta fase", botão CONTINUAR e a dica `[enter] também continua`;
- reorganize o eixo vertical para caber em 720 px sem sobreposição. Se mover o botão, atualize o `results_items` em `src/ui/ui_kit.c` para o clique continuar no lugar desenhado;
- a linha de QA (`#ifdef _DEBUG`, F1) também não pode sobrepor nada.

**Registro:** em `textos-do-jogo.md`:
- o prefixo `Protocolo: ` e o título `Protocolo da fase`;
- os 9 rótulos (IDs **PR-01** a **PR-09**);
- os dois sufixos `cumprido` e `não cumprido`;
- tudo com a contagem de caracteres, numa subseção nova `### Selos do protocolo` em §2.

**Capturas:**
- compile o Debug;
- use `../Nanobo-testes/qa_jogo.ps1` (passos `ENTER`, `ESC`, `waitNNN`, `F` para forçar a conclusão no Debug, `shot_NOME`) para capturar:
  - o aviso da Fase 1 no começo (com a segunda linha);
  - os Resultados da Fase 1;
  - depois de `ENTER`, o aviso e os Resultados da Fase 2;
  - depois, os da Fase 3;
- grave as capturas em `.superpowers/sdd/<workspace>/capturas/` e liste os caminhos no relatório;
- se os cards do diário pausarem o roteiro, feche-os com `ENTER`, como o script já permite.

**Commits:**
- `feat(ui): mostra os selos do protocolo no aviso e nos resultados`
- `docs: registra os textos dos selos do protocolo`

### Task 5: conferência final e registro de QA

**Conferências:**
- Debug e Release com 0 avisos;
- harness com 0 falhas (anote o novo total);
- `conferir_textos.py` com 0 e 0.

**Bots:** rode `harness.exe bots 1000`, `bots2 1000 77` e `bots2 1000 60` sobre o código final. Registre em `docs/game-design/ensino-e-aprendizagem.md`, §"Calibração", numa subseção `#### Linha de base depois da contenção e dos selos (27/11)`:
- a tabela antes e depois (antes: leitor 60%, 25 tempos esgotados, chute 2%; Fase 2: 100%/98% e 7%/0%);
- o que mudou e por quê (o isolamento transforma os tempos esgotados em vitória ou derrota).

**Memória:** rode `../Nanobo-testes/medir_memoria.ps1` sobre o Release, seguindo o método de `docs/planejamento/qa-memoria.md`, e acrescente a linha no fim daquele documento. A meta é ficar abaixo de 100 MB; a referência é 89,95 MB.

**`AGENTS.md`** (local, fora do git): na seção do `gameplay.c`, mencione os selos (`seal_achieved`), o isolamento das resistentes e a pontuação só para agentes iniciais (fora o vírus da rajada).

**Commit:** `docs: registra a calibração e a memória depois da contenção e dos selos`
