# Nanobô 2.0 — design de ensino, narrativa, dificuldade, arte e interface

> **Criado em:** 13/11/2026, a partir da rodada de brainstorming com o dono (Gustavo).
> **Substitui**, no que conflitar: `docs/game-design/gdd.md` (núcleo e fases), os objetivos OA1–OA4 de `ensino-e-aprendizagem.md` e a seção de arte de `decisoes.md`. A regra viral de `fase2-virus.md` continua valendo, com os acréscimos da §6.
> **Prazo:** entrega final em 02/12/2026 (Aula 17). O M5 interno vence em 29/11.
> **Equipe:** este redesenho é do Gustavo. O Eric segue com Top 10, instalador e documentação final.

## 1. Por que mudar

Depois de jogar o build de 12/11, o dono avaliou:

- **Está fácil.** Nada pressiona o jogador: as bactérias andam devagar, nada se multiplica e o erro custa pouco.
- **Não é intuitivo.** As bactérias têm a mesma cor da parede do vaso, e o briefing explica pouco.
- **Ensina quase nada.** O aluno sai sabendo que Gram+ fica roxo e Gram− vermelho, e quase só isso.
- **Não tem narrativa.** A única frase de história é "você defendeu o organismo".
- **A arte não combina.** Cada sprite tem um estilo, e nenhum conversa com o mapa pintado.

## 2. Critérios da disciplina que este design precisa atender

Fonte: `Documentos/` (Plano de Ensino PI II 2026, Aula 1 e Aula 2).

| Critério | Como o design atende |
|---|---|
| Tema de aprendizado do **ensino médio** | Biologia do 1º e 2º ano: citologia (procarionte × eucarionte), bactérias (Gram, morfologia, antibiótico e resistência), vírus (ciclos) e sistema imune (inato e adaptativo). Aparece em livros didáticos e no ENEM. |
| **Mecânicas que contribuam para o aprendizado**, sem quiz nem escolha entre opções | Cada conteúdo é uma regra do mundo que o jogador sente agindo (§5). Não há pergunta de múltipla escolha em lugar nenhum. |
| "Um jogo é uma série de **escolhas interessantes**" | Toda decisão troca algo por algo: atirar agora ou escanear antes, abater ou marcar para o macrófago, salvar a célula ou conter o vírus (§7). |
| A estética **comunica a mecânica** | Contraste dos agentes sobre o vermelho, silhueta neutra antes da leitura, resistência visível e sinais vitais do paciente no HUD (§9). |
| A narrativa pode ser **premissa** | Paciente, Nanobô e infecção, com cards curtos na abertura, entre as fases e no fim (§4). |
| Tecnicamente desafiador | Aliados com IA de alvo, divisão binária com teto, máquina de estados viral, fonte TTF e UI modular em `src/ui/`. |
| Requisitos técnicos da Aula 1 | Menu, game over, 2 fases, teclado e mouse, spritesheet animado, música e efeitos, hi-score, até 100 MB e instalador. Nenhum é removido. |

## 3. Metas, não-metas e anti-metas

**Metas**

1. O aluno sai sabendo **explicar**, com as próprias palavras, os sete objetivos da §5.
2. Uma partida da Fase 1 de alguém que nunca jogou dura de **3 a 5 minutos** e pode ser **perdida**.
3. Quem lê o scanner vence mais do que quem chuta. A meta de calibração herdada de `ensino-e-aprendizagem.md` continua: um humano simulado vence de 55% a 75%; o chute, no máximo 10%.
4. O jogo tem identidade visual própria, coerente com o mapa.

**Não-metas**

- Simular fisiologia real: doses, tempos reais de divisão, farmacologia.
- Terceira fase.
- Diálogos ramificados ou personagens falando.

**Anti-metas** (fracasso mesmo que funcione)

