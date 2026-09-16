# Nanobô — One Sheet

> Documento de visão rápida do projeto. Regras detalhadas e decisões devem ser mantidas no GDD, escopo e decisões de Game Design.

## High Concept

Jogo educacional 2D em C com Allegro 5 no qual o jogador controla o Nanobô em um ambiente microscópico inspirado no interior do corpo humano, explora, analisa organismos pelo scanner, interpreta as informações e toma decisões com consequências no gameplay.

## Mantra

**Explorar → Escanear → Identificar → Decidir → Agir → Consequência**

## Objetivo educacional

Transformar a identificação e interpretação de informações biológicas em decisões dentro do gameplay, evitando que o conteúdo seja apresentado apenas como um quiz.

## Núcleo de gameplay

Exploração → análise com scanner → interpretação das informações → identificação/decisão → ação → consequências em sobrevivência, pontuação e progressão.

## Fase 1 — núcleo do MVP

A primeira fase trabalha bactérias e células saudáveis, incluindo a diferenciação entre bactérias Gram-positivas e Gram-negativas. O jogador deve usar as informações do scanner para orientar suas decisões.

## Fase 2 — objetivo pós-MVP

A segunda fase integra a entrega final planejada e aborda vírus, ciclos lítico e lisogênico e multiplicação limitada. O limite de referência definido para uma mecânica de desafio é de 3 vírus; as regras exatas ainda são pendentes.

## Scanner

- Seleciona o alvo válido mais próximo.
- Opera em pulsos/camadas.
- Cada agente possui identificação única.
- A informação coletada deve influenciar a decisão do jogador.

## Requisitos principais

- Menu inicial e Game Over.
- Múltiplas fases na entrega final.
- Entrada por teclado e mouse.
- Animação por spritesheet.
- Música e efeitos sonoros.
- Pontuação e hi-score persistente.
- Limite máximo de 100 MB de RAM.
- Instalador para Windows.

## Escopo de desenvolvimento

O **MVP permanece centrado em uma fase** para validar uma partida completa antes da expansão do conteúdo. A segunda fase fica como objetivo pós-MVP. Uma terceira somente entra se houver tempo e estabilidade suficientes.

## Equipe

- Gustavo Herick Silva
- Eric Brito Sampaio de Melo
