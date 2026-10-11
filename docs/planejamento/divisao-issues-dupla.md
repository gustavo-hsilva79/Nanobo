# Divisão de Issues — Nanobô

**Responsáveis:** Gustavo Herick Silva (`gustavo-hsilva79`) e Eric Brito (`EricBrito-bcc`)  
**Auditoria do backlog:** 11/10/2026 — GitHub Issues do repositório `gustavo-hsilva79/Nanobo`  
**Status:** divisão calculada sobre o backlog real e pronta para aplicação no GitHub. Este documento substitui a proposta inicial de divisão, que partia de um retrato de **100 issues / 288 Story Points** e deixava 32 issues sem responsável.

## 1. Objetivo

Distribuir **todas** as issues abertas do Nanobô entre os dois integrantes de forma equilibrada em esforço estimado, mantendo coesão entre tarefas que mexem nos mesmos módulos e garantindo que cada integrante tanto **construa** quanto **verifique** parte do produto.

A divisão não é "metade das issues para cada um": uma issue grande pesa mais que várias pequenas.

## 2. Inventário auditado

| Métrica | Valor |
|---|---|
| Issues abertas | **133** (0 PRs abertos) |
| Issues fechadas | 47 |
| Issues abertas sem responsável | **133** (nenhuma estava atribuída) |
| Story Points brutos (todas as abertas) | **359 SP** |
| Issues agrupadoras (pai), contadas separadamente | **5** — #16, #18, #20, #34, #55 = **28 SP** |
| Sub-issues declaradas dessas agrupadoras | **23 issues** = 52 SP |
| **Base executável (bruto − agrupadoras)** | **331 SP** |
| Issues sem estimativa | **1** — #181 |

**Como os Story Points foram lidos:** eles não estão em label nem em campo de Project acessível; ficam no corpo da issue, no formato `**Story Points:** N`. Cada issue tem uma única linha de estimativa.

**Regra de contagem adotada:** uma issue agrupadora **não entra na soma de esforço** — ela acompanha o andamento das filhas. Somar as duas coisas foi o principal erro da proposta anterior. Com as agrupadoras de volta, o bruto volta a 359 SP, o que confirma a consistência dessa regra.

**Situação dos marcos em 11/10/2026:** o M3 foi concluído em 07/10; **estamos no M3.5** (21/10); M4 em 04/11; M5 em 29/11; M6 em 02/12.

## 3. Análise da proposta inicial

A proposta anterior (143 SP para Gustavo, 145 para Eric) tem aritmética **correta e reprodutível**: cada bloco soma exatamente o que os Story Points reais indicam. O problema não é a conta — é a base de dados e o critério.

### 3.1 O que se mantém

- **Divisão por coesão de módulo**, não por metade das issues: tarefas que mexem no mesmo comportamento ficam com a mesma pessoa.
- **Issues pai como acompanhamento**, com as sub-issues como trabalho executável.
- **Revisão cruzada** obrigatória nas interfaces entre gameplay, agentes, fases e estados.
- **Reavaliação a cada marco**, com transferência de tarefas quando houver bloqueio ou desequilíbrio comprovado.
- **Recusa da contagem bruta como critério** de equilíbrio.

### 3.2 O que foi corrigido