- O aluno aprende que "antibiótico mata vírus" ou que toda bactéria é ruim.
- O conteúdo vira texto para ler: se o card de briefing é a única fonte de um OA, o OA falhou.
- O jogo fica mais difícil por **reflexo** e não por **decisão**: velocidade alta sem escolha não conta.

## 4. Narrativa

Premissa curta, contada em cards ilustrados com no máximo 3 linhas cada.

**Abertura** (`STORY_INTRO`, 4 cards, pulável com ESC):

1. *Hospital, 23h.* Um paciente de 16 anos chega com febre alta. Os exames mostram uma infecção que se espalha pelo sangue.
2. A equipe decide testar o **Nanobô**: um nanorrobô médico, menor que uma célula, que lê os invasores de perto.
3. O Nanobô não substitui o corpo. Ele **ajuda o sistema imune**: identifica, marca e trata, sem ferir as células do paciente.
4. *Missão:* entrar na corrente sanguínea, reconhecer cada invasor e manter os sinais vitais do paciente estáveis.

**Interlúdio Fase 1 → Fase 2** (`STORY_INTERLUDE`, 1 card): os sinais vitais aparecem como resultado do desempenho (§7.3). Exemplo: "a febre baixou. Mas o organismo enfraquecido abriu caminho para outro invasor: um vírus". O texto muda conforme os vitais: ≥ 70 "estável", 40–69 "em observação" e < 40 "grave".

**Final** (`VICTORY`): "o paciente recebeu alta", seguido do resumo do que o Nanobô viu: os OAs em linguagem de relatório médico.

**Game over** (`GAME_OVER`): "os sinais vitais caíram demais. A equipe retirou o Nanobô para tentar outro tratamento." Não é morte do paciente; o tom é de fracasso da missão.

**Calls minhas:** o paciente não tem nome nem gênero no texto, e os cards ficam em terceira pessoa, com narrador neutro.

## 5. Objetivos de aprendizagem como mecânicas

Cada OA tem uma **regra do mundo**, o **momento** em que o jogador a sente e o **feedback** que nomeia o conceito.

| OA | Conceito (ensino médio) | Regra no jogo | Quando o jogador aprende |
|---|---|---|---|
| **OA1** | Célula procarionte × eucarionte | O scan da célula do paciente mostra **núcleo e mitocôndrias**; o da bactéria mostra **nucleoide e ribossomos, sem núcleo**. Atirar na célula custa vitais. | Na primeira leitura de cada um; e ao errar o tiro. |
| **OA2** | Morfologia e coloração de Gram | A 1ª camada do scan dá **só a forma** (coco ou bacilo). A 2ª dá a **coloração** (roxo ou rosa) e a **estrutura** (parede espessa ou parede fina com membrana externa). As espécies da fase misturam formas e Gram, então a forma **sozinha** não decide a arma: para acertar, é preciso concluir o scan. | Quando um coco Gram− ou um bacilo Gram+ contraria o palpite pela forma. |
| **OA3** | O antibiótico age numa estrutura | **Antibiótico de parede** (inibe a parede de peptidoglicano) funciona na Gram+; na Gram− a membrana externa barra sua entrada. **Antibiótico de membrana** age na membrana externa, que só a Gram− tem. | Ao escolher a arma pela 2ª camada (estrutura); o erro dá a explicação. |
| **OA4** | Resistência bacteriana e seleção | Antibiótico errado numa bactéria **não a mata e a seleciona como resistente**: ela fica marcada, **ignora antibióticos** e só o sistema imune a remove. Toda bactéria viva se **divide por divisão binária** a cada `DIVISION_TIME`, e a filha herda a resistência. | No primeiro erro de arma; quando a população dobra. |
| **OA5** | Sistema imune inato: fagocitose e opsonização | **Macrófagos aliados** patrulham. Concluir as 2 camadas do scan **marca automaticamente** o alvo (opsonização) e o macrófago livre mais próximo vai até ele e o fagocita. O jogador ainda pode abater o alvo com antibiótico antes de o macrófago chegar. É o único jeito de remover uma bactéria resistente. | Na primeira bactéria resistente; o card do macrófago aparece aí. |
| **OA6** | Vírus é acelular | Na Fase 2, **antibiótico não afeta o vírus**: o projétil atravessa e o feedback diz que vírus não tem parede, metabolismo próprio nem ribossomos. | No primeiro tiro de antibiótico num vírus. |
| **OA7** | Ciclos lisogênico/lítico e imunidade adaptativa | O vírus **lisogênico** fica dentro da célula e pisca (já no código). Escanear a **célula infectada** a marca, e o **linfócito T** aliado a destrói antes da eclosão. O vírus **lítico** livre é neutralizado com **anticorpos**, a 3ª ferramenta, que só existe na Fase 2. | Ao decidir entre conter a célula agora ou caçar vírus livres depois. |

