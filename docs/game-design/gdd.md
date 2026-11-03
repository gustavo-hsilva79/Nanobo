# Nanobô — GDD

## Visão geral

Nanobô é um jogo educacional 2D desenvolvido em C com Allegro 5. O jogador controla um nanorrobô em um ambiente microscópico inspirado no interior do corpo humano.

## High Concept / Mantra

**Explorar, analisar, interpretar e decidir.**

## Loop de gameplay

O gameplay central segue o ciclo:

**Explorar → Escanear → Identificar → Decidir → Agir → Consequência**

O jogador explora o ambiente, utiliza o scanner para analisar organismos, interpreta as informações apresentadas e decide como agir. A consequência da decisão influencia a pontuação, a sobrevivência e a progressão.

## Aprendizagem

A aprendizagem deve acontecer por meio da interpretação das informações e das consequências das decisões, e não somente por perguntas de múltipla escolha.

O conteúdo educacional deve ter função dentro do gameplay: a informação obtida pelo scanner precisa ajudar o jogador a distinguir agentes e escolher uma ação adequada.

A Fase 1 trabalha quatro objetivos, detalhados em [`ensino-e-aprendizagem.md`](ensino-e-aprendizagem.md):

| OA | O jogador será capaz de… |
|---|---|
| OA1 | interpretar o que o resultado roxo ou rosa da coloração de Gram indica |
| OA2 | comparar as camadas externas: Gram+ tem parede mais espessa; Gram− tem parede mais fina e uma camada externa extra |
| OA3 | relacionar a estrutura observada ao alvo do módulo dentro das regras do jogo |
| OA4 | reconhecer e preservar uma célula saudável do organismo, distinguindo a regra do jogo de uma afirmação sobre medicamentos reais |

O caminho do ensino é **briefing → scan → decisão → feedback → resumo**, e cada etapa retoma os mesmos quatro objetivos. A tela de Resultados apresenta o resumo por objetivo, não só o total.

## Protagonista

O jogador controla o **Nanobô**, um nanorrobô responsável por explorar o ambiente microscópico, analisar agentes e agir de acordo com as informações coletadas.

## Briefing

