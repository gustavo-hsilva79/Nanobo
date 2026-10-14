# Guia: mover código do `main.c` para os módulos

**Para quem é:** o próprio autor do jogo, mexendo no Visual Studio.  
**Estado:** os **três passos estão feitos**. `entities.c` e `gameplay.c` estão no `<ClCompile>` de `Nanobo.vcxproj` e já contêm o código, e o passo 3 (auditoria do que sobrou no `main.c`) foi conferido em 12/10/2026 (commit `777b2df`).

## Regra de ouro

Cada coisa tem **uma casa só**:

| Casa | O que mora ali |
|---|---|
| `src/main.c` | loop principal, timer/delta time, recursos (`load_resources`/`free_resources`), entrada (teclado e mouse), desenho de cada cena |
| `src/entities/` | o que **existe** no mundo: Nanobô, agentes, projétil e os dados de cada um |
| `src/gameplay/` | o que o jogo **decide**: alvo do scanner, dano, vida, cooldown, pontuação, fim de fase |
| `src/core/` | máquina de estados (já está pronto) |
| `src/scenes/` | só o `enum` das cenas |

Uma coisa que ajuda: **código novo do M3.5 nasce direto no módulo**. Agentes, colisão, vida, dano e cooldown são exatamente o conteúdo de `entities.c` e `gameplay.c`. Escrever no lugar certo desde o começo é mais fácil do que mover depois.

## Antes de começar

Rode o jogo uma vez e confirme que está funcionando (teclado, scanner, menu). Assim, se algo quebrar no meio, você sabe que foi a mudança.

Guarde o estado atual com um commit simples antes de mexer:

```
git add .
git commit -m "separei os arquivos do codigo e arrumei a documentacao"
```

## Passo 1 — entidades (feito)

**O que sai do `main.c` para `entities.h` / `entities.c`:**

| Símbolo no `main.c` | Vai para |
|---|---|
| `#define NUM_AGENTES 3` | `entities.h` |
| `typedef struct { ... } Agent;` | `entities.h` |
| `Agent agents[NUM_AGENTES] = { ... };` | `entities.c` |
| `int identified_count = 0;` | `entities.c` |
| `float player_x`, `player_y`, `player_speed` | `entities.c` |

**Como fazer, na prática:**

1. Abra `src/entities/entities.h` e cole a `struct Agent` e o `#define NUM_AGENTES` **completos**, entre o `#define NANOBO_ENTITIES_H` e o `#endif`.
2. Para cada variável que você vai levar, escreva no `entities.h` uma linha `extern`, por exemplo:
   ```c
   extern Agent agents[NUM_AGENTES];
   extern int identified_count;
   extern float player_x;
   extern float player_y;
   extern float player_speed;
   ```
   O `extern` só avisa ao compilador que a variável existe em outro arquivo.
3. Abra `src/entities/entities.c` e cole ali as **definições** (as linhas com valor, sem `extern`): `Agent agents[NUM_AGENTES] = { ... };`, `int identified_count = 0;` e as três `float` do jogador.
4. No `main.c`, **apague** tudo isso (menos o `#include "entities/entities.h"`, que você vai adicionar junto dos outros includes).
5. Compile e rode. O jogo tem que se comportar exatamente igual.

**Dica:** se aparecer erro `unresolved external symbol`, é sinal de que você declarou `extern` no `.h` mas esqueceu de colar a definição no `.c`. Se aparecer `redefinition`, colou a variável nos dois lugares.

## Passo 2 — regras de gameplay (feito)

**O que sai do `main.c` para `gameplay.h` / `gameplay.c`:**

| Símbolo no `main.c` | Vai para |
|---|---|
| `#define SCAN_RANGE 130` | `gameplay.h` |
| `#define MAX_LAYERS 2` | `gameplay.h` |
| `const char* layer_one_text(int kind)` | `gameplay.c` |
| `const char* layer_two_text(int kind)` | `gameplay.c` |
| `int pick_target(float x, float y)` | `gameplay.c` |
| `void use_scanner()` | `gameplay.c` |
| `void reset_match()` | `gameplay.c` |
| `void update_match(double dt)` | `gameplay.c`, com a assinatura `update_match(double dt, float screen_w, float screen_h)` |
| `double match_time` | `gameplay.c` |
| `bool key_up`, `key_down`, `key_left`, `key_right` | `gameplay.c`, o `main` escreve e o update lê |

**Como fazer:**

