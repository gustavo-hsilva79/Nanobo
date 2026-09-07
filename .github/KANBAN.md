# Nanobô — Kanban

> Fluxo: **Backlog → To Do → In Dev → In Review → Done**. WIP recomendado: **2 tarefas simultâneas**.
>
> As datas de entrega acadêmicas abaixo vêm do planejamento da disciplina. As datas intermediárias são metas internas recomendadas para organizar o desenvolvimento.

## Milestones a criar no GitHub

| Milestone | Data | Tipo | Objetivo |
|---|---|---|---|
| **M1 — One Sheet / Game Design Base** | 09/09/2026 | Entrega acadêmica | Escopo e conceito consolidados |
| **M2 — Projeto de Software + Kanban** | 23/09/2026 | Entrega acadêmica | Planejamento técnico e backlog estruturados |
| **M3 — Proof of Concept** | 07/10/2026 | Entrega acadêmica | Provar os maiores riscos técnicos |
| **M4 — MVP Funcional** | 04/11/2026 | Entrega acadêmica | Núcleo jogável completo e validável |
| **M5 — Finalização Técnica** | 25/11/2026 | Meta interna | Estabilização, memória, documentação e release candidate |
| **M6 — Versão Final + Apresentação** | 02/12/2026 | Entrega acadêmica | Instalador, documentação e apresentação |

> **M5 é uma meta interna**, não uma data de entrega do professor.

## Organização das Issues

### M1 — One Sheet / Game Design Base
- #1 Escopo do MVP
- #2 Conteúdo biológico dos agentes

### M2 — Projeto de Software + Kanban
- #3 Regras do scanner e decisão
- #4 Build Debug/Release
- #5 Estrutura de pastas
- #6 Game Loop e Delta Time
- #45 Consolidar arquivos de Kanban

### M3 — Proof of Concept
- #7 Estados do jogo
- #8 Input de teclado
- #9 Movimentação
- #10 Colisão
- #11 Vida/dano
- #12 Vírus
- #13 Bactéria
- #14 Célula própria
- #15 Scanner
- #16 Ataque/projétil
- #17 Defesa
- #18 Identificação/pontuação/consequências
- #22 Personagem placeholder
- #23 Input de mouse
- #24 Geração dos agentes
- #25 Variação da ordem dos encontros
- #26 Destino e conclusão da fase
- #27 Cenário mínimo do PoC
- #28 Teste/registro do PoC

### M4 — MVP Funcional
- #19 HUD
- #20 Menu/pausa/vitória/Game Over
- #29 Vitória e reinício

### M5 — Finalização Técnica
- #30 Spritesheet/animação
- #31 Efeitos sonoros
- #32 Música
- #33 Suporte a múltiplas fases
- #34 Hi-score persistente
- #35 Liberação de recursos
- #36 Validação de memória < 100 MB
- #37 Testes de regressão
- #38 Teste com pessoa externa
- #39 Correção de bugs críticos
- #40 Documentação técnica final
- #41 Revisão do One Sheet/GDD
- #42 Build Release

### M6 — Versão Final + Apresentação
- #43 Instalador Windows
- #44 Roteiro/material da apresentação
- #46 Teste em máquina limpa
- #47 Ensaio da apresentação

## Regras de escopo

- O **MVP permanece centrado em uma fase**.
- Múltiplas fases, hi-score, áudio completo e refinamentos entram depois da estabilidade do núcleo.
- Se o cronograma apertar, simplificar conteúdo pós-MVP antes de comprometer scanner, identificação, combate, vida, vitória/derrota e estabilidade.
- T21 foi arquivada porque descrevia uma regra de trabalho do assistente, não uma tarefa do produto.

## Critério geral de Done
Uma Issue só deve ir para **Done** quando o critério de aceite da própria Issue estiver comprovado, o código estiver versionado e a alteração não quebrar funcionalidades já validadas.
