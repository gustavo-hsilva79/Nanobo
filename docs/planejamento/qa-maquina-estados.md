# Nanobô — Validação da infraestrutura da máquina de estados (#102)

> **Última revisão:** 07/10/2026.
>
> **Tarefas cobertas:** #93 (transições), #94 (entrada por estado), #95 (atualização suspensa fora de `GAME`), #96 (render por estado).
>
> **Fora do escopo:** a validação integrada do fluxo de telas e fases pertence à #76, e a integração das condições reais de fim de fase pertence a #20/#68–#75.

## 1. Ambiente do teste

| Item | Valor |
|---|---|
| Sistema | Windows |
| Ferramenta | Visual Studio, toolset v143, plataforma x64 |
| Build | `Debug\|x64` e `Release\|x64` via MSBuild sobre `Nanobo.vcxproj` |
| Executável | `x64\Debug\Nanobo.exe` |
| Janela | 640×480, fechável pela moldura |

## 2. Os seis estados

| Estado | Entrada | Renderiza | Entrada de teclado | Entrada de mouse |
|---|---|---|---|---|
| `MENU` | inicialização | título e botão `INICIAR` | `ENTER` inicia partida | clique no botão inicia partida |
| `GAME` | nova partida; retomada de `PAUSE`; próxima fase vinda de `RESULTS` | gameplay ativo + HUD + painel do scanner | `SPACE` scanner, `ESC` pausa | nenhuma |
| `PAUSE` | `ESC` durante `GAME` | apenas o aviso de pausa | `ESC` retoma | nenhuma |
| `RESULTS` | conclusão de fase em `GAME` | apenas o aviso de fase concluída | `ENTER` próxima fase, `V` vitória | nenhuma |
| `VICTORY` | última fase concluída em `RESULTS` | apenas o aviso de vitória | `ENTER` nova partida | nenhuma |
| `GAME OVER` | derrota em `GAME` | apenas o aviso de derrota | `ENTER` nova partida | nenhuma |

## 3. Transições

A tabela abaixo é a regra implementada em `game_set_scene()`. Qualquer par fora dela é rejeitado e o estado atual permanece o mesmo.

| De | Para permitidos | Para rejeitados |
|---|---|---|
| `MENU` | `GAME` | `PAUSE`, `RESULTS`, `VICTORY`, `GAME OVER` |
| `GAME` | `PAUSE`, `RESULTS`, `GAME OVER` | `MENU`, `VICTORY` |
| `PAUSE` | `GAME` | `MENU`, `RESULTS`, `VICTORY`, `GAME OVER` |
| `RESULTS` | `GAME`, `VICTORY` | `MENU`, `PAUSE`, `GAME OVER` |
| `VICTORY` | `GAME` | `MENU`, `PAUSE`, `RESULTS`, `GAME OVER` |
| `GAME OVER` | `GAME` | `MENU`, `PAUSE`, `RESULTS`, `VICTORY` |

Regras adicionais:

- Transição para o **mesmo** estado é ignorada.
- Valor fora da enumeração é rejeitado.

### Centralização

O estado atual é `currentScene`, variável estática de `src/core/game.c`. A única atribuição a essa variável está dentro de `game_set_scene()`, e todo o resto do programa lê ou solicita mudança por `game_get_scene()` e `game_set_scene()`. Não existe um segundo caminho de mudança de estado.

## 4. Roteiro reproduzível

1. Compilar `Debug|x64` e `Release|x64`.
2. Executar `x64\Debug\Nanobo.exe` a partir da raiz do repositório.
3. Confirmar a tela de `MENU` com o botão `INICIAR`.
4. Pressionar `ENTER` e confirmar a entrada em `GAME`, com HUD e os três agentes.
5. Com o cursor longe de todos os agentes, pressionar `SPACE` duas vezes.
6. Mover o cursor até um agente dentro do alcance e pressionar `SPACE` duas vezes.
7. Repetir o passo 6 no mesmo agente mais duas vezes.
8. Mover o cursor até um segundo agente e pressionar `SPACE` duas vezes.
9. Pressionar `ESC` e aguardar 4 segundos.
10. Pressionar `ESC` e comparar o tempo de partida do HUD.
11. Pressionar `F` e confirmar `RESULTS`.
12. Pressionar `V` e confirmar `VICTORY`.
13. Pressionar `ENTER` e confirmar nova partida em `GAME`, com contador e tempo zerados.
14. Fechar a janela pela moldura.

## 5. Resultado observado

| Passo | Resultado observado |
|---|---|
| 1 | `Debug\|x64` e `Release\|x64` compilam sem erro e sem aviso originado pelo código novo |
| 3 | `MENU` desenha título, subtítulo, botão `INICIAR` e a dica de interação |
| 4 | `GAME` desenha os três agentes, o Nanobô, o HUD (`identificados: 0/3`) e o tempo de partida |
| 5 | contador permanece `0/3` e nenhuma camada é registrada — ativação sem alvo válido não consome camada |
| 6 | contador vai a `1/3` e aparecem as duas camadas do agente |
| 7 | contador permanece `1/3` e nenhuma camada nova aparece |
| 8 | contador vai a `2/3` |
| 9–10 | tempo marcava `2,4` antes da pausa e `4,3` depois; os 4 segundos parados não avançaram o tempo da partida |
| 11 | `RESULTS` desenha apenas o aviso de fase concluída, sem elementos de gameplay |
| 12 | `VICTORY` desenha apenas o aviso de vitória |
| 13 | `GAME` reinicia com `identificados: 0/3` e tempo zerado |
| 14 | o processo encerra sem mensagem de erro |

A verificação foi visual (captura da área do cliente da janela) e por inspeção do código para a tabela de transições. O projeto não possui suíte de testes automatizados, e nenhuma foi criada nesta revisão.

## 6. Limitações conhecidas

- As condições reais de conclusão de fase e de derrota ainda não existem. Os estados `RESULTS`, `VICTORY` e `GAME OVER` são alcançados por **teclas de teste** (`F` e `G`, e `V` dentro de `RESULTS`), que devem sair quando a #20 ligar as condições de verdade.
- O passo 4 do roteiro de mouse da #23 (clique repetido) depende de um retorno ao `MENU` que ainda não existe (#20).
- O Nanobô ainda segue o cursor; a movimentação própria é a #9.

## 7. Falhas encontradas

Nenhuma falha reproduzível foi encontrada na infraestrutura da máquina de estados nesta revisão. As limitações da seção 6 são escopo de outras Issues, não defeitos desta infraestrutura.
