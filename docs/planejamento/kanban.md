# Nanobô — Kanban

> Fluxo: **Backlog → To Do → In Dev → In Review → Done**. WIP recomendado: **2 tarefas simultâneas**.
>
> As datas de entrega acadêmicas abaixo vêm do planejamento da disciplina. As datas intermediárias são metas internas para organizar o desenvolvimento.

## Milestones

| Milestone | Data | Tipo | Objetivo |
|---|---|---|---|
| **M1 — Conceito e Escopo do Jogo** | 09/09/2026 | Entrega acadêmica | Fechar conceito, conteúdo mínimo e fronteira do MVP |
| **M2 — Planejamento e Base Técnica** | 23/09/2026 | Entrega acadêmica | Entregar Projeto de Software + Kanban e organizar a base técnica |
| **M3 — Prova de Conceito Técnica** | 07/10/2026 | Entrega acadêmica | Demonstrar os maiores riscos técnicos e o núcleo da mecânica |
| **M4 — MVP Jogável** | 04/11/2026 | Entrega acadêmica | Ter uma partida completa, jogável, compreensível e validável |
| **M5 — Polimento, Estabilização e Release Candidate** | 29/11/2026 | Meta interna | Fechar conteúdo final, corrigir problemas, validar memória, documentação e build candidata |
| **M6 — Entrega Final e Apresentação** | 02/12/2026 | Entrega acadêmica | Entregar instalador, documentação e apresentação final |

> **M5 é uma meta interna**, não uma data de entrega do professor.

## Ordem de desenvolvimento

1. Base técnica: Game Loop, Delta Time e carregamento de recursos.
2. Estados/cenas e entrada de teclado/mouse.
3. Movimentação, entidades, geração e colisões.
4. Scanner, seleção do alvo, pulsos/camadas e identificação única.
5. Exibição das informações, interpretação e decisão.
6. Combate, vida, dano, defesa e consequências.
7. Fluxo completo da Fase 1, pontuação, progressão, vitória e derrota.
8. HUD, menu, pausa, Game Over e integração das telas.
9. Segunda fase, áudio, hi-score, polimento, testes, memória e release.

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
- #177 Objetivos de aprendizagem e critérios educacionais

### M3 — Prova de Conceito Técnica
- #178 Feedback visual e educacional das decisões
- #179 Aprimoramento de movimento (polimento, não bloqueador por padrão)
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
- A Fase 1 é o núcleo do MVP e trabalha bactérias e células saudáveis, incluindo Gram-positivas e Gram-negativas.
- A segunda fase é objetivo pós-MVP e integra a entrega final; seu conteúdo previsto envolve vírus, ciclos lítico/lisogênico e multiplicação limitada.
- Uma terceira fase só entra se houver tempo e estabilidade suficientes.
- Hi-score, música, refinamentos visuais e outras melhorias entram depois da estabilidade do núcleo.
- #177 é requisito de planejamento educacional e deve permanecer associado ao M2.
- #178 integra a validação do núcleo educacional da PoC/MVP.
- #179 é melhoria de game feel e não deve deslocar os riscos críticos da PoC sem decisão explícita da dupla.
- Se o cronograma apertar, simplificar primeiro conteúdo pós-MVP e polimento antes de comprometer scanner, identificação, combate, vida, vitória/derrota e estabilidade.
- T21 foi arquivada porque descrevia uma regra de trabalho do assistente, não uma tarefa do produto.

## Dependências críticas

- Scanner depende da base de entidades e geração de agentes.
- Identificação/decisão depende do scanner e da definição de conteúdo biológico.
- Combate e consequências dependem de entidades, colisões e regras de gameplay.
- Fluxo completo da fase depende do núcleo de gameplay estar estável.
- Menus e estados finais dependem da máquina de estados.
- Segunda fase depende da estabilidade do MVP.
- Release e instalador dependem de build Release e testes de regressão.

## Critério geral de Done

Uma Issue só deve ir para **Done** quando:

1. o critério de aceite da própria Issue estiver comprovado;
2. o teste for reproduzível;
3. o código/documentação estiver versionado;
4. a alteração não quebrar funcionalidades já validadas.
