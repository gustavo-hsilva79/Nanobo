# Plano de execução do backlog restante — Nanobô

> **Gerado em:** 16/10/2026.
> **Atualização de 18/10/2026:** este retrato é de 16/10. Desde então #14, #16, #17, #49, #50, #51, #53, #62–#64, #68, #69 e #73 foram implementados e fechados. Para o estado corrente, vale `docs/planejamento/kanban.md`.
> **Fontes:** GitHub via `gh` (124 issues abertas / 55 fechadas), Todoist do projeto (64 tarefas abertas), `docs/planejamento/`, `docs/game-design/` e leitura direta do `src/` no commit `ec7f072`.
> **Regra desta etapa:** planejar. Nada foi alterado no repositório, nas issues, nos milestones ou no Todoist.
> **Escopo:** detalhado até o MVP (04/11/2026); esboço depois dele até a entrega final (02/12/2026).

---

## 1. Resumo executivo do estado real

| Indicador | Valor verificado |
|---|---|
| Issues abertas / SP | **124 / 337** |
| Issues fechadas | 55 (125 SP somados; 6 fechadas sem Story Points) |
| Abertas por marco em 16/10 | M3.5 = 7 (21 SP) · M4 = 34 (84 SP) · M5 = 79 (219 SP) · M6 = 4 (13 SP) |
| Carga por responsável (abertas) | **Gustavo** 59 issues / 169 SP · **Eric** 65 issues / 168 SP |
| Tarefas abertas no Todoist | 64 (60 ligadas a issue + 4 metas) |
| Código do combate | ataque, projétil, laser, defesa, cooldown e resistência **implementados**; pontuação **não existe** |

Leitura honesta: a divisão de carga está aplicada e equilibrada no GitHub (169 × 168 SP, `assignees` preenchidos em 124/124 issues). O problema não é distribuição, é **tempo**: o M3.5 tinha 7 issues abertas, todas do Gustavo, e fechava em 5 dias; o M5 concentra 219 SP em 25 dias.

O combate já está ligado: `hit_projectiles()` chama `hit_agent()` (`src/gameplay/gameplay.c`), e o disparo, o cooldown e a defesa estão em uso. O M3.5 está fechado: #14, #16, #49 e #62–#64 estão concluídas.

---

## 2. Tarefas vencidas e urgentes (Todoist de 16/10)

| Situação | Tarefa | Issue | SP |
|---|---|---|---|
| Vencida (15/10) | Implementar seleção do laser de ataque | #62 | 2 |
| Vencida (15/10) | Combinar como o HUD mostra a vida | #11/#19 | — |
| Vence hoje | Implementar disparo e direção do projétil | #63 | 3 |
| Vence hoje | Implementar cooldown do ataque | #64 | 2 |
| Vence hoje | Combinar a mitigação da defesa | #17 | — |
| 18/10 | Colisão do projétil e resistência dos agentes | #49 | 3 |
| 19/10 | Comportamento da célula própria | #14 | 3 |
| 20/10 | Defesa e cooldown | #17 | 3 |
| 21/10 | Integração de ataque, laser e cooldown | #16 | 5 (agrupadora: 0 no esforço) |

Duas dessas decisões já foram fechadas em 16/10 nesta sessão (mitigação da defesa e controles de combate — ver `docs/game-design/decisoes.md`). A do HUD continua aberta: a vida é regra do Gustavo (`#11`, fechada) e o desenho é da #19 (Eric).

---

## 3. Divergências entre Todoist, GitHub e documentação

