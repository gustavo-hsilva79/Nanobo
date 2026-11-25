# Nanobô — fungos da Fase 3: levedura, bolor, esporos e Perfuração

> **Criado em:** 25/11/2026, ao fechar a Fase 3 (#226, subtarefas #227–#233). A Fase 3 entrou no build na branch `feat/fase3`, em nove tarefas (commits `a4255ae` a `2be0d6e`, mais esta documentação). O merge na `master` só acontece com o sim do dono.
> Este arquivo descreve o que o **build faz**, com os valores do código (`src/gameplay/gameplay.h`, `src/entities/entities.h`, `src/gameplay/gameplay.c`). Onde o código divergir daqui, vale o código, e este arquivo se corrige.
> As decisões D1–D8 são do dono (24–25/11/2026). As R1–R6 foram tomadas pelo controlador da implementação e aceitas na revisão. A ordem de autoridade foi: decisões do dono, `decisoes.md`, corpo das issues #227–#233, `gdd.md`, `textos-do-jogo.md`, código.

## Resumo

- **Local:** o pulmão, num mapa de alvéolos e capilares (`mapa_fase3.png`, 1920×512).
- **Composição (`phases[]`, `PHASE_COUNT` 3):** 4 leveduras (*Cryptococcus*), 2 colônias de bolor (*Aspergillus*) e 3 células alveolares do paciente, nove agentes em ids de 1 a 9. Mais os esporos de carga (§"Carga de esporos") e o que a fase gerar sozinha.
- **Ferramentas:** só a **Perfuração** (antifúngico, tecla 1) e a **Ruptura** (tecla 2). A tecla 3 não tem ferramenta na fase. A roda do mouse alterna as duas. A fase **começa na Ruptura** (D8).
- **Lição da fase (verificada em 25/11, [`conteudo-biologico.md`](conteudo-biologico.md)):** a parede do fungo é de quitina, não de peptidoglicano, então o antibiótico não age (a causa é o alvo ausente; o texto seguro é "antibiótico de bactéria não tem alvo no fungo"); a **Perfuração é inspirada na anfotericina B**, que se liga ao ergosterol e forma poros e é tóxica para o paciente, e o alvo do antifúngico é o ergosterol da membrana. O jogador erra a arma de propósito, ou não, e o jogo cobra com a trava de 2 s.
- **Conclusão:** zero fungos vivos e o Nanobô no destino. Vale como ameaça tudo o que é fungo: levedura, colônia, esporo no chão e esporo em voo.
- **Fim de jogo:** a Fase 3 é a última. `CONTINUAR` nos Resultados leva a `VICTORY` (`has_next_phase()` é falso nela).

## Espécies e números

`Species` ganhou `SPECIES_YEAST` (4), `SPECIES_MOLD` (5) e `SPECIES_SPORE` (6), no fim do enum, sem reordenar as outras. `Laser` ganhou `LASER_PERFORATE` (5).

| agente | raio | resistência | velocidade | observação |
|---|---:|---:|---|---|
| levedura (`YEAST_*`) | 18 | 3 (cápsula) | vaga a 15 px/s; em alerta, 35 px/s | brota |
| colônia de bolor (`MOLD_*`) | 20 | 1 | 0: fixa; não vaga, não é empurrada por colisão nem por corrente | cresce a hifa e lança esporos |
| esporo no chão (`SPORE_RADIUS`) | 4 | 1 dose | parado | germina; pode ser empurrado |
| segmento de hifa (`HYPHA_RADIUS`) | 7 | corta com 1 tiro de Perfuração | fixo | é parede |

- `MAX_AGENTS` é 24. A fase começa com 9 agentes, e o resto das vagas é de brotos e esporos.
- A fase **não tem correnteza** (R2): `currents` vazio, `current_count` 0.

## Levedura

- **IA (#228):** usa o alerta dos outros invasores: entra em alerta a `ALERT_RANGE` (200 px) ou ao levar tiro, volta a vagar a 320 px por 3 s, com carência de `AI_GRACE_TIME` (5 s). Em alerta, avança no Nanobô a `YEAST_ALERT_SPEED` (35 px/s). O contato fere como o das outras espécies, 10 de integridade.
- **Brotamento:** a cada `YEAST_BUD_TIME` (20 s) ± `YEAST_BUD_JITTER` (3 s), sorteado por levedura, uma levedura viva gera outra ao lado, a 2 × raio e dentro do mundo. O teto é `YEAST_CAP`, **12 leveduras vivas**.
  - O filho reaproveita o primeiro slot morto com índice ≥ `agents_initial()` ou vai para o fim do array (`spawn_agent_slot()`), nasce com a resistência cheia e sem leitura, soma uma ameaça na fase e na partida, e **não** conta para a identificação.
  - Feedback FB2-14 (`a levedura brotou: nasceu outra`) só aparece quando nenhum outro feedback está no quadro.
- **Cápsula:** 3 doses de Perfuração eliminam a levedura. A dose **não expira e não cria resistência** (R4).
- **Scanner:** camada 1 `forma: levedura (arredondada, brota)`, camada 2 `eucarionte · parede de quitina · membrana com ergosterol`; categoria `fungo`, sem amostra de cor. A leitura completa conta para `identified_count` dos agentes iniciais.

## Bolor

- **Hifa (#229, D6):** a colônia nasce sem hifa. Cresce **1 segmento a cada `HYPHA_TIME` (5 s) ± `HYPHA_JITTER` (1 s)**, até `HYPHA_MAX` (**5**).
  - A direção é fixada quando a colônia nasce, rumo à **célula do paciente viva mais próxima** (`aim_hypha`); sem célula, para a direita.
  - O segmento `k` (de 1 a 5) fica centrado em `base + direção × (MOLD_RADIUS + HYPHA_GAP + HYPHA_STEP × (k − 1))`, ou seja, `20 + 8 + 16 × (k − 1)` px da base, com raio de colisão 7.
- **A hifa é parede:** o Nanobô que encosta num segmento é empurrado para fora (preso ao mundo) e leva **`HYPHA_DAMAGE` (10) de integridade**, **no máximo uma vez por segundo** (a invulnerabilidade de 1 s). A defesa **não protege** da hifa. Agentes não colidem com ela. Feedback `hifa: -10 de integridade`.
- **A hifa não fere a célula do paciente** (D6) e não é alvo de pontos. Um tiro de Perfuração que toca um segmento (não a base) **corta 1 segmento, o da ponta**, sem pontos, com o feedback FB2-12 (`corte a base da colônia`), e consome o projétil. Só a Perfuração corta; o antibiótico na hifa dá FB2-11. Se o projétil toca ao mesmo tempo um agente e um segmento, **vale o agente**.
- **Esporos em voo:** com a hifa em 5 **e** a colônia em alerta (Nanobô a ≤ 200 px, ou colônia atingida), a cada `SPORE_INTERVAL` (5 s) a colônia lança `SPORE_COUNT` (**3**) `SHOT_SPORE` em leque (−20°, 0°, +20°, `SPORE_SPREAD`) na direção do Nanobô, saindo da **ponta da hifa**, a `SPORE_SPEED` (80 px/s), com voo de `SPORE_FLIGHT` (**1,5 s**).
  - O esporo que acerta o Nanobô tira `SPORE_DAMAGE` (**8**) de integridade, com o feedback `esporo do bolor: -8 de integridade`, e some sem pousar.
  - A **defesa destrói** o esporo em voo, com o feedback `a defesa destruiu o esporo` (o texto da toxina foi corrigido).
- **Esporo no chão (D5, `SPECIES_SPORE`):** ao fim do voo (ou ao sair do mundo), o esporo vira um agente ameaça de raio 4.
  - **1 dose** de Perfuração o elimina. **Não dá pontos** e **não causa dano de contato** (decisões do dono de 25/11). Antibiótico nele dá FB2-11.
  - É um agente comum: pode ser empurrado e conta em `phase_threats` e `run_progress.threats`, mas não conta para a identificação.
  - **Germina em `GERMINATE_TIME` (6 s)** numa colônia nova (hifa 0, direção pela D6), se houver menos de **`MOLD_CAP` (4)** colônias vivas. Com 4, **espera a vaga** e germina assim que alguma morrer. Também espera enquanto a colônia nova sobreporia uma célula do paciente (§"Correções da revisão final").
  - Não há mensagem na germinação.
- **Scanner:** colônia, camada 1 `forma: bolor (filamentos)` e camada 2 `eucarionte · parede de quitina · corte a base da colônia`; esporo no chão, camada 1 `esporo de bolor · vai germinar` e camada 2 `eucarionte · parede de quitina · membrana com ergosterol`.

## Ferramentas e combate

| ferramenta | tecla | age em | efeito |
|---|---|---|---|
| **Perfuração** (`LASER_PERFORATE`) | 1 | levedura, colônia, esporo no chão; corta a hifa | 1 dose por tiro; elimina a colônia e o esporo com 1, a levedura com 3; +10 por eliminação de levedura ou de colônia inicial (a germinada não pontua) |
| **Ruptura** (`LASER_GRAM_POSITIVE`) | 2 | nada na fase | em fungo é "arma errada" |

- **Antibiótico em fungo (R5):** não muda a resistência, arma a trava de 2 s (`FIRE_LOCK_TIME`), soma `wrong_decisions` e dá FB2-11 (`a parede do fungo é de quitina: antibiótico não age`). Dissolução e Reserva não existem na fase.
- **Dose na levedura:** o feedback diz quantas doses faltam: `cápsula enfraquecida: faltam 2 doses` depois da 1ª e `cápsula enfraquecida: falta 1 dose` depois da 2ª (FB-33 e FB-33b, revisão final de 25/11). A eliminação usa o feedback de eliminação existente.
- **Perfuração na célula do paciente:** ramo da célula existente, −15 dos vitais (fogo amigo). O "não custa vitais" de `decisoes.md` fala do efeito colateral da arma, que o dono não escolheu; não tira o custo de acertar a célula.
- **Pontuação:** +10 por levedura eliminada e por colônia **inicial** eliminada (índice < `agents_initial()`). O **esporo não dá pontos**, nem em voo nem no chão, e a **colônia que germinou de esporo também não** (decisão do dono de 25/11): ela segue ameaça (conta para a conclusão, para `neutralized` e para `molds_eliminated`), e a eliminação dá o feedback FB-40 `colônia germinada destruída: sem pontos`. O broto de levedura continua valendo +10, como a filha da divisão na Fase 1.
- **O que a fase não afirma:** que o antibiótico cause aspergilose. A carga de esporos pelo estado do paciente é regra de jogo, sem texto que diga isso.
- **Enquadramento do organismo enfraquecido (CC-05c, 25/11):** o risco vem da **defesa fraca**, não de mais esporos inalados; a redação "quanto pior o paciente chega, mais esporos se instalam" inverte a causa e será trocada por "quanto mais fraco o paciente, mais fácil para os esporos se instalarem" (**pendente de redação**; `textos-do-jogo.md` não mudou). Nos textos de fungos oportunistas, evitar "só": *C. gattii* também afeta pessoas saudáveis ([MSD Manual](https://www.msdmanuals.com/pt/casa/infec%C3%A7%C3%B5es/infec%C3%A7%C3%B5es-f%C3%BAngicas/criptococose)); aspergilose atinge sobretudo quem tem a defesa fraca ([CDC](https://www.cdc.gov/aspergillosis/about/)). No arco do jogo, a defesa enfraquecida do Paciente vem do vírus respiratório da Fase 2 ([`decisoes.md`](decisoes.md) §"Narrativa e conteúdo (25/11/2026)").

## Carga de esporos pelo estado do paciente (#231, D4)

`spore_load(vitals, reserve_spent)`: `extras = (vitais < 80) + (vitais < 60) + cargas de Reserva gastas na partida`, com teto `SPORE_LOAD_MAX` (**4**). Os vitais usados são os **do fim da Fase 2, antes do piso de 60** de `start_next_phase`.

| vitais | Reserva gasta | esporos |
|---:|---:|---:|
| 90 | 0 | 0 |
| 79 | 0 | 1 |
| 59 | 0 | 2 |
| 59 | 2 | 4 |
| 30 | 2 | 4 (teto) |
| 100 | 1 | 1 |

- Cada extra nasce como **esporo no chão** num ponto válido de `valid_points[]`, a pelo menos `SPORE_CLEARANCE` (48 px, por eixo) de agentes ativos e do Nanobô, com o tempo de germinação cheio. `spore_load_current()` devolve quantos foram colocados de fato (alimenta o relatório).
- O valor guardado é zerado em `reset_match`.

## Cards, silhueta e relatório (#232, D7)

- **CC-05** (`CARD_WEAKENED`, organismo enfraquecido) abre ao **entrar na Fase 3**. **CC-06** (`CARD_FUNGUS`) abre na **primeira leitura completa** de um fungo **ou** no primeiro antibiótico num fungo, o que vier antes; cada card uma vez por partida. Os textos estão em `textos-do-jogo.md`, §3.1.
- **Silhueta (D7):** os fungos (levedura, colônia e esporo no chão) aparecem como silhueta **até a 1ª camada**, no tamanho de cada um. A **hifa é desenhada mesmo antes da leitura**, porque é parede que machuca.
- **Relatório dos Resultados (5 linhas, contadores próprios):** leveduras eliminadas; colônias eliminadas (inclui as germinadas); tiros de antibiótico em fungo; esporos pelo estado do paciente (a carga colocada); e a frase fixa `fungo: parede de quitina, alvo: ergosterol`. Os contadores zeram em `start_phase`. A tecla F de QA (Debug) não soma neles.
- **Interlúdio:** depois da Fase 2, o subtítulo diz `a seguir: fungos no pulmão` (NR-16).
- **HUD:** objetivo `elimine os fungos e vá ao destino` (CT-42b); ferramenta `Perfuração · antifúngico` (CT-36c).

## Arte (D3)

Gerada por script Lua no Aseprite, no estilo aprovado em 14/11, com os scripts em `../Nanobo-testes/` (`gerar_fungos.lua`, `gerar_tiros_icones.lua`, `gerar_mapa_fase3.lua`). O dono aprovou os sprites e o mapa em 25/11.

- `levedura_a.png` e `levedura_b.png` (40×40, a `_b` com broto), `bolor.png` (48×48), `hifa.png` (16×16), `esporo.png` (8×8), `icone_perfuracao.png` (16×16);
- `tiros.png` passou a 24×72, com a 6ª linha (anel da Perfuração); as cinco linhas antigas não mudaram um pixel;
- `mapa_fase3.png` (1920×512): alvéolos e capilares, sem tinta própria (usa `MAP_TINT`);
- `load_resources()` abre **27 bitmaps**, dos quais 6 entraram em 25/11 (levedura a/b, bolor, hifa, esporo e o ícone da Perfuração), mais o mapa da fase atual. Catálogo em `assets/catalogo-recursos.md`.

## Decisões do dono (24–25/11/2026) e decisões técnicas

| # | Decisão |
|---|---|
| D1 | Trabalho na branch `feat/fase3`; merge na `master` só com o sim do dono. |
| D2 | Ferramentas da fase: **Perfuração** (tecla 1) e **Ruptura** (tecla 2); tecla 3 sem ferramenta; a roda alterna as duas. |
| D3 | Sprites e mapa por script Lua no Aseprite, no estilo de 14/11; capturas aprovadas pelo dono em 25/11. |
| D4 | Carga de esporos: (1 se vitais < 80) + (1 se < 60) + cargas de Reserva gastas, no máximo 4. |
| D5 | Esporo voa 1,5 s, pousa e vira ameaça no chão (1 dose mata), germina em colônia em 6 s; com 4 colônias vivas, espera a vaga. Vale também para os esporos da carga inicial. |
| D6 | A hifa cresce rumo à célula do paciente mais próxima, fixada quando a colônia nasce, e fere o Nanobô (−10) sem ferir a célula. |
| D7 | Fungos como silhueta até a 1ª camada; a hifa se desenha desde o início. |
| D8 | A fase começa com a Ruptura na mão. |
| 25/11 (após a revisão da Task 4) | O esporo, em voo ou pousado, **não dá pontos**; o esporo pousado **não causa dano de contato**. |
| 25/11 (após a revisão final) | A colônia que **germinou de esporo não dá pontos**; só as 2 colônias e as 4 leveduras iniciais (índice < `agents_initial()`) dão +10. A germinada segue ameaça. |

| # | Decisão técnica | Estado |
|---|---|---|
| R1 | Partida e destino continuam os globais (`DESTINATION_X/Y`), como na Fase 2; o mapa 3 foi desenhado para eles. | **dívida**: a #227 pedia partida e destino próprios |
| R2 | Sem correnteza na Fase 3. | feito |
| R3 | Fungos não atacam células do paciente; elas só perdem vitais por tiro do Nanobô (−15). | feito |
| R4 | Doses de Perfuração na levedura não expiram e não criam resistência. | feito |
| R5 | Antibiótico em fungo é "arma errada": sem efeito, trava de 2 s, `wrong_decisions++`, FB2-11. | feito |
| R6 | Sem bots de calibração para a Fase 3; valem os números do `gdd.md`. | **dívida** |

## Comportamentos escolhidos na implementação e aceitos na revisão

- A hifa age como parede: empurra o Nanobô para fora e tira 10 no máximo uma vez por segundo; a defesa não protege.
- O esporo pousado é um agente ameaça comum e pode ser empurrado.
- O tiro que sobrepõe um agente e um segmento de hifa acerta o agente.
- Não há mensagem na germinação; o feedback do brotamento só aparece quando nenhum outro está no quadro.
- CC-05 abre na entrada da Fase 3; CC-06 na primeira leitura completa de fungo ou no primeiro antibiótico num fungo.

## Correções da revisão final (25/11)

- **Colônia germinada sem pontos:** ver a decisão do dono acima e §"Ferramentas e combate".
- **Reinício com tiros no ar:** `start_phase` apaga os tiros inimigos (esporos e vírus da rajada) **antes** de contar as ameaças. Um Game Over com esporos no ar seguido de partida nova não infla mais `run_progress.threats`, e o `ameaças X/Y` da run nova chega a N/N.
- **Dose da levedura:** o texto diz quantas doses faltam (FB-33 e FB-33b).
- **Parede sem dano parado:** o empurrão da hifa e da base da colônia deixa o Nanobô **meio pixel além do alcance** (`reach + 0.5f`). O toque continua ferindo quem entra na parede (−10, no máximo uma vez por segundo), mas o Nanobô parado ao lado dela não perde mais integridade. O empurrão da célula do paciente (Fases 1 e 2) não mudou.
- **Germinação longe da célula:** o esporo pousado só germina se a colônia que nasceria ali não sobrepuser uma célula do paciente viva (distância ≥ `MOLD_RADIUS` + raio da célula). Enquanto isso a germinação espera; o esporo segue ameaça e pode ser destruído, e germina assim que a célula sai de perto.

## Calibração e números de QA

- **Harness de regras:** **482 passaram, 0 falharam** (eram 334 antes da Fase 3 e 470 antes das correções da revisão final). `conferir_textos.py` fecha com 0 divergências.
- **Build:** Debug e Release x64 com 0 avisos.
- **Memória:** pico de **~78,25 MB** de working set com as três fases (Release com as teclas de QA); a Fase 3 roda ~0,4 MB abaixo da Fase 2, porque só um mapa fica carregado por vez. Método e tabela na seção de 25/11 de `docs/planejamento/qa-memoria.md`.
- **Bots:** não há bots da Fase 3 (R6). Os números de brotamento, hifa e esporo são os do `gdd.md`, sem medida de taxa de acerto de leitor e de chutador como nas Fases 1 e 2.

## Dívidas e limites conhecidos

- **Partida e destino próprios** (R1): a Fase 3 usa os globais, como a Fase 2. A #227 pedia pontos próprios.
- **Sem bots de calibração** (R6): a dificuldade da Fase 3 não foi medida.
- **Vagas cheias:** com `MAX_AGENTS` cheio, um esporo some sem aviso, e esporos e brotos de levedura disputam as mesmas vagas livres (≥ `agents_initial()`).
- **Esporos iniciais** ignoram os segmentos de hifa ao escolher o ponto e podem colocar menos esporos que a carga, se não houver ponto válido livre (o relatório registra o número colocado).
- **Broto na borda do mundo** pode nascer sobreposto ao pai; a colisão o separa no quadro seguinte.
- **Tecla F (Debug)** não soma nos contadores do relatório da Fase 3.
- **Mapa** procedural e uniforme, sem marco no meio; aprovado assim pelo dono.
- **Legibilidade de `ê` e `ô`, pré-existente:** os textos com `ê` e `ô` (`Lisogênico` no card da Fase 2, `você`, `colônia`) estão **corretos**, decisão do dono de 25/11. O problema é o tamanho da fonte embutida: no glifo de 8 px o circunflexo encosta na letra, e numa captura reduzida o `ê` lembra `@` e o `ô` lembra `8`. Fica para o redesenho da interface que vem a seguir (`textos-do-jogo.md`, §2.1).
