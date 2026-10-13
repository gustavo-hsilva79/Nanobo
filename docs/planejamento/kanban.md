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

A tabela de marcos — data, tipo e objetivo de cada um — fica em **[milestones.md](milestones.md)**, que é a referência escrita única. A fonte operacional continua sendo o **GitHub Milestones** do repositório: `milestones.md` reproduz o que está lá, e aqui não se repete a lista para as duas versões não divergirem.

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

> **Responsáveis:** `[G]` = Gustavo, `[E]` = Eric. A distribuição completa, com Story Points e a justificativa do equilíbrio, está em `docs/planejamento/divisao-issues-dupla.md`.

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
- #8 Input de teclado *(fechada)*
- #9 Movimentação *(fechada)*
- #27 Cenário mínimo do PoC *(fechada)*
- #28 Teste e registro do PoC *(fechada)*
- #120 Validar loop principal e Delta Time *(fechada)*
- #121 Validar movimento e controles *(fechada)*
- #122 Validar agentes e colisões *(fechada)*
- #123 Validar scanner e identificação *(fechada)*
- #124 Validar ataque, projétil e defesa *(fechada como não planejada; passou para o M3.5)*
- #125 Validar variação de posições e ordem dos agentes *(fechada como não planejada; passou para o M3.5)*
- #126 Consolidar resultados, limitações e correções necessárias da PoC *(fechada)*
- #181 Confirmar manualmente a movimentação por teclado `[E]`

> O M3 está encerrado: a única issue aberta do marco é a #181. As issues #124 e #125 foram fechadas como não planejadas, e ataque, projétil, defesa e colisões passaram a ser escopo do M3.5.

### M3.5 — Construção do Core Gameplay
- #7 Máquina de estados *(infraestrutura entregue; a Issue cobre o restante)* `[E]`
- #10 Detecção de contato `[G]`
- #11 Vida, dano e invulnerabilidade `[G]`
- #13 Bactéria `[G]`
- #14 Célula própria `[G]`
- #16 Disparo e cooldown do ataque `[G]` *(agrupadora)*
- #17 Defesa e cooldown `[G]`
- #22 Personagem placeholder `[G]`
- #24 Geração dos agentes `[G]`
- #25 Variação da ordem dos encontros `[G]`
- #49 Colisão do projétil e resistência `[G]`
- #62–#64 Seleção do laser, disparo e cooldown `[G]`

> Neste marco o Eric fica com 3 SP (#7) e o Gustavo, com 32 SP. Para não acumular trabalho no M4, o Eric adianta no M3.5 as telas que não dependem do core gameplay: #7, #50, #51 e #53.

### M4 — MVP Jogável
- #18 Identificação, pontuação e consequências `[G]` *(agrupadora)*
- #19 HUD `[E]`
- #20 Integração do fluxo das telas `[E]` *(agrupadora)*
- #26 Destino e conclusão da fase `[G]`
- #29 Vitória e reinício da partida *(fechada)*
- #50 Menu inicial `[E]`
- #51 Pausa `[E]`
- #52 Vitória `[E]`
- #53 Game Over `[E]`
- #57 Tela de resultados entre fases `[E]`
- #58 Tutorial introdutório da Fase 1 `[G]`
- #66 Pontuação por ameaça neutralizada `[G]`
- #67 Bônus de identificação completa `[G]`
- #68–#76 Transições, reinício da partida e validação do fluxo `[E]`
- #103–#107 Introdução a Gram+/Gram−, demonstração guiada e validação `[G]`
- #171–#176 Tela de resultados e preservação da pontuação `[E]`
- #178 Feedback visual e educacional das decisões `[G]`
- #179 Aprimoramento de movimento (polimento) `[G]`

### M5 — Polimento, Estabilização e Release Candidate
- #30 Spritesheet e animação `[G]`
- #31 Efeitos sonoros `[G]`
- #32 Música `[G]`
- #33 Suporte a múltiplas fases `[G]`
- #34 Hi-score persistente `[G]` *(agrupadora)*
- #35 Liberação de recursos `[G]`
- #36 Validação de memória < 100 MB `[G]`
- #37 Testes de regressão `[E]`
- #38 Teste com pessoa externa `[G]`
- #39 Correção de bugs críticos `[E]`
- #40 Documentação técnica final `[E]`
- #41 Revisão do One Sheet/GDD `[E]`
- #42 Build Release `[E]`
- #55 Segunda fase jogável `[G]` *(agrupadora)*
- #65 Identificação única na Fase 1 `[G]` *(no GitHub está no M6 — ver nota do M6)*
- #77–#82 Formato, leitura, gravação e validação do hi-score `[E]`
- #83–#91 Estados, multiplicação e integração da Fase 2 — `[G]` #83–#90 · `[E]` #91
- #114–#119 Medição de memória `[G]`
- #127–#143 QA, regressão, teste externo e correção de bugs — `[G]` #127 e #134–#138 · `[E]` #128–#133 e #139–#143
- #144–#158 Documentação final e build Release — `[G]` #144–#146 · `[E]` #147–#158
- #159–#170 Instalador, teste em máquina limpa e ensaio da apresentação — `[E]` #159–#164 · `[G]` #165–#170

### M6 — Entrega Final e Apresentação
- #43 Criar e testar instalador para Windows `[E]`
- #44 Roteiro e materiais da apresentação `[G]`
- #46 Validar produto final em máquina limpa `[E]`
- #47 Ensaio completo da apresentação `[G]`
- #65 Identificação única na Fase 1 `[G]` *(está neste marco no GitHub, mas é trabalho da Fase 1: deve migrar para o M5)*

> As Issues #43, #44, #46 e #47 continuam no M6 no GitHub mesmo com itens equivalentes no M5. A consolidação dessas duplicidades é decisão da dupla e não foi feita automaticamente. Se as quatro forem fechadas como duplicatas, o equilíbrio se mantém movendo a #165 (1 SP) do Gustavo para o Eric: 159 / 159 SP.

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
