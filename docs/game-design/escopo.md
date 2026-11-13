# Nanobô — Escopo

> **Nanobô 2.0 (13/11/2026) — escopo vigente.** O jogo final tem **3 fases**, cada uma com mapa próprio:
> - Fase 1: bactérias na corrente sanguínea;
> - Fase 2: vírus nas vias aéreas;
> - Fase 3: fungos no pulmão.
>
> O Nanobô é o único defensor: aliados (macrófago, linfócito T) estão **fora do escopo**. Entram também narrativa, 8 objetivos de ensino como mecânicas, barra de vitais, arte e interface novas, Top 10, build Release e instalador. "Uma terceira fase só se houver tempo", abaixo, foi superado pela decisão do dono: a Fase 3 está no escopo e nada cede. A ordem de corte, caso o prazo aperte, está no [`kanban.md`](../planejamento/kanban.md) (§"Nanobô 2.0"). A visão completa está no [`gdd.md`](gdd.md).

## Objetivo do escopo

Construir um jogo educacional 2D em C com Allegro 5 no qual o conteúdo biológico influencia decisões do jogador durante o gameplay.

## MVP

O MVP deve entregar uma partida completa e jogável centrada em **uma fase**, contendo o núcleo de:

- exploração;
- scanner;
- interpretação e identificação;
- combate e defesa;
- vida e dano;
- pontuação e consequências;
- condições de vitória e derrota.

> **Estado em 05/11/2026:** exploração, scanner, identificação, combate, defesa, vida, célula própria, pontuação, conclusão da fase e o fluxo de telas (Resultados → Vitória → reinício) estão em `master` desde 04/11; o MVP está fechado. A Fase 2 mínima entrou no mesmo build para fechar o fluxo entre fases.

A Fase 1 do MVP trabalha bactérias e células saudáveis, incluindo a diferenciação entre bactérias Gram-positivas e Gram-negativas.

## Entrega final

A entrega final deve expandir o jogo para **mais de uma fase** e incluir os requisitos técnicos e de apresentação previstos no planejamento.

A Fase 2 mínima (vírus) entrou no build em 04/11; a fase completa, com ciclos lítico/lisogênico e multiplicação limitada, é M5.

## Pós-MVP

A Fase 2 completa é tratada como objetivo pós-MVP e integra a entrega final. Uma terceira fase somente deve ser considerada caso exista tempo e estabilidade suficientes.

## Requisitos técnicos e de produto

- Menu inicial.
- Game Over.
- Múltiplas fases: Fase 1 e Fase 2 mínima no MVP; a completa é M5.
- Entrada por teclado e mouse.
- Spritesheet e animação.
- Música e efeitos sonoros.
- Hi-score persistente.
- Máximo de 100 MB de RAM.
- Instalador para Windows.

## Priorização

Quando houver pressão de prazo, preservar primeiro as mecânicas essenciais para aprendizagem e funcionamento do jogo: exploração, scanner, identificação/decisão, combate, vida, vitória/derrota e estabilidade.

Polimento visual, música, hi-score e conteúdo adicional entram depois da estabilidade do núcleo, respeitando os marcos de entrega.

## Fora do escopo por enquanto

Novos sistemas ou features que não contribuam diretamente para aprendizagem, gameplay principal ou requisitos da disciplina devem ser adiados até que o núcleo esteja estável.

## Controle de escopo

Antes de adicionar uma nova feature, verificar:

1. ela é necessária para o objetivo do jogo?
2. ela contribui diretamente para a aprendizagem?
3. ela atende a um requisito da disciplina?
4. qual é o impacto no cronograma, nas Issues e nos milestones?

Se a resposta for negativa para os três primeiros pontos, a feature deve permanecer fora do escopo atual.