**O que o jogo não afirma** (nota no fim, nos "Para ir além" dos resultados):

- não ensina posologia, nem que um antibiótico real seja "de parede" puro (é uma regra do jogo inspirada em β-lactâmicos e polimixinas);
- simplifica a resistência: ela surge por mutação e é **selecionada** pelo uso inadequado;
- os ciclos lisogênico e lítico são descritos em bacteriófagos; o jogo usa o modelo como analogia didática.

## 6. Fases

### Fase 1 — "Corrente sanguínea" (bactérias)

- **Composição:** 6 bactérias (2 cocos Gram+, 1 bacilo Gram+, 1 coco Gram−, 2 bacilos Gram−), 3 células do paciente e 2 macrófagos.
- **Espécies sugeridas:** *S. aureus* (coco +), *S. pyogenes* (coco +), *Bacillus cereus* (bacilo +), *Neisseria meningitidis* (coco −), *E. coli* (bacilo −), *P. aeruginosa* (bacilo −).
- **Tutorial no mundo:** a primeira bactéria nasce isolada perto do Nanobô, e uma dica curta no próprio mundo indica o scan pelo botão direito. A dica some depois do primeiro scan e reaparece só se o jogador atirar sem ler.
- **Vitória:** população bacteriana zerada e o Nanobô no destino ("foco da infecção").
- **Derrota:** vitais em 0.

### Fase 2 — "Tecido infectado" (vírus)

