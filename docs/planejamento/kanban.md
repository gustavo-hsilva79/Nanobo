# Nanobô — Kanban

> Fluxo: **Backlog → To Do → In Dev → In Review → Done**. WIP recomendado: **2 tarefas simultâneas**.
>
> As datas de entrega acadêmicas abaixo vêm do planejamento da disciplina. As datas intermediárias são metas internas recomendadas para organizar o desenvolvimento.

## Milestones a criar no GitHub

| Milestone | Data | Tipo | Objetivo |
|---|---|---|---|
| **M1 — Conceito e Escopo do Jogo** | 09/09/2026 | Entrega acadêmica | Fechar conceito, conteúdo mínimo e fronteira do MVP |
| **M2 — Planejamento e Base Técnica** | 23/09/2026 | Entrega acadêmica | Entregar planejamento do software e deixar a base técnica organizada |
| **M3 — Prova de Conceito Técnica** | 07/10/2026 | Entrega acadêmica | Demonstrar que os maiores riscos técnicos e o núcleo da mecânica funcionam |
| **M4 — MVP Jogável** | 04/11/2026 | Entrega acadêmica | Ter uma partida completa, jogável, compreensível e validável |
| **M5 — Polimento, Estabilização e Release Candidate** | 29/11/2026 | Meta interna | Fechar conteúdo final, corrigir problemas, validar memória, documentação e build candidata |
| **M6 — Entrega Final e Apresentação** | 02/12/2026 | Entrega acadêmica | Entregar instalador, documentação e apresentação final |

> **M5 é uma meta interna**, não uma data de entrega do professor.

## Organização das Issues

### M1 — Conceito e Escopo do Jogo
- #1 Escopo do MVP
- #2 Conteúdo biológico dos agentes

### M2 — Planejamento e Base Técnica
- #3 Regras do scanner e decisão
- #4 Build Debug/Release
- #5 Estrutura de pastas
- #6 Game Loop e Delta Time
- #45 Consolidar arquivos de Kanban
- #54 Carregamento dos recursos
- #23 Entrada de mouse

### M3 — Prova de Conceito Técnica
- #7 Máquina de estados
- #8 Input de teclado
- #9 Movimentação
- #10 Detecção de contato
- #11 Vida, dano e invulnerabilidade
- #12 Vírus
- #13 Bactéria
- #14 Célula própria
- #15 Detecção e seleção do scanner
- #48 Exibição das informações do scanner
- #16 Disparo e cooldown do ataque
- #49 Colisão do projétil e resistência
- #17 Defesa e cooldown
- #18 Identificação, pontuação e consequências
- #22 Personagem placeholder
- #24 Geração dos agentes
- #25 Variação da ordem dos encontros
- #26 Destino e conclusão da fase
- #27 Cenário mínimo do PoC
- #28 Teste e registro do PoC

### M4 — MVP Jogável
- #19 HUD
- #29 Vitória e reinício da partida
- #50 Menu inicial
- #51 Pausa
- #52 Vitória
- #53 Game Over
- #20 Integração do fluxo das telas

### M5 — Polimento, Estabilização e Release Candidate
- #30 Spritesheet e animação
- #31 Efeitos sonoros
- #32 Música
- #33 Suporte a múltiplas fases
- #55 Segunda fase jogável
- #34 Hi-score persistente
- #35 Liberação de recursos
- #36 Validação de memória < 100 MB
- #37 Testes de regressão
- #38 Teste com pessoa externa
- #39 Correção de bugs críticos
- #40 Documentação técnica final
- #41 Revisão do One Sheet/GDD
- #42 Build Release

### M6 — Entrega Final e Apresentação
- #43 Instalador Windows
- #44 Roteiro/material da apresentação
- #46 Teste em máquina limpa
- #47 Ensaio da apresentação

## Regras de escopo

- O **MVP permanece centrado em uma fase**.
- A entrega final deve ter mais de uma fase; a segunda fase é tratada como objetivo do pós-MVP. Uma terceira fase só entra se houver tempo e estabilidade suficientes.
- Hi-score, música, refinamentos visuais e outras melhorias entram depois da estabilidade do núcleo.
- Se o cronograma apertar, simplificar primeiro conteúdo pós-MVP e polimento antes de comprometer scanner, identificação, combate, vida, vitória/derrota e estabilidade.
- T21 foi arquivada porque descrevia uma regra de trabalho do assistente, não uma tarefa do produto.

## Critério geral de Done
Uma Issue só deve ir para **Done** quando o critério de aceite da própria Issue estiver comprovado, o código estiver versionado e a alteração não quebrar funcionalidades já validadas.