| # | Divergência | Evidência | Ação sugerida |
|---|---|---|---|
| 1 | **65 issues abertas não têm tarefa no Todoist** | Cobertura: M3.5 7/7 · M4 12/34 · M5 38/79 · M6 2/4 | decidir se o Todoist espelha o marco atual ou o backlog inteiro |
| 2 | Tarefa do **#65** continua aberta com a issue fechada | `#65 CLOSED M6` | fechar a tarefa |
| 3 | **`Prazo-alvo` inutilizável**: 7/7 do M3.5 em divergência | #14 03/10, #16 06/10, #17 06/10, #49 06/10, #62/#63/#64 sem data | alinhar com as datas do Todoist |
| 4 | Só **44 de 124** abertas têm `Prazo-alvo` | 80 sem data (todo o M5) | priorizar por marco e dependência, não por data |
| 5 | `priority: high` em **120 de 124** | só #30, #31, #32, #179 não têm | abandonar a label como critério |
| 6 | **22 de 41** issues de M3.5/M4 sem seção `## Dependências` | M3.5: #16, #49, #62–#64 | registrar dependência em "Issue pai"/"Tarefas relacionadas" |
| 7 | Milestones trocados | #8 e #9 fechadas em **M6** sendo trabalho do M3 | corrigir para M3 |
| 8 | Duplicatas abertas no M6 | #43, #44, #46, #47 (as cadeias do M5 já cobrem) | decidir: fechar como duplicata ou assumir como entrega |
| 9 | #124 e #125 fechadas dentro do M3.5 | marco posterior ao escopo delas (M3) | sem ação; só registro |

> **Correção de registro:** um relatório intermediário desta auditoria afirmou "0 assignees em 124 issues". É **falso** — a verificação direta pela API mostra responsável em 124 de 124. Nenhuma conclusão deste plano usa aquele número.

---

## 4. Dependências e caminho crítico

### 4.1 M3.5 — a cadeia fecha em série, no mesmo par de arquivos

```
#62 seleção do raio (2 SP)
   └─ #63 disparo e direção do projétil (3 SP)      [usa player.facing, já existe]
        └─ #49 colisão do projétil + resistência (3 SP)   [usa hit_agent, já existe]
             └─ #64 cooldown do ataque (2 SP)
                  └─ #16 integração ataque/laser/cooldown (agrupadora, 0 SP)

#17 defesa e cooldown (3 SP) — independente do ataque, só compartilha a regra
                              "não atacar durante a defesa"
#14 célula própria (3 SP) — isolada; dependências #2, #24, #10, #11 já fechadas
```

Todas as seis mexem em `src/gameplay/gameplay.c`, `gameplay.h` e `src/main.c`. Pela regra de não paralelizar quem altera os mesmos módulos, **uma por vez**.

### 4.2 M4 — depois do combate, três frentes que correm em paralelo

| Frente | Cadeia | SP |
|---|---|---|
| Combate e pontuação | #18 → #66, #67 → #19 (HUD depende de #11, #18, #64) | 13 |
| Telas e fluxo | #50, #51, #52, #53 → #20 → #68…#76 | 33 |
| Resultados | #57 → #171…#176 | 11 |
| Tutorial | #58 → #103 → #104 → #105 → #106 → #107 | 18 |
| Fechamento da Fase 1 | #26 (depende de #18 e #70) | 3 |
| Sensação e feedback | #178, #179 | 6 |

### 4.3 M5 — agrupado por tema (219 SP)

| Tema | Issues | SP |
|---|---|---|
| Fase 2 + múltiplas fases | #55, #33, #83–#91 | 37 |
| Hi-score persistente | #34, #77–#82 | 22 |
| Memória e limite de 100 MB | #36, #114–#119 | 16 |
| QA, regressão e bugs | #37, #39, #127–#133, #139–#143 | 44 |
| Teste externo | #38, #134–#138 | 16 |
| Documentação | #40, #41, #144–#153 | 30 |
| Release e instalador | #42, #154–#164 | 30 |
| Áudio e sprites | #30, #31, #32 | 10 |
| Apresentação | #165–#170 | 11 |
| Liberação centralizada de recursos | #35 | 3 |

### 4.4 Arquivos que conflitam

`src/main.c` é o gargalo: entrada (teclas), HUD e telas moram nele. #17, #62–#64 e #49 acrescentam teclas e HUD; #19, #50–#53 e #57 acrescentam telas. Duas pessoas editando `main.c` ao mesmo tempo é conflito garantido — a fila entre Gustavo e Eric precisa ser explícita na semana do M4.

