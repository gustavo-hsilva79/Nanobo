# Nanobô — vírus da Fase 2: estados, eclosão e multiplicação

> **Criado em:** 11/11/2026, fechando as pendências da #83. A issue pai é a #55; a implementação fica em #84 (estado lisogênico), #85 (transição), #86 (multiplicação), #87 (scanner) e #88 (combate e consequências). As decisões foram tomadas pelo dono na rodada de 11/11/2026 e estão registradas aqui com as consequências de cada escolha.
> Este arquivo é **regra**, não descrição do build: a Fase 2 mínima de 04/11 tem três vírus sem ciclo, que já andam sozinhos a `AGENT_SPEED` como qualquer agente ativo (`update_match`) e não eclodem (`decisoes.md`, "Dívida de implementação"). Onde o código divergir daqui, vale este documento.
> Cada item abaixo é decisão do dono em 11/11/2026, exceto o que estiver marcado `[PENDÊNCIA]` — mecanismo derivado pela dupla para tornar a decisão implementável, com dono e momento nomeados.

## Estados

O vírus tem dois estados, e os dois são legíveis **no mundo, sem scanner** — é o que permite ao jogador decidir se vale gastar a leitura antes de se aproximar.

| estado | visual | movimento | pode ser neutralizado | transição |
|---|---|---|---|---|
| **lisogênico** | sprite do vírus piscando: alterna entre opaco e semitransparente no ritmo de `LYSOGENIC_BLINK` | parado; o empurrão da defesa continua valendo, como na Fase 2 mínima | **sim** — qualquer arma, 1 acerto (`VIRUS_RESISTANCE`) | ao fim de `LYSOGENIC_TIME` (12 s) entra no ciclo lítico |
| **lítico** | sprite opaco, sem piscar | anda sozinho a `AGENT_SPEED`, em direção aleatória, como o vírus da Fase 2 mínima | **sim** — qualquer arma, 1 acerto | não volta a lisogênico; eclode uma única vez; sai da cena quando neutralizado |

- Todo vírus da fase **nasce lisogênico**; os três da Fase 2 começam o encontro com o temporizador cheio.
- O contato fere nos dois estados. O vírus não persegue o Nanobô, nem depois de lítico: a perseguição continua sendo a do alerta por arma errada (`decisoes.md`, 04/11).
- O estado **não é uma camada do scanner**. A camada 1 repete o estado e o tempo porque é ali que o jogador lê o número, mas a pista principal é o piscar no mundo.
- Vírus criado por eclosão nasce **lisogênico**, com o temporizador cheio (ver "Ciclo lítico"): é o que faz a população crescer por geração.

## Transição lisogênico → lítico

- A condição é **só o tempo**: quando o temporizador lisogênico daquele vírus chega a zero, ele entra no ciclo lítico. Não há gatilho por proximidade, por scan nem por ataque.
- O temporizador é `LYSOGENIC_TIME`, **12 s**, constante ajustável em `src/entities/entities.h` ao lado das outras de combate. **Valor a validar no PoC** — 12 s é o ponto de partida, não um número fechado: se o jogador não conseguir ler o estado a tempo, o valor sobe; se a fase ficar parada, desce.
- O temporizador é **por vírus**, não por fase: cada agente tem o seu, e um vírus criado por eclosão começa com o valor cheio.
- Ele vive em `update_match` e **congela na pausa**, como os outros timers; `reset_match` e `start_phase` devolvem todo vírus ao estado lisogênico com o tempo cheio.
- Neutralizar o vírus antes do fim do temporizador **cancela a eclosão**: não nasce vírus novo e nenhuma célula morre. É a decisão "certa" que o estado lisogênico coloca na frente do jogador.
- A transição é **irreversível**: não existe caminho lítico → lisogênico para o mesmo vírus.

## Ciclo lítico

O ciclo tem três momentos, nesta ordem: eclipse (`[PENDÊNCIA]`, abaixo), multiplicação e a vida lítica do vírus que eclodiu.

### Eclipse — [PENDÊNCIA] mecanismo derivado