1. Copie as assinaturas das funções para o `gameplay.h`, **terminando com `;`** e sem o corpo:
   ```c
   void use_scanner();
   void reset_match();
   void update_match(double dt, float screen_w, float screen_h);
   int pick_target(float x, float y);
   const char* layer_one_text(int kind);
   const char* layer_two_text(int kind);
   ```
2. Copie as funções **inteiras** (com corpo) para o `gameplay.c`, abaixo do `#include "gameplay.h"`.
3. Se alguma função usa algo que ficou no `main.c` (por exemplo `player_x`), confira se o `gameplay.c` está incluindo o `entities.h`: `#include "../entities/entities.h"`.
4. Apague as funções do `main.c` e adicione `#include "gameplay/gameplay.h"` no topo.
5. Compile e rode de novo.

**Duas coisas ficaram um pouco diferentes do previsto:**

- `update_match` recebe o tamanho da tela (`screen_w`, `screen_h`) porque quem conhece o tamanho da janela é o `main.c`, dono do `WIDTH`/`HEIGHT`. O `gameplay.c` usa os dois só para o Nanobô não sair da tela: no `main.c` a chamada ficou `update_match(deltaTime, WIDTH, HEIGHT);`.
- as quatro teclas seguradas foram morar junto com o `match_time`, no `gameplay.c`: quem lê é o `update_match`, quem zera é o `reset_match`, e o `main.c` continua escrevendo nelas nos handlers de entrada.

## Passo 3 — conferir o que ficou parado (feito)

No `main.c` deve sobrar apenas isto:

- `#define WIDTH` / `HEIGHT` e as constantes do botão;
- `load_resources` e `free_resources`;
- as funções de desenho (`draw_menu`, `draw_game`, `draw_message`, `draw_scan_info`, `render`);
- a entrada (`handle_key_down`, `handle_key_up`, `handle_mouse_click`);
- o `main()` com o loop, o timer e o `delta_time`.

Se sobrou mais que isso, provavelmente ainda tem código esperando um lugar — e o lugar é `entities.c` ou `gameplay.c`.

**Auditoria de 12/10/2026** — achou quatro sobras, todas corrigidas:

| Sobra | Por que estava errada | Como ficou |
|---|---|---|
| `float mouse_x`, `float mouse_y` e o `case ALLEGRO_EVENT_MOUSE_AXES` | Estado escrito a cada evento e **nunca lido**: código morto que só existia para se alimentar | Removidos; o clique continua recebendo as coordenadas direto do evento |
| `//ALLEGRO_MOUSE mouse;` | Linha comentada, sem função | Removida |
| `agents[i].x - 16` no `draw_game` | O tamanho do agente é **dado da entidade**, não um literal solto no desenho (e a #10 vai usar o mesmo número na colisão) | `AGENT_RADIUS`, definido no `entities.h` ao lado de `PLAYER_RADIUS` |
| `320` fixo no `draw_message` | Era metade da largura só por coincidência, enquanto o resto do arquivo usa `WIDTH / 2` | `WIDTH / 2` |

**Ficou de propósito:** as teclas de teste do fluxo (`F`, `G`, `V`), que só saem quando as condições reais de fim de fase existirem (#26/#18), e as duas linhas temporárias do HUD: `direcao: N`, que sai quando o disparo (#63) existir, e `encostado no agente N`, que sai quando a vida (#11) mostrar o dano.

## Depois de cada passo

Conferido em 11/10/2026: depois do passo 2 o projeto compila em `Debug|x64` sem erro e sem aviso, e o jogo abre, anda e escaneia igual a antes.

Rode exatamente o que está em [qa-poc.md](qa-poc.md) e confira: `↑` `↓` `←` `→` e `W` `A` `S` `D` movem o Nanobô, ele para ao soltar, não sai da tela, `ESC` volta ao menu, `SPACE` perto de um agente mostra as duas camadas e não identifica o mesmo agente duas vezes. Se algo disso quebrar, desfaça só o último passo — por isso um passo por vez.

## Como commitar

Um commit simples, no fim, com tudo junto:

```
git add .
git commit -m "passei os agentes e as regras de gameplay pros arquivos novos"
```

Mensagem curta, em português, minúscula, sem `feat:`, sem número de issue e sem ponto no fim. Vale a pena conferir `git status` antes do `git add .` para não levar junto arquivo de build.

Não faça `push` sem combinar com o Eric: ele pode ter trabalho local em cima do `main.c`, e aí o conflito vira problema dos dois.