---

## 5. Cronograma por janelas temporais

> Estimativas de esforço usam os Story Points das issues. As taxas em SP/dia são **[ESTIMATIVA]** calculada por regra de três, não medida — o projeto não tem histórico de velocidade registrado.

### 5.1 16–18/10 (hoje até domingo) — abrir o combate

| Ordem | Issue | Resultado concreto | SP |
|---|---|---|---|
| 1 | **#17** | defesa ativa 2 s, cooldown 3 s, imune a dano, empurra bactéria, anel no Nanobô | 3 |
| 2 | **#62** | raio escolhido (roxo = Gram+, vermelho = Gram−) | 2 |
| 3 | **#63** | disparo cria projétil na direção do `facing` | 3 |

Eric, em paralelo (mitigação já registrada em `divisao-issues-dupla.md`): **#50** menu e **#51** pausa — não tocam em `gameplay.c`.

### 5.2 19–21/10 — fechar o M3.5

| Ordem | Issue | Resultado concreto | SP |
|---|---|---|---|
| 4 | #63 (terminar) | — | — |
| 5 | **#64** | cooldown de 1 s entre disparos | 2 |
| 6 | **#49** | projétil chama `hit_agent()`; resistência cai | 3 |
| 7 | **#14** | célula própria com comportamento definido | 3 |
| 8 | **#16** | integração e verificação ponta a ponta | 0 |

**A conta, corrigida:** o cartão do marco soma 21 SP, mas a #16 é agrupadora de **0 no esforço** — o trabalho real é **16 SP** em 5 dias úteis (16, 18, 19, 20 e 21/10, sem sábado) = **3,2 SP/dia**. Cabe no teto do WIP 2 (~3,6 SP/dia), no limite. O risco do marco não é volume, é **sequência**: são cinco mecânicas novas no mesmo par de arquivos, nenhuma com código hoje, e #63 depende de #62, #49 depende de #63.

Ponto de checagem em **18/10**: se #17 e #62 não estiverem fechadas, cortar a **#14** do marco (3 SP) — comportamento da célula não bloqueia nada do combate. Sem esse corte, o marco chega em 21/10 com issue aberta.

### 5.3 22–25/10 — núcleo jogável do M4

#18 (5) → #66 (3) → #67 (2) → #19 (3) · Eric: #53 (2), #57 (3), #171 (1).

### 5.4 26/10–04/11 — fechar o MVP

