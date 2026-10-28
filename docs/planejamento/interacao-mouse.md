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

- O retângulo do botão do menu passou a **240..400 × 208..236** na tela lógica (`BUTTON_X/Y/W/H` em `src/main.c`); os números da §1 são o registro da revisão anterior.
- Desenho e clique leem a mesma lista (`screen_buttons`), então a área clicável é sempre a área desenhada.
- A ativação continua acontecendo no **pressionar** (down), preservando o passo 4 da §3; ainda não existe estado visual de "apertado" porque ele não chega a ser desenhado.
- Hover, resize e `F11` usam uma conversão única (`to_logical_x/y`) e o cursor é guardado em pixel da janela, relido no resize e no fullscreen.
- Continua valendo: clique fora de qualquer botão não produz efeito; no estado `GAME` nenhum clique tem ação.
