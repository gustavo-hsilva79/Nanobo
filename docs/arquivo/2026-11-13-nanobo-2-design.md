# Nanobô 2.0 — design de ensino, narrativa, dificuldade, arte e interface

> **Arquivado em 13/11/2026. Histórico, não vigente.** Este é o primeiro rascunho do design 2.0, antes da arguição da tarde de 13/11. Depois dele, o dono retirou o macrófago e o linfócito T, criou a Fase 3 (fungos), deu mapas próprios a cada fase e decidiu as ferramentas Reserva, Lise e Perfuração. O conteúdo vigente está nos documentos vivos, cada um na sua §"Nanobô 2.0": [`gdd.md`](../game-design/gdd.md), [`ensino-e-aprendizagem.md`](../game-design/ensino-e-aprendizagem.md), [`conteudo-biologico.md`](../game-design/conteudo-biologico.md), [`decisoes.md`](../game-design/decisoes.md), [`fase2-virus.md`](../game-design/fase2-virus.md), [`scanner.md`](../game-design/scanner.md), [`textos-do-jogo.md`](../game-design/textos-do-jogo.md), [`escopo.md`](../game-design/escopo.md), [`catalogo-recursos.md`](../../assets/catalogo-recursos.md) e [`kanban.md`](../planejamento/kanban.md).