Tutorial (#58 + #103–#107 = 18 SP), fluxo de telas (#20 + #68–#76 = 22 SP), dados de resultados (#172–#176 = 10 SP), #178 (3), #179 (3), #26 (3). Dupla em paralelo, `main.c` serializado.

### 5.5 05/11–29/11 — M5 (o maior risco do projeto)

219 SP em 25 dias = **8,8 SP/dia na dupla [ESTIMATIVA]**. É o dobro da taxa implícita no M4. Sem corte de escopo, esse marco não fecha: o candidato natural a sair é a Fase 2 completa (#55 + #83–#91 = 34 SP), mantendo o que o MVP exige (uma fase que ensina explorar → escanear → identificar → decidir).

### 5.6 30/11–02/12 — M6

#43 (5), #44 (3), #46 (3), #47 (2) = 13 SP. Depende de a Release do M5 estar pronta em 29/11.

---

## 6. Distribuição da dupla

| Janela | Gustavo | Eric |
|---|---|---|
| 16–18/10 | #17 → #62 → #63 | #50 → #51 |
| 19–21/10 | #63, #64, #49, #14, #16 | #53, #57, #171 (adianta M4) |
| 22–25/10 | #18, #66, #67 | #57 fechamento, #172–#176 |
| 26/10–04/11 | #58, #103–#107, #26 | #20, #68–#76, #19 |
| M5 | Fase 2, áudio/sprites, QA de gameplay | hi-score, memória, release, docs |
| M6 | apresentação técnica | instalador, validação em máquina limpa |

Assimetria a resolver: **o M3.5 é 100% do Gustavo** (21 SP) enquanto Eric tem 0 SP nele — é o motivo de a mitigação "Eric adianta as telas do M4" existir. Das 34 issues do M4, 22 já são dele.

---

## 7. Riscos e decisões pendentes

| Risco | Impacto | Mitigação |
|---|---|---|
| R1 — 16 SP de mecânica nova em 5 dias úteis, sem código hoje | se #17 ou #62 atrasar, a cadeia inteira atrasa | checagem em 18/10: cortar #14 (3 SP) |
| R2 — M5 com 219 SP em 25 dias | entrega final incompleta | definir o corte agora, não em novembro |
| R3 — `hit_agent()` sem chamador | a #49 pode reescrever regra que já funciona | o disparo chama a função existente |
| R4 — `main.c` como gargalo de duas pessoas | conflito de merge na semana do M4 | fila explícita de quem mexe em `main.c` |
| R5 — datas e atribuições divergentes (Todoist × GitHub) | decisão tomada olhando número errado | fonte única: Todoist para execução, GitHub para escopo |
| R6 — tecla da defesa sem referência consolidada | retrabalho de input | decidida em 16/10: `X` (ver anexo B) |
| R7 — HUD do painel do scanner estoura a tela | 12 linhas a partir de y=370 com passo 14 chegam a y=524 num HUD de 480 | não empilhar texto novo no painel; HUD de combate no bloco de cima |

**Decisões pendentes [PENDÊNCIA]:**
- valor da pontuação por ameaça neutralizada e do bônus de identificação (#66, #67) — não está em nenhum documento;
- regras do vírus na Fase 2 (#83) — `needs: design`;
- contrato do HUD de vida entre #11 (regra) e #19 (desenho);
- qual fonte de data é a verdadeira (recomendo o Todoist).

---

## 8. Critérios de "pronto" por marco

**M3.5** — a partida tem combate de ponta a ponta: escanear → identificar → escolher o raio → disparar → neutralizar (resistência cai e o agente sai da cena) → defender (2 s de imunidade, empurrão, 3 s de cooldown) → vida cai só fora da defesa. Cada issue com passos numerados registrados em `docs/planejamento/qa-poc.md`.

**M4 (MVP)** — menu → tutorial → Fase 1 completa → resultados → nova partida, sem softlock; HUD com vida, pontuação e objetivos; Game Over real por vida = 0 (sem as teclas de teste F/G/V); #76 validado.

**M5** — múltiplas fases com Fase 2 ou corte formalmente registrado; hi-score persistente; **≤ 100 MB medido** com evidência; regressão registrada; teste externo realizado; documentação revisada contra o código; build Release + instalador gerados.

**M6** — instalador validado em máquina limpa; ensaio da apresentação dentro do tempo.

---

## 9. Plano de revisão e validação

1. **Quem implementa não valida** — revisão cruzada obrigatória (regra de `divisao-issues-dupla.md`).
2. **Evidência em documento, não em comentário de issue** — passos numerados e resultado observado em `docs/planejamento/qa-*.md`; a issue fecha direto com os critérios marcados.
3. **Sem teste automatizado** — a injeção de tecla do Allegro não é confiável (registro da #181), então toda validação de gameplay é manual e reproduzível, com o executável e a pasta `assets/` ao lado.
4. **Antes de fechar cada marco**: rodar o roteiro do marco anterior para garantir que nada regrediu.

---

## 10. Mudanças recomendadas no Todoist/Kanban — **para aprovação, nada aplicado**

| # | Mudança | Por quê |
|---|---|---|
| 1 | Fechar a tarefa do **#65** | issue já fechada |
| 2 | Alinhar `Prazo-alvo` das 7 issues do M3.5 com as datas do Todoist | hoje 4 estão vencidas e 3 não têm data |
| 3 | Aposentar `priority: high` (ou remapear para p1/p2/p3 reais) | 120 de 124 issues têm a mesma label |
| 4 | Preencher `## Dependências` das 22 issues sem a seção | dependência só existe em "Issue pai" |
| 5 | Corrigir milestone de #8 e #9 (M6 → M3) | trabalho do M3 arquivado no marco errado |
| 6 | Decidir as duplicatas #43, #44, #46, #47 do M6 | ou fecham como duplicata, ou o M5 perde essas cadeias |
| 7 | Decidir o destino das 65 issues sem tarefa no Todoist | o Todoist hoje cobre 48% do backlog aberto |
| 8 | Registrar a fonte única de prazos | duas fontes discordando (GitHub × Todoist) |
| 9 | Cortar ou repactuar o escopo do M5 antes de 05/11 | 219 SP em 25 dias |

---

## 11. As três ações de maior impacto

1. **Fechar o combate do M3.5 na ordem #17 → #62 → #63 → #64 → #49.** É o que separa a PoC de um jogo, o marco vence em 5 dias e a decisão que travava a #17 foi fechada hoje. Nada mais no backlog destrava tanto trabalho do M4 (#18, #19, #66, #67).
2. **Fixar o ponto de checagem de 18/10 no M3.5.** Os 16 SP de esforço cabem no WIP 2 (3,2 SP/dia em 5 dias úteis), mas são cinco mecânicas novas no mesmo par de arquivos e nenhuma com código hoje. Se #17 e #62 não fecharem até domingo, cortar a #14 do marco na hora — decidir isso no dia 21 já é tarde.
3. **Tirar o M5 do limbo.** 219 SP em 25 dias não cabem na capacidade; decidir já o que sai (a Fase 2 completa é o maior candidato, 34 SP) evita descobrir o estouro em 25/11, quando não há mais tempo de reagir.

---

## Anexo A — #17 especificada em 16/10 (pronta para virar código)

| Item | Decisão |
|---|---|
| Mitigação | **imunidade total** a dano de contato durante os 2 s |
| Janela / recarga | 2 s ativos, 3 s de cooldown, sem custo de energia |
| Tecla | `X` (apertar ativa, não é segurar) |
| Efeito no agente | empurrão: 2× a velocidade por 0,5 s, direção = vetor Nanobô → agente |
| Alcance do empurrão | só bactérias (Gram+ e Gram−); a célula própria é escopo da #14 |
| Contato fora da defesa | inalterado (comportamento da #11) |
| Pausa | timers congelam (já decorre de `update_match` só rodar em GAME) |
| Ataque × defesa | enquanto a defesa está ativa o disparo é ignorado; projétil em voo não é cancelado |
| HUD | anel em volta do Nanobô: ciano forte durante os 2 s, cinza durante os 3 s de recarga, ausente quando pronto |
| Reset | entrar no `reset_match` como estado zerado |
| Onde mora | `gameplay.c`/`gameplay.h` (regra) + `main.c` (tecla e anel) — nenhum arquivo novo no `Nanobo.vcxproj` |

Registro da decisão: `docs/game-design/decisoes.md`. Roteiro de verificação manual: `docs/planejamento/qa-poc.md` §9.

## Anexo B — mapa de teclas (16/10/2026)

| Tecla | Ação | Status |
|---|---|---|
| WASD / setas | mover | existe |
| ESPAÇO | scanner | existe |
| ESC | pausa | existe |
| ENTER | confirmar / nova partida | existe |
| **X** | **defesa** | implementado (#17) |
| **J** | **ataque** | implementado (#63) |
| **1 / 2** | **seleção do laser** | implementado (#62) |
| F, G, V | transições de teste | remover quando #20/#7 fecharem |

Referências da escolha: Hades 2 usa `Space` para dash (ocupada aqui pelo scanner); Enter the Gungeon usa o botão direito do mouse (o mouse ainda não tem função de gameplay, por `interacao-mouse.md`); Hollow Knight usa `X` para ataque e `C` para dash — `X` é tecla de ação de combate na referência mais próxima. `SHIFT` foi descartada porque o Windows dispara o prompt de **Teclas de Aderência** ao tocar SHIFT cinco vezes.