- **Composição:** 3 vírus lisogênicos, cada um ligado à sua célula (já no código, #89), mais 2 células livres, 1 macrófago e 1 linfócito T.
- **Regras:** as de `fase2-virus.md` (12 s de lisogênico, eclipse, eclosão de 3, célula hospedeira rompe e custa vitais), com estes acréscimos:
  - **eclosão (decisão do dono, 13/11, substitui a de 11/11):** os até 3 vírus novos nascem **líticos e livres**, andando, e são alvo do anticorpo. Se um vírus livre tocar uma célula livre do paciente, ele a **infecta** e vira lisogênico dentro dela, com o tempo cheio. Sem célula livre, ele continua livre. Proteger as células passa a fazer parte do jogo;
  - o anticorpo **não atinge** o vírus lisogênico, porque ele está dentro da célula. A única saída é marcar a célula infectada pelo scan para o linfócito T;
  - na Fase 2, o macrófago fagocita vírus livres marcados (scan completo);
  - **teto de 12 vírus** vivos, que resolve a [PENDÊNCIA] de `MAX_AGENTS`;
  - o antibiótico não tem efeito (OA6);
  - o vírus livre exige anticorpo;
  - a célula infectada marcada é destruída pelo linfócito T, e isso conta como salvar o tecido, não como ferir o paciente.
- **Conflito D2.1:** resolvido por (a), ou seja, a célula hospedeira não conta no 6/6 de identificação.
- **Vitória:** zero vírus vivos e o Nanobô no destino.
- **Mapa:** o mesmo, com filtro de cor mais escuro e arroxeado, desenhado com `al_draw_tinted_bitmap`. É call minha.

## 7. Dificuldade, pressão e escolhas

### 7.1 Números de partida (constantes em `entities.h`/`gameplay.h`, a calibrar)

| Constante | Valor | O que decide |
|---|---|---|
| `VITALS_MAX` | 100 | barra do paciente no lugar dos 3 corações |
| `VITALS_CONTACT` | −8 | contato com bactéria ou vírus (respeita a invulnerabilidade de 1 s) |
| `VITALS_HIT_CELL` | −15 | tiro na célula do paciente |
| `VITALS_DRAIN_PER_BACTERIUM` | −0,12/s | cada bactéria viva drena; com 6, cerca de 0,7/s |
| `VITALS_HOST_RUPTURE` | −10 | eclosão rompe a célula (Fase 2) |
| `DIVISION_TIME` | 22 s ± 4 s | divisão binária por bactéria |
| `BACTERIA_CAP` | 16 | teto populacional da Fase 1 |
| `MAX_AGENTS` | 24 | capacidade do array (bactérias, células e aliados) |
| `MACROPHAGE_SPEED` | 45 px/s | velocidade do macrófago (Nanobô a 200) |
| `PHAGOCYTOSIS_TIME` | 1,2 s | o macrófago para enquanto fagocita |
| `T_CELL_SPEED` | 55 px/s | velocidade do linfócito T |
| `AGENT_SPEED` | 30 → 38 | bactérias um pouco mais rápidas |
| `VIRUS_CAP` | 12 | teto viral |

### 7.2 Escolhas que o jogador faz

- **Escanear ou atirar já.** O scan leva 2 cliques e o jogador fica exposto; atirar sem ler arrisca a resistência.
- **Abater ou esperar o macrófago.** O scan completo marca o alvo, e o macrófago começa a ir até ele. O jogador escolhe entre abater já com o antibiótico certo (+10, rápido, sem drenar mais vitais) e deixar o macrófago chegar (+15, lento, mas sem risco de criar resistência). Contra a resistente, só o macrófago funciona.
- **Onde gastar o tempo.** Cada bactéria viva drena vitais e se divide: é preciso priorizar a que está prestes a se dividir. Um anel de progresso aparece em volta dela depois da 1ª camada.
- **Fase 2: célula agora ou vírus depois.** Marcar a célula infectada para o linfócito T impede a eclosão de 3, mas o linfócito T é um só e lento.

### 7.3 Pontuação

- +10 por bactéria eliminada com antibiótico.
- +15 por alvo marcado e eliminado por aliado.
- −5 por resistência criada.
- +15 de bônus de identificação completa (mantido).
- **Bônus de vitais no fim da fase:** vitais × 0,5. É o que liga a narrativa ao placar.

## 8. Clareza e onboarding

- **Contraste:** todo agente tem contorno escuro de 1 px e um brilho interno próprio, sem vermelho; o mapa é todo vermelho.
- **Mapa recuado (decisão do dono, 13/11):** o mapa é desenhado escurecido (cerca de 70% de valor) e um pouco dessaturado, com `al_draw_tinted_bitmap`; o PNG não muda. Isso resolve o mid-on-mid e as hemácias e leucócitos pintados que hoje competem com os agentes (auditoria de 13/11, §7). Todo agente ganha também uma sombra de contato.
- **Resposta às ações (game feel, auditoria §5):** piscar durante a invulnerabilidade, flash e tremor curto ao levar dano, partículas com "+pontos" e hit-stop curto ao neutralizar, flash de 1 quadro no acerto parcial e um anel de scan saindo do Nanobô. Três níveis de intensidade, um ponto de chamada por evento (`SfxEvent`).
- **Antes da leitura:** a silhueta neutra deixa de ser o disco cinza e vira um disco **azul-acinzentado com contorno**, legível sobre o vermelho, sem revelar forma nem Gram.
- **Depois da 1ª camada:** o sprite real, que mostra a forma, num tom neutro.
- **Depois da 2ª camada:** o anel de coloração (roxo ou rosa) e o alvo de marcação.
- **Resistente:** uma auréola âmbar pulsando, para ser lida de longe.
- **Marcado:** um alvo ciano girando, para o jogador ver que o macrófago está indo até ele.
- **Briefing:** a abertura narrativa (§4) substitui os 3 cards de Gram. O conteúdo de Gram passa a aparecer quando o jogador precisa dele: no tutorial do mundo e nos cards curtos que surgem na 1ª ocorrência de cada conceito ("primeira bactéria resistente", "primeiro vírus"). Esses cards pausam o jogo e duram uma tela.

## 9. Arte e interface

### 9.1 Arte (pixel art à mão no Aseprite)

- **Paleta:** extraída de `assets/backgrounds/mapa_fase1.png` para os tons de vermelho, que ficam só no cenário. Os agentes usam uma paleta complementar: ciano e azul (Nanobô, aliados, UI), roxo e magenta (Gram+), rosa e salmão claro (Gram−), verde-água (células do paciente) e âmbar (resistência, alertas).
- **Escala:** sprites de 32×32 com 1 px de contorno escuro (`#121a24`, já usado). Iluminação vindo de cima à esquerda, como no mapa.
- **Lista de assets:**

| Asset | Tamanho | Quadros |
|---|---|---|
| Nanobô | 256×128 | 4 direções × 8 (contrato atual) |
| Coco Gram+, coco Gram−, bacilo Gram+, bacilo Gram− | 32×32 | 4 de pulso cada |
| Célula do paciente (núcleo visível) | 32×32 | 2 |
| Vírus | 32×32 | 2 |
| Célula infectada | 32×32 | 2 |
| Macrófago | 32×32 | 4 (pseudópodes) |
| Linfócito T | 32×32 | 2 |
| Silhueta neutra | 32×32 | 1 |
| Ícones (vitais, scanner, 3 armas, aliado) | 16×16 | 1 |
| Moldura 9-slice | 24×24 | 1 |
| Ilustrações dos cards | 160×90 | 1 por card (6) |

- **Variantes (#211):** 2 variantes por espécie de bactéria, para manter a contagem igual por Gram.

### 9.2 Interface

- **Fonte:** pixel TTF com licença OFL, com acentos do português, em `assets/fonts/`. A escolha da fonte é minha, e a licença vai para `assets/catalogo-recursos.md`.
- **Código:** as telas saem de `main.c` para `src/ui/ui.c` + `ui.h`, destino aprovado no `AGENTS.md`. O `main.c` fica com o loop, a entrada e os recursos.
- **HUD (painel inferior, 104 px):**
  - linha 1: barra de **vitais do paciente** com cor por faixa, contagem de bactérias ou vírus vivos, tempo e pontos;
  - linha 2: **ferramenta ativa** (ícone e nome do alvo: "parede", "membrana", "anticorpo") e recarga;
  - linhas 3–5: **ficha do scanner**, com forma, coloração e estrutura num formato fixo de ficha, como um exame, e não uma frase solta.
- **Telas novas:** `STORY_INTRO` (substitui `BRIEFING`), `STORY_INTERLUDE` (entre `RESULTS` e a Fase 2) e os cards de conceito (sobrepostos, pausando).
- **Máquina de estados:** as transições novas entram em `game.c` e em `docs/planejamento/fluxo-estados.md`.
- **Textos:** todos reescritos e registrados em `textos-do-jogo.md`, com ID e número de caracteres.

## 10. Abordagens consideradas

- **A. Fatia vertical (escolhida pelo dono).** Este documento primeiro; depois as mecânicas da Fase 1 com a arte atual, testadas pelo harness e por partidas; depois arte e interface novas; depois a Fase 2; e por fim calibração e QA. Prova cedo o que tem mais risco (ensino e dificuldade) e desenha a arte para o que já se sabe.
- **B. Visual primeiro.** Descartada: redesenharia o HUD antes de saber o que ele mostra.
- **C. A com o Eric na interface.** Descartada pelo dono: o Eric segue no Top 10, no instalador e na documentação.

## 11. Cronograma

| Etapa | Datas | Entrega |
|---|---|---|
| 0 | 13–14/11 | este documento aprovado |
| 1 | 15–19/11 | Fase 1 nova jogável com a arte atual: forma, antibiótico pelo alvo, resistência, divisão, macrófago, vitais e tutorial no mundo |
| 2 | 20–23/11 | Fase 2 completa: eclipse, eclosão, anticorpo, linfócito T, scanner viral |
| 3 | 24–28/11 | arte e interface novas, narrativa e textos |
| 4 | 29/11–01/12 | calibração, QA, memória (#117/#118) e docs |

**Corte, se a Etapa 1 passar de 19/11:** a Etapa 2 fica sem linfócito T (célula infectada só pode ser preservada; o vírus livre só cai com anticorpo). O dono decide nesse ponto.

## 12. Calls minhas, para o dono conferir

1. O paciente não tem nome nem gênero no texto.
2. A Fase 2 reaproveita o mapa com filtro de cor.
3. Os anticorpos só existem na Fase 2.
4. Teto de 12 vírus e de 16 bactérias.
5. D2.1 resolvido por (a): a célula hospedeira não conta no 6/6.
6. A perseguição por arma errada (RF-06) é substituída pela resistência.
7. Os 3 corações viram uma barra de vitais de 0 a 100. O ícone de coração continua como ícone da barra.
8. A pontuação ganha bônus de vitais.
9. As espécies da Fase 1 mudam para misturar formas e Gram (§6).
10. **Os vitais passam para a Fase 2.** No interlúdio, o paciente recupera até no mínimo 60 (quem terminou acima disso não ganha nada). O bônus de vitais é pago no fim de cada fase.
11. **A identificação não é condição de vitória.** O +15 de identificação completa conta só os agentes iniciais da fase. As filhas da divisão herdam as camadas lidas, a marca e a resistência da mãe.
12. **Variantes por categoria:** 2 variantes para cada uma das 4 categorias de bactéria (coco ou bacilo × Gram), num total de 8 sprites. A contagem é igual por Gram.
13. **Três cards de conceito:** "primeira bactéria resistente", "macrófago" e "antibiótico não age em vírus". São ilustrados com o próprio sprite ampliado, não com uma ilustração nova.
14. **Placeholders:** até a Etapa 3, as espécies novas (macrófago, linfócito T, célula infectada) usam sprites atuais com tinta. O contrato de arquivos (nome, tamanho e quadros) é a tabela da §9.1.

## 14. Decisões do dono na revisão (13/11)

- **Camadas do scan:** a 1ª camada dá só a forma, e a 2ª dá coloração e estrutura. Substitui a decisão de 03/11, em que a camada 1 trazia a coloração.
- **Marcação:** automática ao concluir o scan.
- **Eclosão:** os vírus novos nascem líticos e livres e infectam células livres ao tocá-las. Substitui o "nascem lisogênicos" de `fase2-virus.md` (11/11).

## 13. Issues

- O trabalho novo vira issues no M5 (GitHub) e tarefas no Todoist quando cada etapa começar. Nada de comentários em issues.
- Fechar no GitHub e no Todoist quando terminar: #85–#89 e #91 (Fase 2), #211 (variantes), #117/#118 (memória).
- Candidatas a "not planned", **só com a decisão do dono**: RF-06 (perseguição), #206/#207 (D2.1/D2.2 do modelo antigo), #208 (bônus de run perfeito, substituído pelo bônus de vitais), #209 (pulso de scan de 0,75 s) e #210 (RF-07, substituído pelo relatório final).