- **O eclipse não foi decidido pelo dono.** A decisão de 11/11/2026 foi só esta: ao fim do lisogênico o vírus vira lítico, eclode e gera até 3 novos. O compasso entre o fim do temporizador e o nascimento dos novos é uma leitura da dupla para dar ao jogador uma última janela visível — vale como proposta, não como regra fechada, e precisa ser confirmado ou trocado por eclosão imediata **antes de #84**. Dono: dupla, antes de #84.
- O fim do temporizador **não cria os vírus no mesmo quadro**. Abre um compasso curto em que o vírus para de piscar e continua no lugar; a célula hospedeira **ainda não se rompe**.
- A duração é `ECLIPSE_TIME`, constante própria: **valor em aberto** — curta o bastante para não parecer travamento, longa o bastante para o jogador ver o que aconteceu.
- Durante o eclipse o vírus já é lítico e **continua neutralizável**. Se for neutralizado antes do fim do compasso, a multiplicação é cancelada **e a célula hospedeira é salva**: nenhum vírus novo nasce, a célula fica na cena e o 1 de vida não é cobrado.
- O eclipse é a última janela para evitar a multiplicação e a ruptura da célula; a primeira é o temporizador lisogênico inteiro.

### Multiplicação

- Ao fim do eclipse nascem **até 3 vírus novos**, em volta do ponto da ruptura — ou do próprio vírus, quando não há célula viva —, sem nascer em cima do Nanobô nem fora da área jogável. Raio, ângulo e ordem de criação são da implementação (#86).
- Os novos nascem **lisogênicos**, com `LYSOGENIC_TIME` cheio, e `generation` igual à geração do pai mais 1.
- O vírus que eclodiu **continua lítico**, permanece na cena e **não eclode de novo**: uma marca de "já eclodiu" (nome livre para a implementação) fecha o ciclo dele.
- **No mesmo instante da multiplicação**, a célula hospedeira **se rompe, sai da cena e cobra 1 de vida** do Nanobô (ver "Consequências"). Ruptura, nascimento dos novos e perda de vida são um evento só: neutralizar o vírus durante o eclipse impede os três.
- Sem célula hospedeira viva na fase, a eclosão **só multiplica**: não há célula a romper e não há custo de vida.

### `generation`

- Campo inteiro no agente, zerado em `start_phase`: os três vírus da Fase 2 são **geração 0**.
- Cada eclosão cria vírus de **geração pai + 1**. O campo serve ao balanceamento e a um eventual teto global; a regra atual não depende dele para parar nada (ver "Multiplicação limitada").

## Multiplicação limitada

**Regra, em uma frase:** cada eclosão gera **até 3 vírus**; cada vírus eclode **uma única vez na partida**; a população cresce **por geração**, não por eclosões repetidas do mesmo vírus.

O que a regra impede e o que ela não impede:

- impede o **mesmo** vírus de eclodir duas vezes — não existe laço no próprio agente;
- **não** impede a cadeia entre gerações: os filhos eclodem uma vez cada, e os netos também.

Exemplo numérico, com 3 vírus na cena e 3 novos por eclosão, sem nenhum neutralizado:

| momento | vírus vivos | conta |
|---|---:|---|
| início | 3 | geração 0 |
| geração 0 eclode | 12 | 3 + 3 × 3 |
| geração 1 eclode | 39 | 12 + 9 × 3 |
| geração 2 eclode | 120 | 39 + 27 × 3 |

**Teto global: descartado em 11/11/2026.** A recomendação da rodada era o teto de **6** vírus vivos com só a geração 0 eclodindo; o dono decidiu que **cada vírus eclode uma vez e não há teto**. A consequência fica registrada, não escondida: sem neutralização, a população cresce 3 → 12 → 39 → 120 e a fase pode ficar **invencível**, porque a conclusão exige zerar os vírus vivos antes de chegar ao destino. **[PENDÊNCIA] de balanceamento:** se o playtest do M5 confirmar o problema, o teto global volta como alavanca, com 6 como valor de referência — dono: dupla, no playtest do M5.

**Capacidade do mundo: [PENDÊNCIA] de implementação.** A regra acima não cabe no array de agentes: `MAX_AGENTS` é **9** (`src/entities/entities.h:10`) e a Fase 2 já ocupa **6** slots (3 vírus + 3 células), então a primeira eclosão levaria a cena a **15** agentes (12 vírus + 3 células). Duas saídas possíveis, ambas fora desta tarefa: (a) subir `MAX_AGENTS` para pelo menos 15 e revisar o custo de memória; ou (b) impor um teto populacional que corte a multiplicação quando não houver slot livre — o que, na prática, reintroduz o teto global descartado acima. Dono: dupla, antes de #86. Nada foi alterado em `src/` aqui.

A multiplicação é, portanto, **pressão de tempo e de prioridade**: ela não acrescenta um estado de derrota novo — a partida continua terminando só com a vida em zero —, mas cria uma **segunda fonte de dano**, a espera, e encarece cada geração com mais tiros e mais vida (pela célula hospedeira).

## Scanner

O vírus mantém **duas camadas**, como os outros agentes, e a leitura continua sendo uma camada por ativação (`scanner.md` §4). O que muda é o conteúdo: o estado entra na **camada 1**, para o jogador decidir com o que lê primeiro.

| camada | quando | texto desenhado | caracteres |
|---|---|---|---:|
| 1 | vírus lisogênico | `vírus · Vírus da fase · lisogênico · %d s` | 41 (com 2 dígitos) |
| 1 | vírus lítico | `vírus · Vírus da fase · lítico` | 30 |
| 2, linha 1 | vírus lisogênico | `lisogênico · %d s · espera na célula e eclode em até 3 novos` | 60 |
| 2, linha 1 | vírus lítico | `lítico · não eclode de novo` | 27 |
| 2, linha 2 (revisão) | qualquer estado | `cada vírus eclode uma vez; a célula hospedeira morre` | 52 |

- O `%d s` é o **tempo restante** do lisogênico e aparece **nas duas camadas**: na 1 para quem ainda não leu, na primeira linha da 2 para quem já concluiu a leitura. O painel desenha **uma camada por vez** (`src/main.c:909-925`), então o contador sumiria justo quando o jogador termina de ler se existisse só na 1. No lítico não existe número.
- A categoria do painel continua `vírus`, sem amostra de coloração e sem anel: não há Gram.
- A camada 2 desenha **duas linhas curtas**, não uma frase: a decisão de 11/11/2026 pedia "o ciclo em uma frase", e o painel só comporta duas linhas de até 73 caracteres. O ciclo foi partido entre elas — a primeira traz estado, tempo e mecanismo; a segunda fecha a consequência. A camada 2 **não recomenda arma**: qualquer arma neutraliza o vírus.
- Os textos substituem `RT-09` (camada 1), `RT-10` e `RT-11` (camada 2) de `textos-do-jogo.md` (Fase 2 mínima de 04/11). A atualização de lá entra no commit que desenhar as strings (#87); este documento é a fonte da regra.
- A maior linha tem **60 caracteres**: 60 × 8 = 480 px, dentro dos 588 px úteis do painel (73 caracteres). Até descontando os 12 px do swatch que o painel desenha hoje, a linha cabe nos 576 px restantes. Nenhuma das cinco passa do limite da fonte — medido por contagem de caracteres, que é a unidade da fonte embutida.

## Consequências

### Pontuação

- **+10 por vírus neutralizado**, inclusive os que nasceram de uma eclosão; cada vírus conta uma vez, como qualquer ameaça (#66).
- A célula hospedeira **não pontua**: ela não é alvo do jogador e não pode ser neutralizada por ataque.
- O bônus de identificação da fase continua o da Fase 2 mínima (6/6 paga +15) e é medido em agentes identificados, não em vírus neutralizados.

### Célula hospedeira

- **No fim do eclipse**, o mesmo instante da multiplicação, a eclosão **rompe a célula hospedeira**: ela sai da cena e o Nanobô perde **1 de vida**, respeitando a invulnerabilidade de 1 s (#11). Neutralizar o vírus durante o eclipse evita a multiplicação **e** essa perda.
- A célula **não é ameaça para a conclusão da fase**: a Fase 2 conclui zerando os vírus vivos e chegando ao destino, e a célula não entra nessa conta. Perder a célula não bloqueia a vitória.
- **Qual célula morre: [PENDÊNCIA] regra derivada, não decidida.** O vínculo vírus↔célula é da composição da fase (#89). Os três vírus da Fase 2 nascem ligados às três células; um vírus criado por eclosão não tem célula própria e rompe a **célula viva mais próxima** no instante da ruptura. Se a célula ligada a um vírus de geração 0 já saiu da cena — rompida antes por um filho —, vale a célula viva mais próxima; sem nenhuma célula viva, não há morte nem custo de vida. O vínculo **não é desenhado antes da eclosão** — a pista de qual célula corre risco é a proximidade. A rodada de 11/11/2026 decidiu o custo, não o vínculo: a dupla confirma ou troca esta regra antes de #86.
- Se a célula rompida ainda não foi escaneada, o 6/6 da fase fica impossível e o +15 se perde — a mesma consequência da ameaça neutralizada sem identificação (#67).

### Segunda condição de derrota

- Esta é a divergência registrada em 11/11/2026: até aqui o MVP só perdia vida por **ação** (contato, tiro na célula própria). Com a célula hospedeira, o jogador perde vida por **espera** — por deixar o ciclo correr.
- A derrota continua sendo uma só, a vida em zero; o que muda é a existência de uma **segunda fonte de dano**, que não depende de o jogador errar um alvo. É nesse sentido — segunda fonte que leva à mesma derrota — que "segunda condição de derrota" é usada aqui, e é isso que **substitui, na Fase 2**, a frase de 25/10 em `decisoes.md` ("não existe ... segunda condição de derrota no MVP"). A Fase 1 continua sem segunda fonte.
- A nota da substituição está em `decisoes.md`, seção "Consequência de não atacar" (nota de 11/11/2026).

### Conflito aberto com D2.1

Com D2.1 (identificação obrigatória: 6/6 na Fase 2, M5), uma célula rompida antes de ser escaneada deixa a fase **inconcluível**: o 6/6 vira impossível e a conclusão passa a exigir a identificação. O caso precisa de uma saída antes de #84–#88 entrarem, e ela é decisão do dono: (a) a célula hospedeira não conta no 6/6; (b) a eclosão só acontece depois de a célula ser identificada; ou (c) a célula rompida conta como identificada. **[PENDÊNCIA]**, dono: dupla, antes da implementação.

### O que o jogador aprende

- o estado do vírus muda a prioridade: neutralizar o lisogênico agora evita até três vírus depois;
- a multiplicação não se repete no mesmo agente, mas é cumulativa entre gerações — esperar tem custo crescente;
- a célula hospedeira é o recurso que o vírus consome, e o jogador paga por isso com vida.

### O que o jogo não afirma

- que os vírus do cenário causem doença, ou que todo vírus infecte qualquer célula;
- que "lisogênico" e "lítico" na tela reproduzam o ciclo real em detalhe: o jogo não simula integração do genoma, replicação do DNA viral nem resposta imune — o estado lisogênico é representado por um vírus parado e piscando, com temporizador;
- que neutralizar um vírus corresponda a um tratamento, a um antiviral ou a uma conduta clínica; a arma é regra do jogo;
- que a célula hospedeira morta signifique dano ao organismo fora da ficção da missão.

## O que fica para depois do PoC

Números e ritmos, todos com dono: a dupla, no playtest do M5. Nenhum deles muda as regras acima.

| item | valor de partida | o que o número decide |
|---|---|---|
| `LYSOGENIC_TIME` | 12 s | quanto tempo o jogador tem para ler e agir antes da eclosão |
| `LYSOGENIC_BLINK` | [PENDÊNCIA] | ritmo do piscar; precisa ler como "parado, mas ativo" |
| `ECLIPSE_TIME` | [PENDÊNCIA] | se o eclipse fica; se ficar, o compasso entre o fim do temporizador e o nascimento dos novos |
| novos por eclosão | até 3, fixo ou sorteado | se toda eclosão dá 3 ou varia entre 1 e 3 |
| teto global de vírus vivos | sem teto hoje; referência 6 | se o crescimento por geração torna a fase invencível |
| posição e raio de nascimento | [PENDÊNCIA] | se os novos nascem legíveis e longe do Nanobô |
