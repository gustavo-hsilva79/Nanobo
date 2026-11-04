# Nanobô — Estado atual do projeto

**Data de referência:** 03/11/2026  
**Repositório:** [gustavo-hsilva79/Nanobo](https://github.com/gustavo-hsilva79/Nanobo)  
**Base consultada:** README, GDD, Projeto de Software, documentos de Game Design/planejamento, árvore de `src/`, configuração do Visual Studio e estado registrado nos documentos de QA.

> Este documento complementa o registro acadêmico de 23/09/2026. Não substitui os documentos históricos de entrega. O código e os testes reproduzíveis prevalecem sobre descrições antigas quando houver divergência.

## 1. Resumo executivo

Nanobô é um jogo educacional 2D de ação desenvolvido em C com Allegro 5. O ciclo central permanece **Explorar → Escanear → Identificar → Decidir → Agir → Consequência**: a informação biológica deve orientar uma decisão de gameplay, não ser apresentada como quiz isolado.

O repositório registra a PoC como concluída, o core gameplay como implementado e o fluxo integrado do MVP como fechado. O MVP atual concentra-se na Fase 1. A Fase 2, hi-score persistente, áudio final, spritesheet/animação final e instalador Windows permanecem no escopo da entrega final, salvo evidência mais recente nas Issues/QA.

## 2. Estado implementado observado/documentado

| Área | Estado registrado | Evidência/referência |
|---|---|---|
| Fundação | C11, Allegro 5, Visual Studio 2022, game loop e estados | `Nanobo.sln`, `Nanobo.vcxproj`, `src/main.c`, `src/core/game.c` |
| Movimento | WASD/setas; mundo 1920×512 e câmera; tela lógica 640×360 | README e GDD |
| Scanner | Mouse direito sobre agente; seleção/alvo válido; dois pulsos; identificação única | GDD, `src/gameplay/gameplay.c`, QA PoC |
| Combate | Módulos A/B, projétil, resistência, dano por contato, defesa e vidas | GDD, `src/gameplay/gameplay.c`, QA MVP |
| Agentes da Fase 1 | 3 Gram+, 3 Gram− e 3 células saudáveis; total de 9 agentes | README/GDD |
| Pontuação | +10 por ameaça; bônus +15 por identificação completa 9/9; placar máximo documentado 75 | GDD/QA |
| Estados | MENU, BRIEFING, GAME, PAUSE, RESULTS, VICTORY e GAME OVER descritos no fluxo | `src/core/game.c`, fluxo de estados |
| Resultados | Pontos, ameaças, identificações, bônus e resumo por objetivos | README/GDD/QA MVP |
| Menu/HUD | Menu, pausa, Game Over e HUD com objetivo, progresso, módulo e destino | README e QA MVP |
| Controles | Teclado e mouse; cursor mira; botão esquerdo atira; botão direito escaneia; X defesa; 1/2 ou roda troca módulo; ESC pausa; F11 tela cheia | README/GDD |
| QA | Registros da PoC, MVP, estados e carregamento de recursos disponíveis | `docs/planejamento/qa-*.md` |

### Controles atuais

- **WASD/setas:** mover.
- **Cursor:** mirar.
- **Botão esquerdo:** disparar.
- **Botão direito sobre agente:** escanear.
- **1/2, roda do mouse ou ícone do HUD:** trocar módulo A/B.
- **X:** defesa.
- **ESC:** pausar/retomar.
- **ENTER:** iniciar/confirmar/nova partida.
- **F11:** tela cheia.
- **F/G/H:** atalhos de QA somente no build Debug; não devem funcionar como atalhos de progressão no Release.

Os controles antigos de Space/J para scanner/ataque estão obsoletos desde a alteração de interação por mouse (#198).

## 3. Regras de gameplay vigentes documentadas

### Fase 1 — bactérias e células saudáveis

- A aprendizagem envolve coloração de Gram, diferenças estruturais entre Gram+ e Gram− e preservação de células saudáveis.
- O scanner revela pistas em dois pulsos; não recomenda diretamente o módulo.
- Módulo A corresponde a Gram+; módulo B corresponde a Gram−, conforme modelo simplificado do jogo.
- Bactérias exigem dois acertos corretos; módulo incorreto não reduz resistência.
- Atacar uma célula saudável causa dano ao Nanobô e não remove a célula.
- A regra do jogo não deve ser apresentada como orientação sobre medicamentos ou tratamentos reais.

### Pontuação e conclusão

- +10 por ameaça neutralizada.
- +15 por identificação completa, pago uma vez ao concluir.
- O GDD registra uma divergência importante: a regra anterior permitia concluir com identificações incompletas; uma decisão de 03/11 (D2.1) propõe tornar a identificação condição de conclusão. O documento registra que isso depende de resolver o caso de atacar antes de escanear (D2.2).
- **Não declarar D2.1 implementada sem conferir a Issue, o código atual e um teste reproduzível.** Até a confirmação, tratar como decisão/pêndencia em reconciliação.

## 4. Escopo restante para a entrega final

| Item | Estado neste registro | Próxima verificação |
|---|---|---|
| Fase 2 — vírus | Planejada; ciclos lítico/lisogênico e multiplicação limitada | Regras pedagógicas, limite, critérios de aceite e QA |
| Hi-score persistente | Planejado no README/GDD | Gravação/leitura, Top 10, empate e persistência entre execuções |
| Música e SFX | Requisito final; estado precisa ser confirmado por assets e teste | Verificar assets, carregamento, mixagem e execução |
| Spritesheet/animação | Requisito final; confirmar cobertura real no jogo | Inspecionar assets e animações em runtime |
| Memória ≤100 MB | Requisito | Medição reproduzível em Release |
| Instalador Windows | Requisito final | Build Release e teste em máquina limpa |
| Release e apresentação | Planejadas para 02/12/2026 | Checklist final, evidências e roteiro |

## 5. Requisitos oficiais da disciplina

A versão final deve contemplar: menu inicial, Game Over, múltiplas fases, teclado e mouse, personagens animados por spritesheet, música e efeitos sonoros, hi-score persistente, máximo de 100 MB de RAM e instalador Windows. O projeto deve ser desenvolvido em C com Allegro e manter mecânicas que contribuam para a aprendizagem.

Marcos acadêmicos registrados:
- One Sheet — 09/09/2026
- Projeto de Software + Kanban — 23/09/2026
- PoC — 07/10/2026
- MVP — 04/11/2026
- Entrega final + apresentação — 02/12/2026

M3.5 e M5 são marcos internos, não entregas oficiais.

## 6. Estrutura técnica consultada

```text
src/
├── main.c
├── core/
│   ├── game.c
│   └── game.h
├── entities/
│   ├── entities.c
│   └── entities.h
├── gameplay/
│   ├── gameplay.c
│   └── gameplay.h
├── scenes/
│   └── scenes.h
└── systems/

assets/
docs/
Nanobo.sln
Nanobo.vcxproj
```

**Observação técnica:** o README registra quais arquivos entram efetivamente na compilação do Visual Studio. A existência de diretórios/módulos não comprova que estejam integrados ao build. Confirmar `Nanobo.vcxproj` antes de declarar modularização concluída.

## 7. Fontes vivas e histórico

- [README — estado corrente](../../README.md)
- [GDD](../game-design/gdd.md)
- [Decisões de Game Design](../game-design/decisoes.md)
- [Ensino e aprendizagem](../game-design/ensino-e-aprendizagem.md)
- [Conteúdo biológico](../game-design/conteudo-biologico.md)
- [Escopo](../game-design/escopo.md)
- [Scanner](../game-design/scanner.md)
- [Milestones](milestones.md)
- [Kanban](kanban.md)
- [QA da PoC](qa-poc.md)
- [QA do MVP](qa-mvp.md)
- [QA da máquina de estados](qa-maquina-estados.md)
- [QA de recursos](qa-recursos.md)
- [Execução das correções da auditoria](../auditoria/execucao-correcoes-2026-10.md)

## 8. Limitações desta atualização

- Esta é uma leitura documental e de árvore de arquivos do repositório; não executei o jogo nem compilei o projeto neste ambiente.
- Os resultados de QA são tratados como registros da equipe, não como execução independente nesta revisão.
- O estado operacional de Issues, milestone, responsáveis e GitHub Project deve ser conferido na interface/Issues; não inferi status apenas a partir da existência de arquivos.
- Divergências entre GDD, decisões recentes, código e QA devem ser resolvidas pela dupla e registradas; este documento não transforma pendências em decisões.

**Confiança geral:** 0,88 para o resumo do estado documentado; menor para o estado efetivamente executado em runtime, pois não houve build/teste independente.

## 9. Correção de 03/11/2026 (noite)

A leitura de 03/11 dizia que o fluxo tem a cena `BRIEFING`; ela **não existe** no enum — `src/scenes/scenes.h` tem `MENU`, `GAME`, `PAUSE`, `RESULTS`, `VICTORY`, `GAME_OVER` e `EXIT`, e o menu vai direto para `GAME`. O briefing continua sendo o que o M4 precisa entregar (#103/#106).

A mesma rodada renomeou as armas (**Ruptura** e **Dissolução**), fixou a coloração em **roxo/vermelho**, definiu a camada 1 com coloração + nome do agente e **descartou o clique no ícone do HUD** — a troca de arma é pela roda e por `1`/`2`. O texto de 03/11 acima, que fala em "módulos A/B" e "rosa", é anterior a essas decisões e fica como histórico.
