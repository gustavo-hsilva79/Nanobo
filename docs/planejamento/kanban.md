# Nanobô — Kanban

> Fluxo: **Backlog → To Do → In Dev → In Review → Done**. WIP recomendado: **2 tarefas simultâneas**.
>
> Este documento é a visão de planejamento. O **GitHub Project** é a fonte operacional do status de cada Issue e os **Milestones** do repositório são a fonte das datas. As listas abaixo reproduzem o agrupamento por milestone; em caso de divergência, vale o GitHub.

## Entregas acadêmicas (Plano de Ensino)

| Instrumento | Aplicação | Composição da nota | Milestone |
|---|---|---|---|
| One Sheet Paper | Aula 5 — 09/09/2026 | 20% | M1 |
| Projeto de Software + Kanban | Aula 7 — 23/09/2026 | 20% | M2 |
| Prova de Conceito | Aula 9 — 07/10/2026 | 20% | M3 |
| Core gameplay loop + MVP | Aula 13 — 04/11/2026 | 20% | M4 |
| Entrega final + apresentação | Aula 17 — 02/12/2026 | 20% | M6 |

> **Atenção:** a tabela de avaliação do Plano de Ensino lista *"Core gameplay loop"* e *"MVP"* como instrumentos distintos, ambos aplicados na **Aula 13**, enquanto o cronograma de aulas descreve a Aula 10 como proposta do MVP e a Aula 13 como sua entrega. O M4 assume as duas leituras; confirmar com o professor se são duas entregas ou uma.

## Milestones

| Milestone | Data | Tipo | Objetivo |
|---|---|---|---|
| **M1 — Conceito e Escopo do Jogo** | 09/09/2026 | Entrega acadêmica | Fechar conceito, conteúdo mínimo e fronteira do MVP |
| **M2 — Planejamento e Base Técnica** | 23/09/2026 | Entrega acadêmica | Entregar Projeto de Software + Kanban e organizar a base técnica |
| **M3 — Prova de Conceito Técnica** | 07/10/2026 | Entrega acadêmica | Provar o núcleo de investigação: scanner, camadas e identificação única |
| **M3.5 — Construção do Core Gameplay** | 21/10/2026 | Meta interna | Transformar a PoC em núcleo jogável: agentes, colisões, combate e vida |
| **M4 — MVP Jogável** | 04/11/2026 | Entrega acadêmica | Ter uma partida completa, jogável, compreensível e validável |
| **M5 — Polimento, Estabilização e Release Candidate** | 29/11/2026 | Meta interna | Fechar conteúdo final, corrigir problemas, validar memória, documentação e build candidata |
| **M6 — Entrega Final e Apresentação** | 02/12/2026 | Entrega acadêmica | Entregar instalador, documentação e apresentação final |

> **M3.5 e M5 são metas internas**, não datas de entrega do professor.
>
> **M3.5 existe para separar o que a PoC realmente provou** — Game Loop, Delta Time, estados, entrada, movimentação, scanner e identificação única, registrados em `docs/planejamento/qa-poc.md` — **do que ficou para depois**: agentes, colisões, combate, vida e condições reais de fim de fase. Sem esse marco, o escopo da PoC pareceria ter incluído o combate, o que não aconteceu.

## Ordem de desenvolvimento

1. Base técnica: Game Loop, Delta Time e carregamento de recursos. *(feito — M2)*
2. Estados/cenas e entrada de teclado/mouse. *(feito — M2)*
3. Movimentação do Nanobô. *(feito — M3)*
4. Scanner, seleção do alvo, pulsos/camadas e identificação única. *(feito — M3)*
5. Entidades, geração de agentes e colisões. *(M3.5)*
6. Combate, vida, dano, defesa e consequências. *(M3.5)*
7. Exibição das informações, interpretação e decisão. *(M3.5 → M4)*
8. Fluxo completo da Fase 1, pontuação, progressão, vitória e derrota. *(M4)*
9. HUD, menu, pausa, Game Over e integração das telas. *(M4)*
10. Segunda fase, áudio, hi-score, polimento, testes, memória e release. *(M5 → M6)*

## Organização das Issues

### M1 — Conceito e Escopo do Jogo
- #1 Escopo do MVP *(fechada)*
- #2 Conteúdo biológico dos agentes *(fechada)*

### M2 — Planejamento e Base Técnica
- #4 Build Debug/Release *(fechada)*
- #5 Estrutura de pastas *(fechada)*
- #6 Game Loop e Delta Time *(fechada)*
- #45 Consolidar arquivos de Kanban *(fechada)*
- #23 Entrada de mouse *(fechada)*
- #3 Regras do scanner e decisão *(fechada)*
- #59–#61 Seleção do alvo, camadas e identificação única *(fechadas)*
- #92–#102 Máquina de estados, transições e validação *(fechadas)*
- #108–#113 Catálogo, carregamento, liberação e validação dos recursos *(fechadas)*
- #177 Objetivos de aprendizagem e critérios educacionais *(fechada)*

