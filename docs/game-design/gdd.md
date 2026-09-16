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
- A relação exata entre as informações do scanner, alcance/raio e apresentação visual ainda precisa ser validada nos testes.

## Agentes e conteúdo

### Fase 1 — bactérias e células saudáveis

A primeira fase é o núcleo do MVP. Ela trabalha a diferenciação entre bactérias e células saudáveis, incluindo a distinção entre bactérias Gram-positivas e Gram-negativas.

O jogador deve usar as informações do scanner para interpretar o agente encontrado e escolher a ação correspondente.

### Fase 2 — vírus

A segunda fase é objetivo pós-MVP e integra a entrega final planejada. O conteúdo previsto envolve os ciclos lítico e lisogênico e uma mecânica de multiplicação limitada.

Foi definido um limite de referência de **3 vírus** para uma mecânica de desafio. As regras exatas dessa mecânica ainda precisam ser detalhadas e testadas.

## Combate e consequências

O projeto prevê combate e defesa como parte da tomada de decisão. Vida, dano, ataque, defesa, cooldowns, resistência e condições exatas de consequência ainda devem ser balanceados durante a implementação e os testes.

## Pontuação e progressão

A pontuação deve considerar as decisões do jogador e pode incluir bônus por identificação. A progressão deve refletir a conclusão dos objetivos da fase e as consequências das ações.

O hi-score deve ser persistente na versão final.

## Interface e controles

O projeto utiliza entrada por **teclado e mouse**.

Os detalhes dos comandos, HUD e apresentação das informações serão refinados durante a implementação, respeitando o fluxo de gameplay e os requisitos da disciplina.

## Estados e fluxo

O jogo deverá organizar o fluxo por estados/cenas, contemplando no mínimo os estados necessários para:

- menu inicial;
- gameplay;
- pausa;
- vitória;
- Game Over;
- transição/reinício quando aplicável.

A estrutura concreta da máquina de estados será definida durante a implementação.

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

- Regras finais de identificação e recomendação.
- Conteúdo biológico detalhado e validado de cada agente.
- Relação final entre dados do scanner e escolha do raio/ação.
- Balanceamento de vida, dano, ataque, defesa, cooldowns e resistência.
- Regras de progressão entre fases.
- Apresentação final das informações do scanner.
- Regras detalhadas da multiplicação limitada da Fase 2.
