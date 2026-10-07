# Nanobô — Milestones

| Milestone | Data | Tipo | Objetivo |
|---|---|---|---|
| **M1 — Conceito e Escopo do Jogo** | 09/09/2026 | Entrega acadêmica | Fechar conceito, conteúdo mínimo e fronteira do MVP |
| **M2 — Planejamento e Base Técnica** | 23/09/2026 | Entrega acadêmica | Entregar Projeto de Software + Kanban e organizar a base técnica |
| **M3 — Prova de Conceito Técnica** | 07/10/2026 | Entrega acadêmica | Provar o núcleo de investigação: scanner, camadas e identificação única |
| **M3.5 — Construção do Core Gameplay** | 21/10/2026 | Meta interna | Transformar a PoC em núcleo jogável: agentes, colisões, combate e vida |
| **M4 — MVP Jogável** | 04/11/2026 | Entrega acadêmica | Ter uma partida completa, jogável, compreensível e validável |
| **M5 — Polimento, Estabilização e Release Candidate** | 29/11/2026 | Meta interna | Fechar conteúdo final, corrigir problemas e validar a build candidata |
| **M6 — Entrega Final e Apresentação** | 02/12/2026 | Entrega acadêmica | Entregar instalador, documentação e apresentação final |

> **M3.5 e M5 são metas internas**, não datas de entrega do professor.

## Critérios de foco por marco

### M1 — Conceito e Escopo

Fechar o conceito, o objetivo educacional, o núcleo de gameplay e a fronteira do MVP.

### M2 — Planejamento e Base Técnica

Organizar o planejamento do software, Kanban, estrutura do projeto e base técnica necessária para iniciar a implementação do núcleo.

### M3 — PoC

Provar os riscos técnicos do **núcleo de investigação**, que é a mecânica principal do jogo: Game Loop, Delta Time, estados, entrada por teclado e mouse, movimentação do Nanobô, scanner com seleção do alvo válido mais próximo, leitura em camadas e identificação única por agente.

O registro reproduzível está em `docs/planejamento/qa-poc.md`. **Combate, vida, dano, agentes e colisões não fazem parte do escopo do M3** — foram remanejados para o M3.5.

### M3.5 — Construção do Core Gameplay

Acrescentar ao núcleo já provado tudo o que transforma a PoC em jogo: entidades e geração dos agentes (#13, #14, #22, #24, #25), detecção de contato (#10), vida, dano e invulnerabilidade (#11), ataque, projétil, raio e cooldown (#16, #49, #62–#64) e defesa (#17).

Este marco existe para que o escopo do M3 fique honesto: a PoC provou a investigação, não o combate.

### M4 — MVP

Entregar uma partida completa centrada na Fase 1, com exploração, scanner, interpretação/identificação, combate, vida, pontuação, vitória e derrota, além do fluxo das telas necessário para jogar e reiniciar.

### M5 — Release Candidate

Concluir a expansão pós-MVP, incluindo a segunda fase planejada, e realizar estabilização, testes, validação de memória, documentação e build candidata.

### M6 — Entrega Final

Entregar a versão final com múltiplas fases, requisitos técnicos atendidos, instalador Windows, documentação e material de apresentação.

## Referência operacional

A referência detalhada de Issues, fluxo Kanban e regras de escopo permanece em `docs/planejamento/kanban.md`.