### M3 — Prova de Conceito Técnica
- #8 Input de teclado *(entregue)*
- #9 Movimentação *(entregue)*
- #27 Cenário mínimo do PoC
- #28 Teste e registro do PoC
- #120 Validar loop principal e Delta Time
- #121 Validar movimento e controles
- #122 Validar agentes e colisões
- #123 Validar scanner e identificação
- #124 Validar ataque, projétil e defesa
- #125 Validar variação de posições e ordem dos agentes
- #126 Consolidar resultados, limitações e correções necessárias da PoC

### M3.5 — Construção do Core Gameplay
- #7 Máquina de estados *(infraestrutura entregue; a Issue cobre o restante)*
- #10 Detecção de contato
- #11 Vida, dano e invulnerabilidade
- #13 Bactéria
- #14 Célula própria
- #16 Disparo e cooldown do ataque
- #17 Defesa e cooldown
- #22 Personagem placeholder
- #24 Geração dos agentes
- #25 Variação da ordem dos encontros
- #49 Colisão do projétil e resistência
- #62–#64 Seleção do raio, disparo e cooldown

### M4 — MVP Jogável
- #18 Identificação, pontuação e consequências
- #19 HUD
- #20 Integração do fluxo das telas
- #26 Destino e conclusão da fase
- #29 Vitória e reinício da partida
- #50 Menu inicial
- #51 Pausa
- #52 Vitória
- #53 Game Over
- #57 Tela de resultados entre fases
- #58 Tutorial introdutório da Fase 1
- #66 Pontuação por ameaça neutralizada
- #67 Bônus de identificação completa
- #103–#107 Introdução a Gram+/Gram−, demonstração guiada e validação
- #171–#176 Tela de resultados e preservação da pontuação
- #178 Feedback visual e educacional das decisões
- #179 Aprimoramento de movimento (polimento)

### M5 — Polimento, Estabilização e Release Candidate
- #30 Spritesheet e animação
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
- #55 Segunda fase jogável
- #65 Identificação única na Fase 1
- #77–#82 Formato, leitura, gravação e validação do hi-score
- #83–#91 Estados, multiplicação e integração da Fase 2
- #114–#119 Medição de memória
- #127–#143 QA, regressão, teste externo e correção de bugs
- #144–#158 Documentação final e build Release
- #159–#170 Instalador, teste em máquina limpa e ensaio da apresentação

### M6 — Entrega Final e Apresentação
- #43 Criar e testar instalador para Windows
- #44 Roteiro e materiais da apresentação
- #46 Validar produto final em máquina limpa
- #47 Ensaio completo da apresentação

> As Issues #43, #44, #46 e #47 continuam no M6 no GitHub mesmo com itens equivalentes no M5. A consolidação dessas duplicidades é decisão da dupla e não foi feita automaticamente.

## Regras de escopo

- O **MVP permanece centrado em uma fase**.
- A Fase 1 é o núcleo do MVP e trabalha bactérias e células saudáveis, incluindo Gram-positivas e Gram-negativas.
- A segunda fase é objetivo pós-MVP e integra a entrega final; seu conteúdo previsto envolve vírus, ciclos lítico/lisogênico e multiplicação limitada.
- Uma terceira fase só entra se houver tempo e estabilidade suficientes.
- Hi-score, música, refinamentos visuais e outras melhorias entram depois da estabilidade do núcleo.
- A PoC do M3 cobre o núcleo de investigação. Combate, vida, dano, agentes e colisões pertencem ao M3.5.
- #177 é requisito de planejamento educacional e permanece associado ao M2.
- #178 integra a validação do núcleo educacional do MVP.
- #179 é melhoria de game feel e não deve deslocar os riscos críticos do núcleo sem decisão explícita da dupla.
- Se o cronograma apertar, simplificar primeiro conteúdo pós-MVP e polimento antes de comprometer scanner, identificação, combate, vida, vitória/derrota e estabilidade.

## Dependências críticas

- Scanner depende da posição do Nanobô e da base de agentes.
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

## Histórico

- **07/10/2026** — Kanban reconciliado com os Milestones reais do GitHub. O M3 foi reduzido ao núcleo de investigação; o restante foi movido para o novo **M3.5**. A Issue #12 (vírus) saiu do M3: já estava fechada e o vírus é escopo de Fase 2, conforme `docs/game-design/gdd.md`. T21 foi arquivada porque descrevia uma regra de trabalho do assistente, não uma tarefa do produto.
