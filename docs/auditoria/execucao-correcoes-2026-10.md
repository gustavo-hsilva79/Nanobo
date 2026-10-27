# Execução das correções da auditoria — outubro/2026

> **Base:** relatório `Nanobo-auditoria/Auditoria_Nanobo_26-10-2026.md` (26/10/2026) e inventário `inventario-issues-2026-10-26.csv`.
> **Commit de partida:** `1bc1f35` (`master`).
> **Branch de trabalho:** `feat/mvp-fluxo-e-hud` · **PR:** [#193](https://github.com/gustavo-hsilva79/Nanobo/pull/193), mesclado em 26/10/2026 no commit `e512ad745d2a2647b05d7b8351b80f428874a568`.
> **Revisão:** o merge foi feito por decisão do dono do repositório, sem a revisão do Eric; a revisão cruzada fica registrada como pendência retroativa para o próximo PR.
> **Objetivo:** fechar o caminho crítico do MVP de 04/11 sem alterar decisões de Game Design não registradas.

## 1. Fase 0 — revalidação do estado

Reconferido em 26/10/2026, 17:20, antes de qualquer edição:

| Achado da auditoria | Estado atual | Evidência | Ação |
|---|---|---|---|
| Loop sem transição para Resultados | Confirmado | `#70` aberta; `main.c` só desenhava o texto "fase concluida" | Corrigido nesta branch |
| Tela de Resultados ausente | Confirmado | `#57` e `#171–#176` abertas | Implementada versão v0 |
| Vitória e reinício | Confirmado | `#52`, `#72`, `#74`, `#75` abertas | Implementado fluxo de uma fase |
| HUD sem objetivo e sem totais | Confirmado | `#19` aberta; captura de 26/10 | Corrigido |
| Instrução inicial ausente | Confirmado | `#58` e `#103–#107` abertas | Entregue uma tela de instrução; cadeia completa segue fora do MVP |
| Texto da camada 2 entregava a classificação | Confirmado | `gameplay.c:66-79` | Corrigido |
| `#184` contradiz decisão de 22/10 | Confirmado | Issue em M5, Todoist em M4 | Proposta registrada; não fechada |
| Backlog do M5 e prioridade | Confirmado | 80 Issues/221 SP; 172/183 `high` | Propostas na §6; sem limpeza em massa |
| PRs sem revisão | Confirmado | 8 PRs, 0 reviews | PR desta branch aguarda revisão cruzada |
| Todoist só do Gustavo | Confirmado | 50 tarefas, nenhuma do Eric | Sem alteração; regra proposta na §6 |
| Nenhum PR aberto no início | Confirmado | `gh pr list --state open` vazio | — |

Nada foi alterado nesta fase.

## 2. Decisões e contradições

### 2.1 Seis ameaças e nove agentes não são contradição

A composição da Fase 1 é **9 agentes**: 3 bactérias Gram+, 3 Gram− e 3 células saudáveis. As **6 bactérias** são as ameaças; as células não contam para a conclusão. A regra está registrada em `docs/game-design/decisoes.md` (22/10/2026), no GDD e na Issue #26 ("neutralizar as 6 ameaças", "identificar os 9 agentes"). Documentos anteriores a 22/10 falavam em 6 agentes porque o encontro ainda tinha 3 bactérias + 3 células; foram superados pela decisão de ampliar o mapa.

**Decisão:** manter 9 agentes / 6 ameaças. A HUD agora mostra `identificados: x/9` e `restam: x/6` a partir de `NUM_AGENTES` e `threats_total()`, sem literal divergente.

### 2.2 Pontuação e bônus já tinham decisão registrada nas Issues

A auditoria apontou que +10/+15 não estavam em `decisoes.md`. A decisão existe, fechada e com critérios aceitos:

- **#66** (fechada em 23/10): +10 por ameaça neutralizada, uma única vez.
- **#67** (fechada em 25/10): +15 quando `identificados = 9/9`, uma única vez, e 9/9 **não** é requisito de vitória.
- **#26** (fechada em 25/10): 6 ameaças neutralizadas + destino; 9/9 é desempenho.

**Decisão:** manter o comportamento e registrar a regra em `docs/game-design/decisoes.md`, citando as Issues. Nenhum valor foi alterado.

### 2.3 Scanner: pista sem a resposta pronta

O texto da camada 2 nomeava "Gram+/Gram−" e o HUD traduzia o rótulo para o laser, o que permitia vencer sem interpretar o envelope. A correção:

- A camada 2 passou a descrever **parede espessa sem membrana externa** e **parede fina sob membrana externa**.
- A regra que liga a pista à classificação e ao laser é ensinada **uma vez na tela inicial**: parede espessa = Gram+ = raio roxo; parede fina com membrana externa = Gram− = raio vermelho.
- O feedback posterior à ação continua podendo nomear a classificação, porque ele explica uma decisão já tomada.
- A paleta das bactérias **não** foi alterada: a cor continua sendo acessibilidade e ainda pode funcionar como atalho. A troca por paleta neutra + forma é decisão de playtest do M5.

Isso é compatível com `scanner.md` ("o scanner não recomenda ação") e com o objetivo de aprendizagem de `conteudo-biologico.md` ("classificar pela pista do scanner").

### 2.4 Fase 2 — opções (não implementada)

| Opção | Escopo | Esforço aproximado | Risco | Avaliação |
|---|---|---|---|---|
| A — ciclo viral simplificado | vírus com 1 de resistência + janela lítica/lisogênica como estado observável; reusa combate/scanner | ~20 SP | Médio | **Recomendada**: preserva o conteúdo educativo e cabe no calendário |
| B — ciclo viral completo | #83–#91 como estão | ~37 SP | Alto | Não cabe com áudio, hi-score e instalador no mesmo M5 |
| C — fase espelho | outra composição de bactérias, sem conteúdo novo | ~8 SP | Baixo | Atende "múltiplas fases", mas agrega pouco em aprendizagem |

Nada foi implementado. A escolha depende da dupla e do playtest de 05–08/11.

## 3. Correções implementadas

| Item | O que mudou | Onde | Critério de aceite | Evidência |
|---|---|---|---|---|
| **#70** | `phase_completed()` dispara `GAME → RESULTS` uma vez; a lógica para de rodar | `main.c` (ramo do timer) | Conclusão entra em Resultados sem `F` | Captura `qa-mvp-03-resultados.png` |
| **#57 / #171–#176 (v0)** | Tela de Resultados com pontos, ameaças neutralizadas/total, identificados/total e status do bônus | `main.c` (`draw_results`) | Números reais da partida; funciona com e sem bônus | Capturas 03 e 04 |
| **#52 / #72** | Resultados → Vitória; Vitória mostra a pontuação final | `main.c` (`draw_victory`, `handle_key_down`) | Uma fase conclui em Vitória | Captura 05 |
| **#74 / #75** | Vitória/Game Over → nova partida pelo `reset_match()` existente | `main.c` | Vida, score, identificação, tempo, agentes, projéteis e cooldowns reiniciados | Captura 06 + harness |
| **#19 (essencial)** | HUD: `restam: x/6`, objetivo dinâmico, estado do raio (`[pronto]`/`[recarga]`), marca do destino quando fora da tela | `main.c`, `gameplay.c` (`threats_total`, `attack_ready`) | Objetivo legível; destino localizável; sem denominador fixo | Capturas 02 e 06 |
| **#58 (recorte do MVP)** | Tela inicial com objetivo, controles e a regra pista → Gram → laser | `main.c` (`draw_menu`) | Pessoa nova entende o objetivo sem explicação verbal | Captura 01 |
| **#178 / #67** | Estado do bônus exposto para a tela | `gameplay.c` (`bonus_awarded`) | Bônus aparece só quando pago | Harness 78/78 |
| **#76 (parcial)** | Teclas `F`, `G` e `H` viram QA **somente no Debug**; Release não salta estado | `main.c` (`#ifdef _DEBUG`) | Release não reage a `F`/`H` | Captura 07 |
| **Pausa por foco** | Perder o foco em `GAME` entra em `PAUSE` | `main.c` (`DISPLAY_SWITCH_OUT`) | Alt-tab não causa dano | Captura `qa-mvp-08-pausa-foco.png` |

### O que ficou fora desta entrega

- Tutorial guiado completo (#103–#107): a cadeia de 18 SP não é necessária para o MVP; a tela de instrução cobre o mínimo.
- Resumo biológico na tela de Resultados (#174): depende do texto final da Fase 2; fica para o M5.
- Troca de paleta das bactérias: decisão de playtest.
- Fase 2, áudio, spritesheet, hi-score e instalador: M5/M6.
- Fechamento e reatribuição de Issues: aguardam revisão (§6).

## 4. Evidências

### 4.1 Build

| Configuração | Comando | Resultado |
|---|---|---|
| Debug\|x64 | `MSBuild Nanobo.vcxproj -p:Configuration=Debug -p:Platform=x64 -t:Rebuild` | 0 avisos, 0 erros, `x64\Debug\Nanobo.exe` |
| Release\|x64 | idem, `-p:Configuration=Release` | 0 avisos, 0 erros, `x64\Release\Nanobo.exe` |

Logs completos em `docs/auditoria/evidencias/build-debug.log` e `build-release.log`.

### 4.2 Regras (harness fora do repositório)

- 68 casos originais + 10 novos: `threats_total`, bônus pago/não pago, reset do bônus, `attack_ready` antes/depois do disparo e durante a defesa.
- Resultado: **78 passaram, 0 falharam**, contra cópia fresca das fontes desta branch.
- O harness continua fora do repositório; a versão atualizada está no template da sessão de verificação.

### 4.3 Teste em jogo (Debug e Release)

Ambiente: Windows 11, monitor 2560×1440, janela maximizada, executável a partir da raiz do repositório. As cenas foram acionadas por mensagens de teclado ao display (sem depender de foco), e a captura foi feita com `PrintWindow`.

| # | Passo | Resultado observado | Arquivo |
|---|---|---|---|
| 01 | Abrir o jogo | Menu com objetivo, controles e regra Gram/laser | `evidencias/qa-mvp-01-menu.png` |
| 02 | ENTER | Partida com 3 corações, 0/9, restam 6/6, objetivo na HUD, marca do destino | `qa-mvp-02-jogo.png` |
| 03 | Concluir a fase sem identificar (QA `F`) | Resultados: 60 pontos, 6/6, 0/9, bônus não obtido | `qa-mvp-03-resultados.png` |
| 04 | Concluir com 9/9 (QA `H`) | Resultados: 75 pontos, 6/6, 9/9, bônus +15 | `qa-mvp-04-resultados-bonus.png` |
| 05 | ENTER em Resultados | Vitória com a pontuação final | `qa-mvp-05-vitoria.png` |
| 06 | ENTER em Vitória | Nova partida: 0/9, 6/6, 0 pontos, 3 corações, tempo zerado | `qa-mvp-06-nova-partida.png` |
| 07 | Release + `F` e `H` | Nenhuma transição; permanece em GAME | `qa-mvp-07-release-sem-teclas.png` |
| 08 | Alt+Tab durante a partida | Entra em `PAUSE` com `esc para voltar` | `qa-mvp-08-pausa-foco.png` |
| 09 | Partida parada até a vida zerar | `GAME OVER` com `enter = nova partida` | `qa-mvp-09-game-over.png` |

**Não verificado nesta sessão:** movimento com teclas seguradas, leitura real do scanner e combate com teclado físico (a injeção de tecla segurada continua não confiável neste ambiente); instalação em máquina limpa. Esses itens estão no roteiro de `docs/planejamento/qa-mvp.md` para a dupla executar.

## 5. Documentação sincronizada

| Documento | Alteração |
|---|---|
| `docs/game-design/decisoes.md` | Fluxo de telas, pontuação/bônus, pausa por foco, texto do scanner |
| `docs/game-design/scanner.md` | Camada 2 sem o rótulo Gram; regra ensinada na tela inicial |
| `docs/game-design/gdd.md` | Estados reais, HUD, pontuação, Resultados/Vitória |
| `docs/game-design/escopo.md` | Estado do MVP em 26/10 |
| `README.md` | Estado do produto, controles, links dos novos documentos |
| `docs/planejamento/fluxo-estados.md` | Implementação real das transições |
| `docs/planejamento/kanban.md` | Nota datada de 26/10 e correções de #186/#29 |
| `docs/planejamento/qa-poc.md` | Nota sobre passo fixo de 1/60 s |
| `docs/planejamento/qa-mvp.md` | **Novo** roteiro ponta a ponta |
| `docs/auditoria/execucao-correcoes-2026-10.md` | **Novo** este documento |

## 6. GitHub e Todoist — propostas, nada aplicado

| Item | Proposta | Por que não foi aplicado |
|---|---|---|
| #70, #52, #57, #72, #74, #75, #76 | Fechar após revisão do PR, com os critérios marcados | São do Eric; a regra da dupla é revisão cruzada |
| #19 | Revisar e fechar com o Eric | Idem; o HUD essencial está implementado |
| #58 / #103–#107 | Mover para o M5 (tutorial guiado) e manter a tela de instrução no MVP | Decisão de escopo da dupla |
| #179 | Mover para o M5 | Polimento de game feel |
| #184 | Fechar como *not planned* ou reescrever | Contradiz a decisão de 22/10; exige decisão da dupla |
| #65 | Corrigir milestone M6 → M4/M5 | Marco fechado exige `gh api -X PATCH`; não é bloqueio do MVP |
| #185, #184 | Definir responsável e label | Não atribuir sem acordo |
| M5 (80 Issues / 221 SP) | Consolidar microtarefas e cortar a Fase 2 para ~20 SP | Mudança de backlog; decisão da dupla |
| Project | Mover o trabalho real para `In Dev` e usar `Estimate` | Exige acesso e acordo de fluxo |
| Todoist #18 | Concluir a tarefa (Issue já fechada) | Board pessoal; não alterado sem confirmação |
| Todoist #58 | Reagendar ou remover, conforme o corte do tutorial | Depende da decisão acima |

**Regra proposta:** GitHub é a fonte do escopo e do estado; Todoist é a agenda pessoal do Gustavo. Nenhuma tarefa do Todoist precisa repetir marco ou Story Points.

## 7. Plano até 02/12/2026

| Janela | Foco | Saída |
|---|---|---|
| 27–28/10 | Revisão do PR; decisões de escopo (tutorial, Fase 2, #184) | PR revisado e decisões registradas |
| 29–31/10 | Ajustes do playtest interno; correções do que o teste revelar | Bugs corrigidos |
| 01–03/11 | Roteiro de QA do MVP; congelamento | Relatório de aceite |
| 04/11 | Entrega do MVP | Demonstração sem teclas de QA |
| 05–08/11 | Playtest com 3–5 pessoas; decisão da Fase 2 | Dados observados + decisão |
| 09–15/11 | Fase 2 (opção escolhida) e suporte a múltiplas fases | Incremento jogável |
| 16–22/11 | Spritesheet, áudio, hi-score, memória com áudio | Requisitos finais ativos |
| 23–29/11 | Instalador, máquina limpa, regressão, documentação | Release candidate |
| 30/11–02/12 | Ensaio, contingência, entrega | Pacote final e apresentação |

## 8. Pendências para a dupla

1. **Revisar o PR** e decidir se o fluxo de telas fica no MVP como está.
2. **Tela de Resultados:** manter a v0 (pontos/ameaças/identificados/bônus) ou incluir o resumo biológico (#174) antes de 04/11?
3. **Tutorial:** confirmar o corte da cadeia #103–#107 para o M5 e manter só a tela de instrução no MVP.
4. **Scanner:** validar no playtest se a pista do envelope basta para classificar sem a resposta pronta.
5. **Paleta:** decidir no M5 se as cores das bactérias continuam iguais às dos lasers.
6. **Fase 2:** escolher entre A (recomendada), B ou C.
7. **#184:** fechar como *not planned* ou reescrever para "indicador do alvo atual".
8. **Issues do Eric:** definir quem revisa #19/#52/#57/#70–#76 e quando fecham.
9. **Milestone #65:** corrigir para M4/M5.
10. **Harness de regras:** decidir se ele passa a ser versionado no repositório (fora do build do jogo) ou continua externo; hoje está documentado em `qa-mvp.md` §3, mas não é reproduzível só com o repositório.

## 9. Resumo final

**Corrigido e testado**
- Transição automática GAME → RESULTS (#70).
- Tela de Resultados com números reais, nos dois caminhos de bônus (#57/#171–#176 v0).
- Resultados → Vitória → nova partida com reset (#52/#72/#74/#75).
- HUD com objetivo, `restam x/6`, estado do raio e marca do destino (#19 essencial).
- Tela inicial com objetivo, controles e regra Gram/laser (#58 recorte).
- Texto da camada 2 sem a classificação pronta (#3/#177).
- Teclas de QA fora do Release (#76 parcial).
- Pausa por perda de foco (Alt+Tab), verificada em jogo.
- Build Debug/Release sem avisos; harness 78/78.

**Corrigido, mas não testado**
- Persistência da preferência de laser entre partidas (comportamento mantido e documentado; sem teste dedicado).

**Não corrigido**
- Fase 2, áudio, spritesheet, hi-score, instalador, memória com áudio: M5/M6.
- Tutorial guiado completo: fora do MVP por decisão de escopo proposta.
- Backlog do M5 e prioridade: dependem da dupla.

**Aguardando decisão da dupla**
- Fechamento/revisão das Issues do Eric.
- #184, #58/#103–#107, #179, #65, #185.
- Paleta, Fase 2, resumo biológico na tela de Resultados.

**Bloqueado por acesso/permissão**
- GitHub Project via API (`read:project` ausente): a leitura foi feita pela view pública.
- Teste em máquina limpa: não há instalador.