> **Criado em:** 13/11/2026, a partir da rodada de brainstorming com o dono (Gustavo).
> **Substitui**, no que conflitar: `docs/game-design/gdd.md` (núcleo e fases), os objetivos OA1–OA4 de `ensino-e-aprendizagem.md` e a seção de arte de `decisoes.md`. A regra viral de `fase2-virus.md` continua valendo, com os acréscimos da §6.
> **Prazo:** entrega final em 02/12/2026 (Aula 17). O M5 interno vence em 29/11.
> **Equipe (atualizado em 13/11, depois da auditoria):** o redesenho é do Gustavo, que também assumiu os requisitos obrigatórios da disciplina que estavam com o Eric: hi-score Top 10 (#77–#82, #197), build Release (#42, #154–#158) e instalador (#43, #159–#164). O Eric fica com QA e regressão, documentação final, registro de textos (#212) e volta ao menu (#195).
> **Auditoria relacionada:** `docs/auditoria/auditoria-2026-11-13.md`.

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
3. O Nanobô não substitui o corpo. Ele **ajuda o sistema imune**: identifica, marca e trata, sem ferir as células do paciente. A maioria das bactérias do corpo é inofensiva; ele vai atrás só das que causam a infecção.
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
| **OA3** | O antibiótico age numa estrutura | A **Ruptura** (antibiótico de parede, inspirado na penicilina) impede a bactéria de montar a parede de peptidoglicano e funciona na Gram+; na Gram−, a membrana externa barra sua entrada. A **Dissolução** (antibiótico de membrana, inspirado na polimixina) desfaz a membrana externa, que só a Gram− tem. | Ao escolher a arma pela 2ª camada (estrutura); o erro dá a explicação. |
| **OA4** | Resistência bacteriana por **tratamento incompleto** | A bactéria precisa de **2 doses do antibiótico certo**. Se levar 1 dose e continuar viva por mais de `INCOMPLETE_TIME` (cerca de 6 s, mostrado por um anel que se esvazia), ela se torna **resistente àquele antibiótico**: as doses seguintes não fazem efeito e só o macrófago a remove. O **antibiótico errado não faz efeito nenhum**: gasta o tiro e o tempo, e o feedback explica por quê (OA3), mas não cria resistência. Toda bactéria viva se **divide por divisão binária** a cada `DIVISION_TIME`; a filha herda a resistência, mas não a dose recebida. | Quando deixa uma bactéria "meio tratada" escapar. A mensagem é "termine o tratamento" (decisão do dono, 13/11, que substitui "erro de arma gera resistência a tudo", porque isso ensinaria multirresistência por um erro só). |
| **OA5** | Sistema imune inato: fagocitose e opsonização | **Macrófagos aliados** patrulham. Concluir as 2 camadas do scan **marca automaticamente** o alvo (opsonização) e o macrófago livre mais próximo vai até ele e o fagocita. O jogador ainda pode abater o alvo com antibiótico antes de o macrófago chegar. É o único jeito de remover uma bactéria resistente. | Na primeira bactéria resistente; o card do macrófago aparece aí. |
| **OA6** | Vírus é acelular | Na Fase 2, **antibiótico não afeta o vírus**: o projétil atravessa e o feedback diz que vírus não tem parede, metabolismo próprio nem ribossomos. | No primeiro tiro de antibiótico num vírus. |
| **OA7** | Ciclos lisogênico/lítico e imunidade adaptativa | O vírus **lisogênico** fica dentro da célula e pisca (já no código). Escanear a **célula infectada** a marca, e o **linfócito T** aliado a destrói antes da eclosão. O vírus **lítico** livre é neutralizado pela **Neutralização** (anticorpos), a 3ª ferramenta, que só existe na Fase 2. | Ao decidir entre conter a célula agora ou caçar vírus livres depois. |

**O que o jogo não afirma** (nota no fim, nos "Para ir além" dos resultados):

- não ensina posologia, nem que um antibiótico real seja "de parede" puro (é uma regra do jogo inspirada em β-lactâmicos e polimixinas);
- simplifica a resistência: ela surge por mutação e é **selecionada** pelo uso inadequado;
- os ciclos lisogênico e lítico são descritos em bacteriófagos; o jogo usa o modelo como analogia didática;
- não afirma que toda bactéria é ruim: a maioria das que vivem no corpo é inofensiva ou útil (microbiota); as do jogo são as que causam a infecção do paciente.

### 5.1 Como cada conteúdo é explicado

> **Verificado em 13/11** a pedido do dono. Antes desta seção, o design dizia **que regra** ensina cada conteúdo, mas não **como** o aluno descobre o significado dela. Só três conteúdos tinham um momento de explicação; o porquê da cor do Gram, a diferença entre procarionte e eucarionte, o linfócito T, os anticorpos e os ciclos virais não tinham texto nenhum; e "peptidoglicano", "nucleoide" e "opsonização" eram usados sem ser definidos.

**Critério de clareza.** Cada conteúdo precisa de três coisas:

1. **Definição** em linguagem de ensino médio, no máximo **na primeira vez** em que o termo aparece na tela. Todo termo técnico vem com uma explicação curta na mesma frase.
2. **Feedback** quando a ação do jogador contraria a regra, dizendo o porquê biológico e não só "errou".
3. **Retomada** no relatório de resultados, com o que o jogador fez naquele conteúdo.

**Os cinco cards de conceito.** Cada um aparece uma única vez, na primeira ocorrência do evento, pausa o jogo, tem 3 linhas de no máximo 70 caracteres e é ilustrado com o sprite ampliado. Todos são rascunhos, a contar e registrar em `textos-do-jogo.md`.

| Card | Quando aparece | Texto |
|---|---|---|
| **Coloração de Gram** | 1ª leitura completa (2ª camada) de uma bactéria | No laboratório, um corante roxo tinge a parede das bactérias. / Gram+: parede grossa de peptidoglicano, que segura o roxo. / Gram-: parede fina e membrana externa; perde o roxo e fica rosa. |
| **Resistência** | 1ª bactéria que fica resistente | Uma dose só não basta: as bactérias que sobrevivem se multiplicam. / Esta ficou resistente: o mesmo antibiótico não age mais nela. / Termine o tratamento. Contra a resistente, conte com o macrófago. |
| **Macrófago** | 1ª fagocitose de um alvo marcado | O macrófago é uma célula de defesa que engole invasores. / Isso se chama fagocitose. O scan marca o alvo para ele. / No corpo, quem marca são os anticorpos: é a opsonização. |
| **Vírus não é célula** | 1ª leitura de um vírus ou 1º antibiótico num vírus, o que vier antes | O vírus não tem parede, ribossomos nem metabolismo próprio. / Ele só se multiplica dentro de uma célula. Antibiótico não o atinge. / Neutralização: anticorpos que se prendem ao vírus livre. |
| **Lisogênico e lítico** | 1ª leitura de uma célula infectada | Lisogênico: o vírus fica dentro da célula, sem destruí-la ainda. / Lítico: ele faz cópias, a célula se rompe e libera novos vírus. / O linfócito T destrói a célula infectada antes que ela se rompa. |

**O que cada conteúdo usa:**

| OA | Definição (onde e texto) | Feedback do erro | Retomada nos resultados |
|---|---|---|---|
| OA1 procarionte × eucarionte | Ficha do scanner. Célula do paciente: "célula do paciente · eucarionte · núcleo guarda o DNA". Bactéria, na 2ª camada: "procarionte · sem núcleo: DNA solto no nucleoide" | Tiro na célula: "você feriu uma célula do paciente: ela tem núcleo, não é bactéria" | "Você poupou X de Y células do paciente" |
| OA2 forma e Gram | Ficha: "forma: coco (esférica)" ou "forma: bacilo (bastão)" na 1ª camada; o card **Coloração de Gram** na 2ª | Errar a arma pela forma: "a forma não define o grupo: veja a coloração" | "Você leu X bactérias até a coloração" |
| OA3 antibiótico × estrutura | Ficha na 2ª camada: "Gram+ · parede grossa, sem membrana externa" ou "Gram- · parede fina sob membrana externa". No menu e no HUD: "Ruptura · parede" e "Dissolução · membrana". O scanner **não recomenda a arma**, regra mantida desde o M4 | Ruptura na Gram−: "a membrana externa barrou a Ruptura". Dissolução na Gram+: "a Gram+ não tem membrana externa para a Dissolução" | "X doses certas, Y no grupo errado" |
| OA4 resistência | Na 1ª dose: "1ª dose: complete o tratamento" e o anel de 6 s; depois, o card **Resistência** | Quando a resistência surge: "tratamento incompleto: esta bactéria ficou resistente à Ruptura" (ou à Dissolução) | "X tratamentos completos, Y bactérias resistentes" |
| OA5 imunidade inata | Card **Macrófago** | Tiro inútil numa resistente: "resistente: só o macrófago remove esta bactéria" | "O macrófago removeu X invasores marcados" |
| OA6 vírus acelular | Card **Vírus não é célula** | Antibiótico num vírus: "antibiótico não age em vírus: use a Neutralização" | "Você usou a Neutralização em X vírus" |
| OA7 ciclos e imunidade adaptativa | Card **Lisogênico e lítico**; ficha: "vírus · lisogênico · 9 s" e "célula infectada · marcada para o linfócito T" | Eclosão: "a célula se rompeu: 3 vírus livres (ciclo lítico)" | "X células infectadas contidas antes de romper" |

**Ferramentas**, no menu "como jogar" e no HUD:
- **Ruptura** · antibiótico de parede: impede a bactéria de montar a parede.
- **Dissolução** · antibiótico de membrana: desfaz a membrana externa das Gram-.
- **Neutralização** · anticorpos: prendem o vírus livre e impedem que ele entre em outra célula (só na Fase 2).

**Anti-meta "toda bactéria é ruim":** o card 3 da abertura (§4) ganha uma frase, e o relatório final ganha um "Para ir além" com a microbiota: "a maioria das bactérias do corpo é inofensiva ou útil; estas causam a infecção".

**Teste de clareza no QA da Etapa 4.** Para cada OA, uma partida automatizada confirma que o card aparece, que o feedback do erro dispara com o texto certo e que a retomada no resultado bate com o que aconteceu. Na leitura, o dono confere se cada texto se entende sem conhecer o documento.

## 6. Fases

### Fase 1 — "Corrente sanguínea" (bactérias)

- **Composição:** 6 bactérias (2 cocos Gram+, 1 bacilo Gram+, 1 coco Gram−, 2 bacilos Gram−), 3 células do paciente e 2 macrófagos.
- **Espécies sugeridas:** *S. aureus* (coco +), *S. pyogenes* (coco +), *Bacillus cereus* (bacilo +), *Neisseria meningitidis* (coco −), *E. coli* (bacilo −), *P. aeruginosa* (bacilo −).
- **Tutorial no mundo:** a primeira bactéria nasce isolada perto do Nanobô, e uma dica curta no próprio mundo indica o scan pelo botão direito. A dica some depois do primeiro scan e reaparece só se o jogador atirar sem ler.
- **Primeiro quadro com vida:** hoje os 9 agentes nascem fora da câmera, e a tela inicial mostra só o Nanobô (auditoria §4). Na Fase 1 nova, a bactéria do tutorial e um macrófago aparecem no primeiro quadro; o resto continua sorteado nos pontos válidos, longe do Nanobô.
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
- **Conflito D2.1:** deixou de existir, porque a identificação não é mais condição de vitória (§12 item 11). A célula hospedeira rompida só sai da conta do bônus de identificação.
- **Vitória:** zero vírus vivos e o Nanobô no destino.
- **Mapa:** o mesmo, com filtro de cor mais escuro e arroxeado, desenhado com `al_draw_tinted_bitmap`. É call minha.
- **Partida e destino diferentes da Fase 1:** hoje as duas fases começam no mesmo ponto, com o mesmo destino (auditoria §4). Na Fase 2, o Nanobô entra pela esquerda e o destino fica no alto à direita, para a fase não parecer repetida. É call minha.

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
| `INCOMPLETE_TIME` | 6 s | janela para a 2ª dose antes da resistência |
| `BACTERIA_CAP` | 16 | teto populacional da Fase 1 |
| `MAX_AGENTS` | 24 | capacidade do array (bactérias, células e aliados) |
| `MACROPHAGE_SPEED` | 45 px/s | velocidade do macrófago (Nanobô a 200) |
| `PHAGOCYTOSIS_TIME` | 1,2 s | o macrófago para enquanto fagocita |
| `T_CELL_SPEED` | 55 px/s | velocidade do linfócito T |
| `AGENT_SPEED` | 30 → 38 | bactérias um pouco mais rápidas |
| `VIRUS_CAP` | 12 | teto viral |

### 7.2 Escolhas que o jogador faz

- **Escanear ou atirar já.** O scan leva 2 cliques e o jogador fica exposto. Atirar sem ler arrisca gastar tiros com o antibiótico errado enquanto as bactérias se dividem.
- **Terminar o tratamento.** Depois da 1ª dose, o jogador tem cerca de 6 s para dar a 2ª, mesmo que outra ameaça apareça. Deixar a bactéria meio tratada cria uma resistente.
- **Abater ou esperar o macrófago.** O scan completo marca o alvo, e o macrófago começa a ir até ele. O jogador escolhe entre abater já com o antibiótico certo (+10, rápido, sem drenar mais vitais) e deixar o macrófago chegar (+15, lento, mas sem risco de criar resistência). Contra a resistente, só o macrófago funciona.
- **Onde gastar o tempo.** Cada bactéria viva drena vitais e se divide: é preciso priorizar a que está prestes a se dividir. Um anel de progresso aparece em volta dela depois da 1ª camada.
- **Fase 2: célula agora ou vírus depois.** Marcar a célula infectada para o linfócito T impede a eclosão de 3, mas o linfócito T é um só e lento.

### 7.3 Pontuação

- +10 por bactéria eliminada com antibiótico.
- +15 por alvo marcado e eliminado por aliado.
- −5 por resistência criada (tratamento incompleto).
- +15 de bônus de identificação completa (mantido).
- **Bônus de vitais no fim da fase:** vitais × 0,5. É o que liga a narrativa ao placar.
- **Top 10 (requisito da disciplina, #34/#77–#82/#197):** a pontuação final da run entra no ranking só em vitória (decisão de 05/11, mantida), com nome de até 10 caracteres digitado na tela de vitória. O ranking aparece no menu.

## 8. Clareza e onboarding

- **Contraste:** todo agente tem contorno escuro de 1 px e um brilho interno próprio, sem vermelho; o mapa é todo vermelho.
- **Mapa recuado (decisão do dono, 13/11):** o mapa é desenhado escurecido (cerca de 70% de valor) e um pouco dessaturado, com `al_draw_tinted_bitmap`; o PNG não muda. Isso resolve o mid-on-mid e as hemácias e leucócitos pintados que hoje competem com os agentes (auditoria de 13/11, §7). Todo agente ganha também uma sombra de contato.
- **Resposta às ações (game feel, auditoria §5):** piscar durante a invulnerabilidade, flash e tremor curto ao levar dano, partículas com "+pontos" e hit-stop curto ao neutralizar, flash de 1 quadro no acerto parcial e um anel de scan saindo do Nanobô. Três níveis de intensidade, um ponto de chamada por evento (`SfxEvent`).
- **Antes da leitura:** a silhueta neutra deixa de ser o disco cinza e vira um disco **azul-acinzentado com contorno**, legível sobre o vermelho, sem revelar forma nem Gram.
- **Depois da 1ª camada:** o sprite real da **forma** (coco ou bacilo), num tom neutro. O sprite é o mesmo para Gram+ e Gram−, para não entregar o grupo antes da hora.
- **Depois da 2ª camada:** o mesmo sprite tingido pela coloração (roxo ou rosa), com `al_draw_tinted_bitmap`, o anel de coloração e o alvo de marcação. Biologicamente, a cor vem do corante do laboratório e não da bactéria, então tingir o sprite neutro é a representação certa.
- **Resistente:** uma auréola âmbar pulsando, para ser lida de longe.
- **Marcado:** um alvo ciano girando, para o jogador ver que o macrófago está indo até ele.
- **Briefing:** a abertura narrativa (§4) substitui os 3 cards de Gram. O conteúdo de Gram passa a aparecer quando o jogador precisa dele: no tutorial do mundo e nos três cards de conceito (§12 item 13), que surgem na 1ª ocorrência de cada um e pausam o jogo por uma tela.

## 9. Arte e interface

### 9.1 Arte (pixel art à mão no Aseprite)

- **Paleta:** extraída de `assets/backgrounds/mapa_fase1.png` para os tons de vermelho, que ficam só no cenário. Os agentes usam uma paleta complementar: ciano e azul (Nanobô, aliados, UI), roxo e magenta (Gram+), rosa e salmão claro (Gram−), verde-água (células do paciente) e âmbar (resistência, alertas).
- **Escala:** sprites de 32×32 com 1 px de contorno escuro (`#121a24`, já usado). Iluminação vindo de cima à esquerda, como no mapa.
- **Lista de assets:**

| Asset | Tamanho | Quadros |
|---|---|---|
| Nanobô | 256×128 | 4 direções × 8 (contrato atual) |
| Coco e bacilo, neutros (o Gram vem do tingimento, §8), 2 variantes cada | 32×32 | 4 de pulso cada |
| Célula do paciente (núcleo visível) | 32×32 | 2 |
| Vírus | 32×32 | 2 |
| Célula infectada | 32×32 | 2 |
| Macrófago | 32×32 | 4 (pseudópodes) |
| Linfócito T | 32×32 | 2 |
| Silhueta neutra | 32×32 | 1 |
| Ícones (vitais, scanner, 3 armas, aliado) | 16×16 | 1 |
| Moldura 9-slice | 24×24 | 1 |
| Ilustrações dos cards | 160×90 | 1 por card (6) |

- **Variantes (#211):** 2 variantes por forma (coco e bacilo), 4 sprites no total. Como o sprite não muda com o Gram, a contagem fica igual por grupo sem esforço extra. Os sprites `_b` de 12/11 saem quando a arte nova entrar.

### 9.2 Interface

- **Fonte:** pixel TTF com licença OFL, com acentos do português, em `assets/fonts/`. A escolha da fonte é minha, e a licença vai para `assets/catalogo-recursos.md`. Antes de adotar, testo maiúsculas acentuadas (`Ç`, `Ã`, `Ô`). Se renderizarem bem, a regra do `AGENTS.md` de evitar maiúscula acentuada deixa de valer, e os botões podem voltar a ter acento ("AVANÇAR").
- **Código:** as telas saem de `main.c` para `src/ui/ui.c` + `ui.h`, destino aprovado no `AGENTS.md`. O `main.c` fica com o loop, a entrada e os recursos.
- **HUD (painel inferior, menor que os 104 px atuais, que ocupam 29% da tela; auditoria §6):**
  - linha 1: barra de **vitais do paciente** com cor por faixa, contagem de bactérias ou vírus vivos, tempo e pontos;
  - linha 2: **ferramenta ativa** (ícone, nome e alvo: "Ruptura · parede", "Dissolução · membrana", "Neutralização · vírus livre") e recarga;
  - **ficha do scanner** só quando há alvo sob o cursor ou lido há pouco, com forma, coloração e estrutura num formato fixo, como um exame. Sem alvo, a ficha recolhe e o espaço volta para o mundo, em vez de mostrar "Nenhum alvo" a partida inteira.
- **Telas novas:**
  - `STORY_INTRO` (substitui `BRIEFING`);
  - `STORY_INTERLUDE` (entre `RESULTS` e a Fase 2);
  - os cards de conceito (sobrepostos, pausando);
  - **entrada de nome** na vitória e **Top 10** acessível pelo menu (#81, #197).
- **Navegação:** o foco segue a geometria. Botões lado a lado andam com ←/→ e empilhados com ↑/↓, porque hoje ←/→ não fazem nada no briefing (auditoria §6). A pausa, o game over e a vitória ganham "voltar ao menu" (#195, do Eric), e essa transição entra na máquina de estados.
- **Resultados em linguagem de jogador:** o resumo deixa os códigos internos ("OA1 Gram:") e fala como relatório do paciente ("você identificou 4 de 6 bactérias pelo Gram"). Os códigos OA ficam só nos documentos.
- **Máquina de estados:** as transições novas entram em `game.c` e em `docs/planejamento/fluxo-estados.md`.
- **Textos:** todos reescritos e registrados em `textos-do-jogo.md`, com ID e número de caracteres.

## 10. Abordagens consideradas

- **A. Fatia vertical (escolhida pelo dono).** Este documento primeiro; depois as mecânicas da Fase 1 com a arte atual, testadas pelo harness e por partidas; depois arte e interface novas; depois a Fase 2; e por fim calibração e QA. Prova cedo o que tem mais risco (ensino e dificuldade) e desenha a arte para o que já se sabe.
- **B. Visual primeiro.** Descartada: redesenharia o HUD antes de saber o que ele mostra.
- **C. A com o Eric na interface.** Descartada pelo dono. Depois da auditoria de 13/11, o Eric ficou com QA, documentação, textos e a volta ao menu.

## 11. Cronograma

| Etapa | Datas | Entrega |
|---|---|---|
| 0 | 13–14/11 | este documento aprovado; issues da Etapa 1 criadas; Todoist remarcado |
| 1 | 15–19/11 | Fase 1 nova jogável com a arte atual: forma, antibiótico pelo alvo, resistência, divisão, macrófago, vitais, tutorial no mundo, **mapa recuado** e os **4 primeiros itens de game feel** (§8). Em paralelo, **amostra de arte** (Nanobô, coco e macrófago) para aprovação até 18/11 |
| 1b | 20–21/11 | **instalador de fumaça:** Release empacotado e instalado numa pasta limpa, mesmo com o jogo incompleto, para descobrir cedo o que falta (DLLs do Allegro, caminhos dos assets, arquivo de hi-score gravável) (#42, #154–#158, #159–#161) |
| 2 | 20–23/11 | Fase 2 completa: eclipse, eclosão, anticorpo, linfócito T, scanner viral |
| 3 | 24–28/11 | arte e interface novas, narrativa, textos, **Top 10 com entrada de nome** (#77–#82, #197) |
| 4 | 29/11–01/12 | calibração, QA, memória (#117/#118), **instalador final** testado em máquina limpa (#43, #162–#164) e docs |

**Prioridade de corte**, do que nunca sai para o que sai primeiro:

1. **Requisitos obrigatórios da Aula 1**, porque reprovam se faltarem: Top 10 salvo, instalador, até 100 MB, menu e game over, 2 fases, spritesheet animado, áudio.
2. **Ensino da Fase 1** (Etapa 1): é o centro da nota de "mecânicas que contribuem para o aprendizado".
3. **Fase 2 nova:** se a Etapa 1 passar de 19/11, ela fica sem linfócito T (a célula infectada só pode ser preservada, e o vírus livre só cai com anticorpo).
4. **Arte nova:** se faltar tempo, primeiro o Nanobô e os agentes da Fase 1; ilustrações dos cards por último.
5. **Interface nova:** se faltar tempo, HUD e menu primeiro; resultados e pausa ficam com o layout atual e os textos novos.

O dono decide em cada ponto de corte.

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
12. **Variantes por forma:** 2 variantes de coco e 2 de bacilo, num total de 4 sprites neutros. O Gram vem do tingimento depois da 2ª camada (§8), então a contagem é igual por Gram automaticamente. Revisado em 13/11; a primeira versão previa 8 sprites, um por categoria, e entregava o Gram na 1ª camada.
13. **Cinco cards de conceito** (§5.1): "Coloração de Gram", "Resistência", "Macrófago", "Vírus não é célula" e "Lisogênico e lítico". Ilustrados com o próprio sprite ampliado, não com uma ilustração nova. Revisado em 13/11: eram três, e deixavam a coloração e os ciclos virais sem explicação.
15. **Nome da 3ª ferramenta:** **Neutralização**. O dono decidiu manter Ruptura e Dissolução, e a 3ª segue o mesmo estilo. "Anticorpo neutralizante" é o termo real da imunologia.
14. **Placeholders:** até a Etapa 3, as espécies novas (macrófago, linfócito T, célula infectada) usam sprites atuais com tinta. O contrato de arquivos (nome, tamanho e quadros) é a tabela da §9.1.

## 13. Issues

- O trabalho novo vira issues no M5 (GitHub) e tarefas no Todoist quando cada etapa começar. Nada de comentários em issues.
- Fechar no GitHub e no Todoist quando terminar: #85–#89 e #91 (Fase 2), #211 (variantes), #117/#118 (memória), e os requisitos obrigatórios que o Gustavo assumiu (#42, #43, #77–#82, #154–#164, #197).
- Candidatas a "not planned": #206/#207 (D2.1/D2.2 do modelo antigo), #208 (bônus de run perfeito, substituído pelo bônus de vitais) e #209 (pulso de scan de 0,75 s). Também as duplicadas do M6: #43/#44/#46/#47 repetem #159–#170. **Decisão do dono em 13/11: nada é fechado por enquanto.** A #210 (RF-07) já está fechada desde 09/11.
- A substituição da perseguição (RF-06) pela resistência não tem issue própria; entra numa das issues novas da Etapa 1.

## 14. Decisões do dono na revisão (13/11)

- **Camadas do scan:** a 1ª camada dá só a forma, e a 2ª dá coloração e estrutura. Substitui a decisão de 03/11, em que a camada 1 trazia a coloração.
- **Marcação:** automática ao concluir o scan.
- **Eclosão:** os vírus novos nascem líticos e livres e infectam células livres ao tocá-las. Substitui o "nascem lisogênicos" de `fase2-virus.md` (11/11).
- **Mapa:** pode ser escurecido e dessaturado na hora de desenhar; o PNG não muda.
- **Equipe:** o Gustavo assume Top 10, build Release e instalador.

### Rodada de arguição (13/11)

- **Resistência:** surge por **tratamento incompleto** e vale só para o antibiótico usado. O antibiótico errado não faz efeito, mas não cria resistência (§5, OA4).
- **Arte:** uma **amostra cedo**. Até 18/11, as peças-âncora (Nanobô de frente, 1 coco e 1 macrófago) são desenhadas sobre o mapa escurecido e passam pela aprovação do dono antes do resto. Se o estilo não for aprovado, ainda há tempo de mudar de abordagem.
- **Playtest:** só o dono e partidas simuladas. Não haverá teste com aluno do ensino médio antes de 02/12, então a calibração da §3 (meta 3) é feita por bots no harness e pelas partidas do dono. A #38 e as #134–#138 (teste externo) ficam sem cumprir; o destino delas segue a regra de "não fechar nada por enquanto".
- **Escopo:** o dono manteve tudo, com a ordem de corte da §11 como rede de segurança.
- **Checagem diária:** no fim de cada dia de trabalho, um resumo curto para o dono, sem o código cru:
  - o que mudou e qual issue fechou (no GitHub e no Todoist);
  - capturas automáticas das telas afetadas;
  - harness e build Debug/Release com 0 warnings;
  - o que vem no dia seguinte;
  - as decisões que só o dono pode tomar, cada uma com uma recomendação.
- **Merge:** direto, como até agora: branch local por etapa e merge fast-forward na `master` quando a etapa fecha, sem PR.
- **Dificuldade-alvo:** quem joga a Fase 1 pela primeira vez e lê o scanner perde cerca de 1 em 3 partidas e vence quase sempre na segunda tentativa. A calibração usa dois bots no harness:
  - **leitor:** completa o scan, escolhe a arma pela 2ª camada, termina o tratamento e reage com atraso humano de cerca de 0,4 s; precisa vencer 55–75%;
  - **chute:** não escaneia e escolhe a arma ao acaso; precisa vencer no máximo 10%.
- **Modo de demonstração:** não haverá. A apresentação usa o jogo normal, com ensaio (#47, #167).
- **Call minha:** o macrófago prioriza a bactéria resistente marcada; empatando, vai na mais próxima.
- **Armas:** os nomes atuais ficam: **Ruptura** (antibiótico de parede) e **Dissolução** (antibiótico de membrana). A 3ª ferramenta, da Fase 2, se chama **Neutralização** (anticorpos), por call minha, no mesmo estilo.
- **Clareza do ensino:** a §5.1 define, para cada conteúdo, a definição, o feedback do erro e a retomada nos resultados, com os textos de rascunho dos cinco cards.
