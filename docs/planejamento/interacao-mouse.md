# Nanobô — Interação de mouse (#23)

> **Última revisão:** 07/10/2026.
>
> **Requisito atendido:** RF03 — permitir interação por mouse (`docs/planejamento/projeto-de-software.md`).

A #23 pedia que a interação de mouse fosse escolhida pela dupla antes da implementação, preferencialmente em uma ação de interface **já prevista**, para não criar mecânica artificial só para cumprir requisito.

## 1. Interação definida

**Clique com o botão esquerdo sobre o botão `INICIAR` na tela de `MENU` inicia a partida** (`MENU → GAME`).

- A ação já estava prevista pelo fluxo de telas: `MENU → GAME` é a transição de entrada do menu inicial (#20, #50).
- A área clicável é o retângulo do botão: `x` de 220 a 420 e `y` de 200 a 250 na tela lógica de 640×360 (`BUTTON_X/Y/W/H` em `src/main.c`).
- A mesma ação continua disponível por teclado (`ENTER`), que é o caminho já implementado na #94. O clique é uma segunda forma de acionar a mesma transição, não uma mecânica nova.

## 2. Regras

- O clique só é interpretado no estado `MENU`.
- Clique **fora** da área do botão não produz efeito nenhum.
- Nos estados `GAME`, `PAUSE`, `RESULTS`, `VICTORY` e `GAME OVER` nenhum clique tem ação definida nesta revisão.
- O evento de mouse é lido uma única vez no loop de eventos e encaminhado ao estado atual por uma função única (`handle_mouse_click`), para a interface não duplicar o processamento de entrada.
- A interação não substitui comandos de gameplay: ataque, defesa, scanner e movimento continuam definidos pelo Game Design, e nenhum deles é acionado por clique nesta revisão.

## 3. Verificação

| Passo | Resultado esperado |
|---|---|
| 1. Abrir o jogo e clicar sobre o botão `INICIAR` | a partida começa (`GAME`) |
| 2. Voltar ao `MENU` e clicar fora do botão | nada acontece, o jogo continua no `MENU` |
| 3. Clicar durante `GAME`, `PAUSE`, `RESULTS`, `VICTORY` e `GAME OVER` | nenhum efeito |
| 4. Clicar duas vezes seguidas sobre o botão | a ação ocorre uma única vez por clique válido; o segundo clique já acontece em `GAME` e não tem alvo |

O passo 4 depende do fluxo de retorno ao `MENU`, que ainda não existe (#20). Enquanto o menu só é acessado na inicialização, o teste de repetição é feito reiniciando o jogo.

## 4. Fora do escopo desta revisão

- Botões em `PAUSE`, `RESULTS`, `VICTORY` e `GAME OVER` (#51, #52, #53, #57).
- Destaque visual do botão sob o cursor (*hover*).
- Seleção de laser/ataque por mouse (#16).
- Navegação de menu por outros dispositivos.

## 5. Observação sobre a posição do mouse

Até a #9, o sprite do Nanobô era desenhado na posição do cursor e era essa posição que o scanner usava para medir distância até os agentes. Isso era comportamento provisório herdado do trabalho anterior no `src/main.c`.

Com a #9 implementada, o Nanobô tem posição própria (`Nanobo player`, em `src/entities/entities.c`), é o teclado que a altera, e é essa posição que o scanner usa. A interação de interface definida nesta Issue — clique sobre `INICIAR` no `MENU` — não mudou. As coordenadas vêm do próprio evento de clique (`event.mouse.x`/`event.mouse.y`); o caso `ALLEGRO_EVENT_MOUSE_AXES`, que só alimentava estado morto, foi removido em 12/10/2026.

## 6. Implementação atual

| Item | Onde |
|---|---|
| Área do botão | `BUTTON_X`, `BUTTON_Y`, `BUTTON_W`, `BUTTON_H` em `src/main.c` |
| Desenho do botão | `draw_menu()` em `src/main.c` |
| Tratamento do clique | `handle_mouse_click()` em `src/main.c` |
| Leitura do evento | caso `ALLEGRO_EVENT_MOUSE_BUTTON_DOWN` em `main()` |

## 7. Atualização 28/10/2026 — botões em todas as telas

O escopo da §4 mudou com o polimento de menus e HUD: agora **todas as telas não-jogáveis têm botão** (`MENU`, `PAUSE`, `RESULTS`, `VICTORY` e `GAME OVER`), todos com foco de teclado e destaque sob o cursor.

- O retângulo do botão do menu passou a **240..400 × 210..238** na tela lógica (`BUTTON_X/Y/W/H` em `src/main.c`); os números da §1 são o registro da revisão anterior.
- O botão `CONTINUAR` dos resultados fica em **240..400 × 196..224**; `NOVA PARTIDA` da vitória em **240..400 × 212..240** e a do game over em **240..400 × 204..232** (`screen_buttons`).
- Desenho e clique leem a mesma lista (`screen_buttons`), então a área clicável é sempre a área desenhada.
- A ativação continua acontecendo no **pressionar** (down), preservando o passo 4 da §3; ainda não existe estado visual de "apertado" porque ele não chega a ser desenhado.
- Hover, resize e `F11` usam uma conversão única (`to_logical_x/y`) e o cursor é guardado em pixel da janela, relido no resize e no fullscreen.
- Continua valendo: clique fora de qualquer botão não produz efeito; no estado `GAME` nenhum clique tem ação.

## 8. Atualização 03/11/2026 — mouse no jogo (#198)

A §7 registrava que, no estado `GAME`, nenhum clique tinha ação. Esta revisão muda isso: o mouse passa a mirar, atirar, escanear e trocar o módulo dentro da fase; as regras de interface das §1–§7 continuam valendo fora do jogo.

| Item | Onde |
|---|---|
| Clique esquerdo atira na direção do cursor | `handle_game_click()` em `src/main.c` |
| Clique direito escaneia o agente sob o cursor | `scan_at()` em `src/gameplay/gameplay.c` |
| Roda do mouse e `1`/`2` trocam o módulo | `toggle_module()` e `handle_key_down()` |
| Janela → tela lógica → mundo | `to_logical_x/y` + `camera` |
| Cursor escondido na fase e mira desenhada | `render()` e `draw_game()` |

- A tolerância do scan é de 24 px entre o cursor e o centro do agente (`CURSOR_SCAN_TOLERANCE`); o alcance de 130 px continua medido do Nanobô (`SCAN_RANGE`).
- Clique com y lógico ≥ `PLAY_H` (painel do HUD) não dispara nem escaneia.
- `ESPAÇO` e `J` deixam de existir no gameplay: ataque e scan são só do mouse.
- Cursor a menos de 8 px do Nanobô cai na direção do `facing` (`AIM_DEAD_ZONE`).
- Clique no ícone do módulo no HUD **não** foi implementado nesta revisão e foi **descartado** em 03/11 (noite): a troca é pela roda e por `1`/`2`. O `handle_game_click()` retorna antes de olhar o painel (`ly >= PLAY_H`), e essa é a regra.

### 8.1 Verificação executada (03/11/2026)

Debug x64, janela maximizada (cliente 1920×1080, escala inteira 3), capturas do cliente com foco, clique e roda sintéticos. Evidências em `docs/auditoria/evidencias/qa-m4-01-menu-controles.png` e `qa-m4-02-hud-modulo-mira.png`.

| Passo | Esperado | Observado |
|---|---|---|
| 1. Entrar na fase e mover o cursor | a mira acompanha o cursor; o cursor do sistema some | **[OK]** mira ciano desenhada no ponto do cursor; o cursor do sistema não entra na captura, então "sumiu" não é verificável por imagem — a mira ocupa o lugar dele |
| 2. Clique direito sobre uma bactéria a até 130 px | o painel mostra a leitura daquele alvo | **[PEND]** a tentativa com o agente em movimento não leu (ele saiu dos 24 px entre a captura e o clique); o caso fica para o QA manual da #107, com alvo parado |
| 3. Clique esquerdo longe do Nanobô | o projétil sai na direção do clique | **[OK]** o HUD entra em `recarga` e a barra cai para 3/4; o projétil não aparece na captura (sai do quadro em menos de um segundo) |
| 4. Roda do mouse e `1`/`2` | a letra do módulo no HUD troca entre A e B | **[OK]** roda: `A` → `B` com o ícone vermelho (qa-m4-02); `1`/`2` não verificado em runtime — teclado injetado não chega à janela nesta sessão |
| 5. Clique no painel do HUD | nada é disparado | **[PEND]** não executado |
| 6. `ESC` e `ESC` | pausa com o cursor de volta; volta ao jogo com a mira | **[PEND]** depende de teclado, que não chega por injeção nesta sessão |

**Limite deste registro:** teclado sintético (`SendInput` e `PostMessage`) não chegou à janela do Allegro aqui; tudo que depende de tecla — `1`/`2`, `ESC`, o pulo do Game Over para a fase e a tecla `F` de QA — ficou para o QA manual da #107. O menu e a HUD que a captura mostra são evidência de desenho, não de entrada.