1. **Inventário desatualizado.** A proposta cobria 101 das 133 issues abertas (100 na divisão + a #181). Ficaram de fora **32 issues / 71 SP**: #147–#153, #154–#158, #159–#164, #165–#170, #171–#176, #178 e #179. Exatamente 288 + 71 = 359, o total real. As issues órfãs são quase todas do eixo de documentação, release, instalador e apresentação — ou seja, **do lado do Eric**. Aplicada como estava, a divisão viraria aproximadamente **149 SP contra 210 SP**.
2. **Dupla contagem.** As cinco agrupadoras (#16, #18, #20, #34, #55) eram somadas junto com suas 23 filhas. Nas issues cobertas pela proposta, o esforço executável é 260 SP, não 288.
3. **Desequilíbrio de tipo.** No desenho original o Gustavo recebia **zero** issue de QA, documentação ou entrega, e o Eric recebia **zero** implementação de jogo. Isso cria dois pontos únicos de falha: sem o Gustavo o jogo não existe; sem o Eric nada é validado nem entregue. A divisão abaixo distribui verificação para os dois lados.
4. **#181 sem estimativa era nota de rodapé.** Aqui ela é decisão explícita: fica com o Eric, sem estimativa, e não entra na soma.
5. **Divergências de dados encontradas na auditoria** (registradas na seção 9): a issue #65, que é trabalho da Fase 1, está no marco M6; #124 e #125 foram fechadas como *not planned* e continuam no M3.5; o bloco M3 do Kanban lista issues já fechadas como abertas.

## 4. Princípios da divisão

1. **Equilíbrio por esforço**, medido em Story Points executáveis.
2. **Coesão técnica:** tarefas do mesmo módulo ficam com o mesmo responsável.
3. **Agrupadora não conta SP.** Ela fica com quem tem a maioria das filhas.
4. **Cada um constrói e verifica.** Ninguém fica só implementando nem só testando.
5. **Revisão cruzada:** quem não escreveu é quem valida. Gameplay e Fase 2 implementados pelo Gustavo são validados pelo Eric; telas, estados e áudio implementados pelo Eric são validados pelo Gustavo.
6. **Dependências antes de começar:** issue bloqueada não é iniciada como se fosse independente; a ordem vem do Kanban e dos marcos.
7. **Validação antes de fechar:** compilar não basta — critério de aceite comprovado e teste reproduzível.

## 5. Gustavo Herick Silva (`gustavo-hsilva79`) — 62 issues / **165 SP**

Responsável principal pelo núcleo do jogo: gameplay, agentes, combate, Fase 2 e recursos.

| Bloco | Issues | SP | Nº |
|---|---|---|---|
| M3.5 — núcleo de gameplay | #10 (2), #11 (3), #13 (3), #14 (3), #17 (3), #22 (2), #24 (3), #25 (3), #49 (3), #62 (2), #63 (3), #64 (2) | 32 | 12 |
| M4 — Fase 1 e aprendizagem | #26 (3), #58 (5), #66 (3), #67 (2), #103 (2), #104 (3), #105 (3), #106 (2), #107 (3), #178 (3), #179 (3) | 32 | 11 |
| M5 — Fase 2 (vírus) | #83 (3), #84 (3), #85 (3), #86 (3), #87 (3), #88 (3), #89 (3), #90 (2) | 23 | 8 |
| M5 — recursos, áudio, gráficos, múltiplas fases | #30 (5), #31 (3), #32 (2), #33 (3), #35 (3) | 16 | 5 |
| M5 — memória | #36 (3), #114 (2), #115 (2), #116 (2), #117 (2), #118 (3), #119 (2) | 16 | 7 |
| M5 — verificação da interface e dos estados | #127 (3) | 3 | 1 |
| M5 — teste com pessoa externa | #38 (3), #134 (2), #135 (2), #136 (3), #137 (3), #138 (3) | 16 | 6 |
| M5 — documentação técnica | #144 (3), #145 (2), #146 (3) | 8 | 3 |
| M5/M6 — apresentação | #44 (3), #47 (2), #165 (1), #166 (2), #167 (2), #168 (2), #169 (2), #170 (2) | 16 | 8 |
| M6 — identificação única da Fase 1 | #65 (3) | 3 | 1 |
| **Agrupadoras (acompanhamento, 0 SP)** | #16, #18, #34, #55 | 0 | 4 |
| **Total atribuído** | | **165** | **66** |

## 6. Eric Brito (`EricBrito-bcc`) — 65 issues / **166 SP** (+ #181)

Responsável principal pela interface, progressão, entrega — e por parte da implementação (telas, fluxo, resultados e hi-score).

| Bloco | Issues | SP | Nº |
|---|---|---|---|
| M3.5 — máquina de estados | #7 (3) | 3 | 1 |
| M4 — telas, fluxo e tela de resultados | #19 (3), #50 (2), #51 (2), #52 (2), #53 (2), #57 (3), #68 (1), #69 (2), #70 (2), #71 (2), #72 (1), #73 (1), #74 (2), #75 (3), #76 (3), #171 (1), #172 (2), #173 (2), #174 (2), #175 (2), #176 (2) | 42 | 21 |
| M5 — hi-score | #77 (2), #78 (3), #79 (3), #80 (3), #81 (3), #82 (3) | 17 | 6 |
| M5 — validação da Fase 2 | #91 (3) | 3 | 1 |
| M5 — regressão | #37 (3), #128 (3), #129 (3), #130 (3), #131 (3), #132 (3), #133 (3) | 21 | 7 |
| M5 — estabilização e bugs | #39 (5), #139 (2), #140 (3), #141 (5), #142 (3), #143 (2) | 20 | 6 |
| M5 — documentação final | #40 (3), #41 (3), #147 (2), #148 (3), #149 (2), #150 (3), #151 (2), #152 (2), #153 (2) | 22 | 9 |
| M5 — build Release | #42 (3), #154 (2), #155 (2), #156 (3), #157 (3), #158 (2) | 15 | 6 |
| M5/M6 — instalador e máquina limpa | #43 (5), #46 (3), #159 (2), #160 (3), #161 (2), #162 (3), #163 (3), #164 (2) | 23 | 8 |
| Verificação manual da PoC | #181 (sem estimativa) | — | 1 |
| **Agrupadora (acompanhamento, 0 SP)** | #20 | 0 | 1 |
| **Total atribuído** | | **166** | **67** |

## 7. Conferência do equilíbrio

| | Gustavo | Eric |
|---|---|---|
| Issues executáveis | 62 | 65 |
| Story Points executáveis | **165** | **166** |
| Agrupadoras (0 SP) | 4 | 1 |
| Sem estimativa | 0 | 1 (#181) |
| Total de issues atribuídas | 66 | 67 |

- Soma dos dois: **331 SP** — a base executável inteira, sem agrupadoras.
- Diferença: **1 SP (0,6%)**.
- Cobertura: 62 + 65 + 5 agrupadoras + #181 = **133/133 issues**, nenhuma duplicada entre as duas listas.

### Distribuição por marco

| Marco | Gustavo | Eric |
|---|---|---|
| M3.5 — Core Gameplay (21/10) | 32 SP / 12 issues | 3 SP / 1 issue |
| M4 — MVP Jogável (04/11) | 32 SP / 11 issues | 42 SP / 21 issues |
| M5 — Release Candidate (29/11) | 93 SP / 36 issues | 113 SP / 41 issues |
| M6 — Entrega Final (02/12) | 8 SP / 3 issues | 8 SP / 2 issues |

O desequilíbrio do M3.5 é **intencional**: o marco é a construção do núcleo de gameplay, que exige as entidades, colisões, combate e vida — território do Gustavo. O marco fecha em 21/10 e o único item do Eric nele é a máquina de estados (#7).

**Mitigação obrigatória:** durante o M3.5 o Eric adianta as telas do M4 que **não dependem** do core gameplay — #7 (estados), #50 (menu), #51 (pausa) e #53 (Game Over). Sem isso, ele chega ao M4 com 42 SP concentrados em poucas semanas.

## 8. Regras de execução

1. **Agrupadoras acompanham, não somam.** #16, #18, #34 e #55 com o Gustavo; #20 com o Eric.
2. **Direção da revisão cruzada** — quem valida não é quem escreveu:
   - Eric valida o que o Gustavo construiu: #128, #129, #130, #131 (regressão) e #91 (Fase 2 de ponta a ponta);
   - Gustavo valida o que o Eric construiu: #127 (menu, estados e reinício) e #132 (áudio e recursos);
   - o teste com pessoa externa (#38, #134–#138) fica com o Gustavo, que não é o dono da entrega.
3. **A interface entre os dois eixos precisa de contrato antes do código:** posição/estado do Nanobô, dados que a tela de resultados exibe, formato do arquivo de hi-score e sinais de conclusão de fase.
4. **Transferência só por evidência:** bloqueio real, atraso medido ou desequilíbrio observado ao fim de um marco — não por preferência.

## 9. Aplicação no GitHub

**Aplicado em 11/10/2026.** A atribuição está no GitHub: **66 issues para `gustavo-hsilva79` e 67 para `EricBrito-bcc`**.

- [x] Autenticar a CLI (`gh auth login`) — a API pública permite ler o backlog, mas não gravar responsáveis.
- [x] Aplicar os responsáveis (comandos abaixo, idempotentes — podem ser repetidos para reauditar):

```powershell
gh issue edit 10 11 13 14 17 22 24 25 49 62 63 64 26 58 66 67 103 104 105 106 107 178 179 `
  83 84 85 86 87 88 89 90 30 31 32 33 35 36 114 115 116 117 118 119 127 38 134 135 136 137 138 `
  144 145 146 44 47 165 166 167 168 169 170 65 16 18 34 55 --add-assignee gustavo-hsilva79

gh issue edit 7 19 50 51 52 53 57 68 69 70 71 72 73 74 75 76 171 172 173 174 175 176 `
  77 78 79 80 81 82 91 37 128 129 130 131 132 133 39 139 140 141 142 143 `
  40 41 147 148 149 150 151 152 153 42 154 155 156 157 158 43 46 159 160 161 162 163 164 `
  20 181 --add-assignee EricBrito-bcc
```

- [x] Conferido pela API em 11/10/2026: **133 issues abertas, nenhuma sem responsável, nenhuma com dois**, e **165 / 166 SP** executáveis por integrante.
- [ ] Decisões pendentes da dupla:
  - **Registrar a hierarquia real no GitHub.** Hoje os vínculos pai/filho existem apenas como texto (`## Issue pai`) no corpo das issues — a API de sub-issues retorna zero para #16, #18, #20, #34 e #55. Criar os 23 vínculos declarados (#16←#62–#64, #18←#65–#67, #20←#68–#76, #34←#77–#82, #55←#83) é o que impede a dupla contagem de voltar a acontecer. **Em aberto:** decidir se #84–#91 também se tornam filhas de #55 (o texto só declara a #83).
  - **#65 está no marco errado.** É trabalho da Fase 1 e está no M6 (entrega final). O correto é movê-la para o M4 ou M5.
  - **#124 e #125** estão fechadas como *not planned* mas continuam no marco M3.5 no GitHub.
  - **Duplicatas do M6:** #43, #44, #46 e #47 (13 SP) repetem as cadeias #159–#164 (instalador) e #165–#170 (apresentação) do M5; #40 e #41 repetem #144–#153; #42 repete #154–#158. Esta divisão **mantém as quatro como verificação de aceite da entrega final**. Se a dupla optar por fechá-las, a regra de reequilíbrio é: mover **#165 (1 SP)** do Gustavo para o Eric → 159 / 159.
  - **#181** fica sem estimativa até decisão da dupla. Se ganhar pontos, o próximo item de mesmo tamanho vai para o Gustavo.
- [ ] Conferir o Kanban depois das atribuições — a prioridade continua vindo dos marcos e dos bloqueios reais.

## 10. Variante por módulo estrito

Se a dupla preferir a separação original — Eric com **toda** a interface, QA, documentação e entrega, e Gustavo com **todo** o jogo —, o equilíbrio se mantém trocando dois blocos:

- hi-score (#34, #77–#82, 17 SP) vai para o Eric, que já tem a entrada de nome (#81) e a tela de resultados;
- apresentação (#44, #47, #165–#170, 16 SP) vai para o Gustavo, que explica as decisões técnicas e de Game Design;
- #150 volta para o Eric para fechar a conta.

Resultado: 165 / 166 SP, com a divisão mais próxima de "dev contra QA+entrega". A divisão da seção 5 e 6 é a recomendada porque distribui implementação e verificação entre os dois.

## 11. Limitações e riscos

- **Story Points são relativos** e não equivalem a horas; refletem o entendimento da dupla no momento da estimativa.
- **O backlog muda:** issues podem ser criadas, fechadas ou reestimadas. Qualquer recálculo deve manter a regra de ouro: atribuir por SP executável, nunca por contagem bruta, e nunca somar agrupadora com filha.
- **Desequilíbrio por etapa é esperado.** Os totais fecham no fim, não em cada semana. Reavaliar a cada marco.
- **Dependências entre módulos:** gameplay, HUD, estados e progressão precisam de contratos combinados antes da implementação.
- **Teste e documentação não são trabalho residual:** precisam de tempo reservado e evidência registrada, ou viram a primeira coisa a ser cortada.
- **Campos do GitHub Project não foram tocados.** A API disponível não expõe operações de Project; status e campos continuam sendo atualizados na interface.

## 12. Acordo de trabalho

1. Cada responsável abre uma branch para uma tarefa independente e faz commits pequenos.
2. Issues que alterem interfaces compartilhadas têm o contrato combinado **antes** da implementação.
3. Ao terminar, o responsável registra como testar e quais critérios de aceite foram cumpridos.
4. O colega revisa ou reproduz o teste antes do fechamento, seguindo a direção da revisão cruzada da seção 8.
5. Bugs encontrados na integração viram issue ou comentário vinculado à issue original — não são ocultados para manter o quadro limpo.
6. A dupla reavalia a carga semanalmente e em cada marco, usando trabalho restante e bloqueios reais.

## 13. Critério para considerar a divisão validada

A divisão é considerada validada quando:

- as 133 issues abertas têm responsável definido no GitHub;
- o esforço executável está em 165 / 166 SP, sem agrupadoras somadas;
- os bloqueios e marcos estão coerentes (incluindo a correção da #65);
- ambos concordam com a distribuição;
- a carga real observada após um ciclo não indica desequilíbrio relevante;
- as tarefas continuam sendo revisadas e testadas em conjunto.

**Conclusão:** a proposta inicial acertou o critério (coesão de módulo, não divisão ao meio) e errou a base (100 de 133 issues, agrupadoras somadas com as filhas). Esta versão mantém o que era bom, cobre o backlog inteiro e equilibra **331 SP** em **165 contra 166**, com implementação e verificação dos dois lados.
