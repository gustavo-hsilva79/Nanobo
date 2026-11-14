# Nanobô — Milestones

> **Referência escrita única dos marcos.** A fonte operacional das datas e do escopo é o **GitHub Milestones** do repositório; a tabela abaixo reproduz o que está lá.
>
> Documentos de entrega acadêmica — [projeto-de-software.md](projeto-de-software.md), [one-sheet.md](../one-sheet.md) — foram escritos na data da respectiva entrega e são registro daquele momento, não referência viva. Se um deles divergir daqui, vale esta página e, acima dela, o GitHub.

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

> **Nanobô 2.0 (13/11/2026):** o **M5** passa a incluir o redesenho decidido pelo dono: 3 fases com mapas próprios (bactérias, vírus nas vias aéreas e fungos no pulmão), narrativa, 8 objetivos de ensino como mecânicas, barra de vitais, arte e interface novas, além dos requisitos obrigatórios (Top 10, build Release e instalador, agora com o Gustavo). O **M6** continua sendo a entrega e a apresentação de 02/12. O cronograma por etapa e a ordem de corte estão no [`kanban.md`](kanban.md) (§"Nanobô 2.0"); a visão do jogo, no [`gdd.md`](../game-design/gdd.md).
>
> **Revisão de 14/11:** a Etapa 1 fechou em 14/11 (#214–#224). Por decisão do dono, a dificuldade passa a vir da IA dos invasores, com duas barras (integridade do Nanobô e vitais do paciente) e sem drenagem por tempo. Entra no M5 a **Etapa 1b** (15–18/11), e a amostra de arte é refeita até 19/11.

## Critérios de foco por marco

### M1 — Conceito e Escopo

Fechar o conceito, o objetivo educacional, o núcleo de gameplay e a fronteira do MVP.

### M2 — Planejamento e Base Técnica

Organizar o planejamento do software, Kanban, estrutura do projeto e base técnica necessária para iniciar a implementação do núcleo.

### M3 — PoC

Provar os riscos técnicos do **núcleo de investigação**, que é a mecânica principal do jogo: Game Loop, Delta Time, estados, entrada por teclado e mouse, movimentação do Nanobô, scanner com seleção do alvo válido mais próximo, leitura em camadas e identificação única por agente.

O registro reproduzível está em `docs/planejamento/qa-poc.md`. **Combate, vida, dano, agentes e colisões não fazem parte do escopo do M3** — foram remanejados para o M3.5.

### M3.5 — Construção do Core Gameplay

Acrescentar ao núcleo já provado tudo o que transforma a PoC em jogo: entidades e geração dos agentes (#13, #14, #22, #24, #25), detecção de contato (#10), vida, dano e invulnerabilidade (#11), ataque, projétil, laser e cooldown (#16, #49, #62–#64) e defesa (#17).

**Estado em 18/10/2026:** entidades, geração, contato, vida, ataque, defesa, resistência e célula própria estão implementados; o marco não tem issues abertas.

Este marco existe para que o escopo do M3 fique honesto: a PoC provou a investigação, não o combate.

### M4 — MVP

Entregar uma partida completa centrada na Fase 1, com exploração, scanner, interpretação/identificação, pontuação e consequências, condições de vitória e derrota, além do fluxo das telas necessário para jogar e reiniciar. Combate, vida, menu, pausa e Game Over já estão prontos desde o M3.5. O M4 fecha com a Fase 1 completa e o fluxo entre duas fases (Fase 2 mínima); a Fase 2 completa é M5.

### M5 — Release Candidate

Concluir a expansão pós-MVP, incluindo a segunda fase planejada, e realizar estabilização, testes, validação de memória, documentação e build candidata.

**Situação em 11/11/2026:** movimento com aceleração e frenagem, áudio (seis efeitos e música em loop), spritesheet animado, regras dos estados do vírus da Fase 2 e medição de memória fecharam numa branch só (#179, #31, #32, #30, #83, #114–#116). O que resta do marco é a Fase 2 jogável (#84–#91), o hi-score Top 10 (#34/#77–#82), as dívidas #206–#212, o teste externo (#38/#134–#138) e o build/instalador (#42/#144–#158).

### M6 — Entrega Final

Entregar a versão final com múltiplas fases, requisitos técnicos atendidos, instalador Windows, documentação e material de apresentação.

## Referência operacional

A referência detalhada de Issues, fluxo Kanban e regras de escopo permanece em `docs/planejamento/kanban.md`.
