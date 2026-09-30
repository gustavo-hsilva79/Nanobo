# Nanobô — Estados e transições principais

Este documento define o fluxo geral da máquina de estados previsto em #7/#98. As transições específicas entre telas e fases serão integradas pela #20 e suas subtarefas.

## Estados

| Estado | Entrada | Saída permitida |
|---|---|---|
| `MENU` | Inicialização do jogo ou retorno por reinício, quando essa opção for integrada | Iniciar uma nova partida → `GAME` |
| `GAME` | Nova partida ou retomada de `PAUSE`; avanço para a próxima fase vindo de `RESULTS` | Solicitar pausa → `PAUSE`; concluir fase → `RESULTS`; perder toda a vida → `GAME OVER` |
| `PAUSE` | Solicitação de pausa durante `GAME` | Retomar → `GAME` |
| `RESULTS` | Conclusão de uma fase em `GAME` | Há próxima fase → `GAME`; última fase concluída → `VICTORY` |
| `VICTORY` | Conclusão da última fase em `RESULTS` | Iniciar nova partida → `GAME` |
| `GAME OVER` | Condição de derrota durante `GAME` | Iniciar nova partida → `GAME` |

## Regras do fluxo

- `MENU` é o estado inicial.
- A pausa só pode ser solicitada durante `GAME`; retomar restaura `GAME` sem reiniciar a partida.
- A conclusão de uma fase leva a `RESULTS`. A tela de resultados escolhe entre iniciar a próxima fase ou concluir a execução em `VICTORY`.
- `GAME OVER` ocorre quando a condição de derrota da fase é atingida.
- Uma nova partida iniciada em `VICTORY` ou `GAME OVER` reinicializa vida, pontuação da execução, agentes e demais dados transitórios, conforme #75. O hi-score persistente não é apagado.
- Estados terminais e `PAUSE` não atualizam a lógica da partida; apenas `GAME` executa a atualização do gameplay.
- Fechar a janela encerra o processo, sem exigir uma transição para outro estado.

## Fora desta definição

Esta máquina não define conteúdo das telas, regras de vitória/derrota, dados apresentados em `RESULTS` ou a integração da progressão. Essas responsabilidades permanecem nas Issues de interface e fluxo da #20.
