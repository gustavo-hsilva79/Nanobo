# Nanobô — Estados e transições principais

Este documento define o fluxo geral da máquina de estados previsto em #7/#98. As transições específicas entre telas e fases serão integradas pela #20 e suas subtarefas.

## Estados

| Estado | Entrada | Saída permitida |
|---|---|---|
| `MENU` | Inicialização do jogo ou retorno por reinício, quando essa opção for integrada | Iniciar uma nova partida → `BRIEFING` |
| `BRIEFING` | Início de partida vindo de `MENU`, `VICTORY` ou `GAME OVER` (**M4, #103/#106**) | Pular ou terminar os 3 cartões → `GAME` |
| `GAME` | Fim do briefing, retomada de `PAUSE` ou avanço vindo de `RESULTS` | Solicitar pausa → `PAUSE`; concluir fase → `RESULTS`; perder toda a vida → `GAME OVER` |
| `PAUSE` | Solicitação de pausa durante `GAME` | Retomar → `GAME` |
| `RESULTS` | Conclusão de uma fase em `GAME` | Há próxima fase → `GAME`; última fase concluída → `VICTORY` |
| `VICTORY` | Conclusão da última fase em `RESULTS` | Iniciar nova partida → `BRIEFING` |
| `GAME OVER` | Condição de derrota durante `GAME` | Iniciar nova partida → `BRIEFING` |

## Regras do fluxo

- `MENU` é o estado inicial.
- A pausa só pode ser solicitada durante `GAME`; retomar restaura `GAME` sem reiniciar a partida.
- A conclusão de uma fase leva a `RESULTS`. A tela de resultados escolhe entre iniciar a próxima fase ou concluir a execução em `VICTORY`.
- `GAME OVER` ocorre quando a condição de derrota da fase é atingida.
- Uma nova partida iniciada em `VICTORY` ou `GAME OVER` reinicializa vida, pontuação da execução, agentes e demais dados transitórios, conforme #75, e passa pelo `BRIEFING` antes da fase. O hi-score persistente não é apagado.
- Estados terminais e `PAUSE` não atualizam a lógica da partida; apenas `GAME` executa a atualização do gameplay.
- Fechar a janela encerra o processo, sem exigir uma transição para outro estado.

## Implementação atual (04/11/2026)

`src/core/game.c` implementa as transições da tabela, `BRIEFING` incluído. O enum `GameScene` (`src/scenes/scenes.h`) tem `MENU`, `BRIEFING`, `GAME`, `PAUSE`, `RESULTS`, `VICTORY`, `GAME_OVER` e `EXIT`, e qualquer par fora da tabela é ignorado pela máquina de estados. `EXIT` existe no enum, mas `game_set_scene()` só aceita cenas até `GAME_OVER`; fechar a janela encerra o processo.

- `MENU → BRIEFING → GAME` é o caminho de toda partida; `VICTORY → BRIEFING` e `GAME OVER → BRIEFING` fazem a partida nova passar pelo briefing outra vez.
- No briefing, `ENTER`, `→` ou clique avançam o cartão; no terceiro, `SEGUIR` entra na fase. `ESC` e `PULAR` vão direto para `GAME`. O cartão corrente volta a zero a cada entrada na cena.
- `GAME OVER` é alcançado de verdade quando `player.health == 0`.
- `RESULTS` é alcançado automaticamente quando a fase conclui (6 ameaças neutralizadas + Nanobô no destino, #26/#70); `VICTORY` vem de `RESULTS` por `ENTER`.
- **Decisão de 03/11/2026 (D2.1, `decisoes.md`):** a conclusão passa a exigir também a identificação dos agentes. Até a mudança ser implementada, a regra em vigor é a de #26/#70 acima; D2.1 e D2.2 ficam para o M5.
- `VICTORY` e `GAME OVER` iniciam nova partida com reset completo (#74/#75) e briefing.
- `F`, `G` e `H` são teclas de QA e existem somente no build Debug; o Release não tem atalho de estado (#76).
- Perder o foco da janela durante `GAME` entra em `PAUSE`; retomar é `ESC`.
- Não existe caminho de volta para `MENU`; nova partida em `VICTORY`/`GAME OVER` vai para `BRIEFING` e depois `GAME`.
- `PAUSE` congela os timers porque `update_match()` só roda em `GAME`.

## Fora desta definição

Esta máquina não define conteúdo das telas, regras de vitória/derrota, dados apresentados em `RESULTS` ou a integração da progressão. Essas responsabilidades permanecem nas Issues de interface e fluxo da #20.
