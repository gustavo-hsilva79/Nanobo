# Nanobô — Interação de mouse (#23)

> **Última revisão:** 07/10/2026.
>
> **Requisito atendido:** RF03 — permitir interação por mouse (`docs/planejamento/projeto-de-software.md`).

A #23 pedia que a interação de mouse fosse escolhida pela dupla antes da implementação, preferencialmente em uma ação de interface **já prevista**, para não criar mecânica artificial só para cumprir requisito.

## 1. Interação definida

**Clique com o botão esquerdo sobre o botão `INICIAR` na tela de `MENU` inicia a partida** (`MENU → GAME`).

- A ação já estava prevista pelo fluxo de telas: `MENU → GAME` é a transição de entrada do menu inicial (#20, #50).
- A área clicável é o retângulo do botão: `x` de 220 a 420 e `y` de 300 a 350 na tela de 640×480.
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
- Seleção de raio/ataque por mouse (#16).
- Navegação de menu por outros dispositivos.

## 5. Observação sobre a posição do mouse

Enquanto a movimentação própria do Nanobô (#9) não existe, o sprite do Nanobô é desenhado na posição atual do cursor, e é essa posição que o scanner usa para medir distância até os agentes. Isso é comportamento provisório herdado do trabalho anterior no `src/main.c` e **não** faz parte da interação de interface definida nesta Issue.

## 6. Implementação atual

| Item | Onde |
|---|---|
| Área do botão | `BUTTON_X`, `BUTTON_Y`, `BUTTON_W`, `BUTTON_H` em `src/main.c` |
| Desenho do botão | `draw_menu()` em `src/main.c` |
| Tratamento do clique | `handle_mouse_click()` em `src/main.c` |
| Leitura do evento | caso `ALLEGRO_EVENT_MOUSE_BUTTON_DOWN` em `main()` |