Antes da Fase 1, três cartões puláveis apresentam o necessário para decidir: (1) o que a coloração de Gram revela e o limite da cor; (2) a comparação das camadas externas Gram+ e Gram−, com o peptidoglicano como parte da parede; (3) os módulos como modelo do jogo e a regra de preservar a célula própria. ENTER, → ou clique avançam; ESC pula. Não há tutorial guiado depois, e o Diário de Campo — que reabriria o conteúdo no pause — ficou fora do M4 (#103, #106).

## Scanner

O scanner é uma mecânica central de investigação.

- A ativação é o **botão direito** sobre o agente sob o cursor (tolerância de 24 px, alcance de 130 px); o teclado não escaneia no M4.
- A leitura ocorre em **dois pulsos**: o pulso 1 mostra o resultado da coloração (roxo ou rosa) e o pulso 2 mostra o nome do grupo, a pista estrutural e o porquê.
- Durante os pulsos o Nanobô fica parado e sem atirar; dano de contato cancela o pulso.
- Cada agente possui identificação única, para evitar que a mesma identificação seja contabilizada repetidamente.
- As informações obtidas apoiam a interpretação do agente e a decisão do jogador; o scanner não recomenda módulo.

## Agentes e conteúdo

### Fase 1 — bactérias e células saudáveis

A primeira fase é o núcleo do MVP. Ela trabalha a diferenciação entre bactérias e células saudáveis, incluindo a distinção entre bactérias Gram-positivas e Gram-negativas.

O jogador deve usar as informações do scanner para interpretar o agente encontrado e escolher a ação correspondente.

### Fase 1 — de onde os agentes podem nascer

Os agentes nascem em pontos fixos do mundo, sorteados a cada partida sem repetir ponto (implementados em `src/gameplay/gameplay.c`, na tabela `valid_points`, com **12** coordenadas conferidas por `_Static_assert`). Um ponto válido respeita três regras:

1. fica dentro do mundo de **1920×512** (3 telas de 640 px por 2 faixas de 256 px), com margem das bordas;
2. fica dentro da altura do mundo e não mais da faixa do painel: a câmera mostra 256 px por vez, então o painel do scanner deixou de limitar o nascimento — a tabela usa `y = 96`/`140` na metade de cima e `y = 380`/`420` na de baixo;
3. fica a pelo menos **100 px** do ponto de nascimento do Nanobô `(960, 256)` — em 12/10/2026 um ponto a 22 px fazia um agente nascer já encostado (e, com a vida da #11, isso seria dano de graça no primeiro frame).

### Fase 2 — vírus

A segunda fase é objetivo pós-MVP e integra a entrega final planejada. O conteúdo previsto envolve os ciclos lítico e lisogênico e uma mecânica de multiplicação limitada.

Foi definido um limite de referência de **3 vírus** para uma mecânica de desafio. As regras exatas dessa mecânica ainda precisam ser detalhadas e testadas.

## Combate e consequências

O combate está implementado no M3.5; os módulos A/B entram no M4 (#105) e até lá o build ainda usa o raio roxo/vermelho por espécie. Os números atuais são ponto de partida, não balanceamento final:

| Regra | Valor atual | Onde |
|---|---|---|
| Vida do Nanobô | 3 corações; 1 de dano por contato com bactéria ou por acertar a célula; 1 s de invulnerabilidade | `entities.h`, `gameplay.c` |
| Ataque | módulo na direção do cursor, 1 disparo por segundo, até 4 projéteis na tela | `gameplay.c`, `main.c` |
| Módulos | A neutraliza Gram+ (alvo na parede); B neutraliza Gram− (alvo na camada externa); módulo errado gasta o projétil e não tira resistência | `gameplay.c` |
| Resistência | bactéria 2 acertos, célula 0 (não é destruída) | `entities.h` |
| Defesa | 2 s de imunidade, 3 s de recarga; empurra bactéria a 2× a velocidade por 0,5 s | `gameplay.c` |
| Consequência | resistência zerada neutraliza o agente, que sai da cena | `gameplay.c` |
| Célula atacada | tiro na célula tira 1 de vida do Nanobô, **com qualquer módulo** (D1), e não remove a célula; ela continua identificável | `gameplay.c` |

O balanceamento de dano, cooldowns e resistência continua **[PENDÊNCIA]**; a regra fechada está em `docs/game-design/decisoes.md`.

## Pontuação e progressão

A pontuação atual da Fase 1:

| Evento | Pontos | Regra |
|---|---:|---|
| Ameaça neutralizada | **+10** | uma vez por ameaça, só no acerto que zera a resistência (#66) |
| Identificação completa | **+15** | uma vez, na conclusão, se `identificados = 9/9` (#67) |
| Placar máximo da fase | **75** | 6 ameaças + bônus |

`9/9` **era** condição apenas de bônus: a fase concluía com menos identificações (#26, #67). Em 03/11/2026 a dupla decidiu o contrário (D2.1, `decisoes.md`): a identificação dos agentes passa a ser **condição de conclusão**. A regra em vigor no código ainda é a de #26/#67 — a mudança depende de resolver o destino de quem atira antes de escanear (D2.2). O hi-score deve ser persistente na versão final.

## Interface e controles

O jogo usa teclado e mouse, com o mouse como forma de mirar, atirar e escanear:

- setas ou `WASD`: mover o Nanobô;
- mover o cursor: mirar; **botão esquerdo**: atirar na direção do cursor;
- **botão direito** sobre o agente: escanear;
- `1`/`2` ou roda do mouse: trocar o módulo A/B; clique no ícone do módulo no HUD também troca;
- `X`: defesa;
- `ESC`: pausa; `ENTER`: confirmar/iniciar;
- `F11`: tela cheia, escala inteira quando o encaixe der 1x ou mais; janela menor que 640×360 encolhe proporcionalmente;
- clique no botão `INICIAR` do menu.

O ataque e o scanner **não têm tecla** no M4 (#198).

> **Estado em 03/11/2026, após o merge do PR #203:** os controles por cursor (#198) e os módulos A/B (#105) **estão implementados** e são o que o jogo faz hoje. Do M4 ainda faltam o briefing (#103, #106), o pulso 2 nomeando o grupo (#104) e a identificação como condição de conclusão (D2.1).

O HUD ocupa o rodapé de 104 px: identificados x/9, ameaças restantes x/6, pontos, tempo, três corações, módulo selecionado com estado (`[pronto]`/`[recarga]`) e a leitura do alvo atual. O objetivo da fase aparece na segunda linha do painel; os controles ficam no menu e na pausa, para não competir com a leitura do jogo. A marca do destino aparece na borda da faixa visível quando ele está fora da câmera (#19).

## Estados e fluxo

O fluxo está implementado em `src/core/game.c` com seis estados — `MENU`, `GAME`, `PAUSE`, `RESULTS`, `VICTORY` e `GAME OVER` — e o M4 acrescenta `BRIEFING` entre o menu e a fase (#106). As transições permitidas e a regra de que só `GAME` atualiza a lógica estão em [Fluxo de estados](../planejamento/fluxo-estados.md).

`GAME OVER` é alcançado por vida zerada; `RESULTS` é alcançado automaticamente quando a fase conclui (6 ameaças neutralizadas + Nanobô no destino); `VICTORY` vem de `RESULTS`; `VICTORY` e `GAME OVER` iniciam nova partida com reset completo. `F`, `G` e `H` são teclas de QA e existem somente no build Debug (#70, #76).

### Resultados e vitória

A tela de Resultados apresenta os números reais da partida: pontos, ameaças neutralizadas/total, agentes identificados/total e se o bônus de +15 foi pago. No M4 ela ganha o **resumo por objetivo** (#174): scans por grupo, decisões certas e erradas e ataques a células, a partir de contadores em memória — sem CSV nem telemetria. Um total geral não substitui a leitura de cada objetivo. A tela de Vitória mostra a pontuação final e inicia nova partida.

## Áudio e arte

O projeto prevê:

- spritesheet e animação;
- música;
- efeitos sonoros;
- cenários e demais recursos visuais.

Os assets são organizados em `assets/`, separados por tipo.

## Escopo de desenvolvimento

O MVP permanece centrado em uma fase para garantir uma partida completa e jogável antes da expansão do conteúdo. A segunda fase fica como objetivo pós-MVP e integra a entrega final planejada.

Uma terceira fase somente deve ser considerada se houver tempo e estabilidade suficientes.

## Pendências de design

- Conteúdo biológico detalhado e validado de cada agente.
- Duração do pulso do scanner e intervalo entre leituras.
- Calibração dos números já implementados de vida, dano, ataque, defesa e resistência.
- Regras de progressão entre fases.
- Apresentação final das informações do scanner (#199).
- Regras detalhadas da multiplicação limitada da Fase 2.

Fora do M4: Diário de Campo, zonas guiadas, registro de eventos/telemetria, arte neutra e calibração de trava/perseguição; ver [`ensino-e-aprendizagem.md`](ensino-e-aprendizagem.md).
