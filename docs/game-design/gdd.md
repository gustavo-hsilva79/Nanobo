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

## Protagonista

O jogador controla o **Nanobô**, um nanorrobô responsável por explorar o ambiente microscópico, analisar agentes e agir de acordo com as informações coletadas.

## Scanner

O scanner é uma mecânica central de investigação.

- O alvo válido considerado pelo scanner é o organismo válido mais próximo.
- A leitura ocorre em pulsos/camadas.
- Cada agente deve possuir identificação única para evitar que a mesma identificação seja contabilizada repetidamente.
- As informações obtidas devem apoiar a interpretação do agente e a decisão do jogador.
- A camada 2 traz a pista do envelope celular e o jogador escolhe o laser correspondente; o scanner não recomenda ação.

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

O combate está implementado no M3.5. Os números atuais são ponto de partida, não balanceamento final:

| Regra | Valor atual | Onde |
|---|---|---|
| Vida do Nanobô | 3 corações; 1 de dano por contato com bactéria ou por acertar a célula; 1 s de invulnerabilidade | `entities.h`, `gameplay.c` |
| Ataque | laser na direção do `facing`, 1 disparo por segundo, até 4 projéteis na tela | `gameplay.c` |
| Laser | roxo para Gram+, vermelho para Gram−; laser errado gasta o projétil e não tira resistência | `gameplay.c` |
| Resistência | bactéria 2 acertos, célula 0 (não é destruída) | `entities.h` |
| Defesa | 2 s de imunidade, 3 s de recarga; empurra bactéria a 2× a velocidade por 0,5 s | `gameplay.c` |
| Consequência | resistência zerada neutraliza o agente, que sai da cena | `gameplay.c` |
| Célula atacada | tiro na célula tira 1 de vida do Nanobô e não remove a célula; ela continua identificável | `gameplay.c` |

O balanceamento de dano, cooldowns e resistência continua **[PENDÊNCIA]**; a regra fechada está em `docs/game-design/decisoes.md`.

## Pontuação e progressão

A pontuação deve considerar as decisões do jogador e pode incluir bônus por identificação. A progressão deve refletir a conclusão dos objetivos da fase e as consequências das ações.

O hi-score deve ser persistente na versão final.

## Interface e controles

O jogo usa teclado e mouse. Os controles implementados são:

- setas ou `WASD`: mover o Nanobô;
- `ESPAÇO`: scanner;
- `J`: atacar; `1`/`2`: laser roxo (Gram+) / vermelho (Gram−);
- `X`: defesa;
- `ESC`: pausa; `ENTER`: confirmar/iniciar;
- `F11`: tela cheia, sempre com escala inteira;
- clique no botão `INICIAR` do menu.

O HUD ocupa o rodapé de 104 px (identificados, tempo, vida, laser, controles e a leitura do alvo atual). A apresentação final das informações e a pontuação ainda serão refinadas (#19, #48).

## Estados e fluxo

O fluxo está implementado em `src/core/game.c` com seis estados: `MENU`, `GAME`, `PAUSE`, `RESULTS`, `VICTORY` e `GAME OVER`. As transições permitidas e a regra de que só `GAME` atualiza a lógica estão em [Fluxo de estados](../planejamento/fluxo-estados.md).

`GAME OVER` já é alcançado por vida zerada; `RESULTS` e `VICTORY` ainda dependem das teclas de teste `F` e `V` até a #20/#26.

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
- Cooldown definitivo do scanner.
- Calibração dos números já implementados de vida, dano, ataque, defesa e resistência.
- Regras de progressão entre fases.
- Apresentação final das informações do scanner.
- Regras detalhadas da multiplicação limitada da Fase 2.
