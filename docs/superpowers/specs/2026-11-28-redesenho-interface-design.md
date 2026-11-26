# Redesenho da interface (#245) e arte da interface (#246) — design

> Decisões do dono em 26/11/2026: [`docs/game-design/decisoes.md`](../../game-design/decisoes.md) §"Interface (26/11/2026)". Termos: [`glossario.md`](../../game-design/glossario.md) §"Interface". Este documento traz o **como**: telas, medidas, estados e arte. Os textos finais vêm do inventário "atual × proposto" aprovado pelo dono.

## Por quê

As telas fora do Diário ainda são painéis sobre fundo preto que pouco explicam: o menu espreme o "como jogar" num painel, o HUD está a 20 px do limite na linha 0 e mistura o objetivo da fase com as barras, a mira é desenhada em código e a Vitória e o Game Over não contam a história do Léo. O objetivo é que cada tela explique o que mostra, com a voz do Diário, e tenha o mesmo cuidado visual das fases.

**Fora do escopo:** troca de fonte (a embutida 8×8 fica), menu para reler o Diário, o Diário em si (caixa de diálogo, cards, interlúdios) e qualquer mudança de regra de jogo.

## Abordagens consideradas

| Ponto | Escolhido | Alternativas descartadas |
|---|---|---|
| "Como jogar" | cena nova `GAME_SCENE_HOWTO`, aberta do menu e da Pausa | sobreposição no menu como os RECORDES (não abre da Pausa sem duplicar código); painel no menu (sem espaço) |
| Espaço do HUD | coluna de vida à esquerda, HUD com 40 px | HUD de 48 px (mexe em `PLAY_H`, usado por regras e harness); barras flutuando no mundo (competem com o jogo) |
| Objetivo da fase | aviso de 4 s + texto na Pausa | faixa fixa no mundo (ocupa a tela o tempo todo); só na Pausa (some do jogo) |
| Arte | script Lua no Aseprite, aprovação no chat | desenho à mão (sem tempo até 29/11) |
| Fundos | 4 temas (menu, como jogar, hospital × 3 luzes) | um por tela (~9 imagens); só o menu |

## Telas

Tela lógica 640×360, fonte de 8 px por caractere, sem maiúscula acentuada, botões em caixa alta ASCII. Os painéis continuam com `draw_panel` (`UI_INK`/`UI_PANEL` + `UI_FRAME`) por cima dos fundos.

### Menu
- Fundo `fundo_menu.png`; por cima, `hemacias_deriva.png` deslizando para a esquerda a ~8 px/s, desenhada duas vezes lado a lado para emendar; o Nanobô do spritesheet (linha 1, frente, quadros parados) à direita do título, oscilando ±2 px num seno de ~2 s.
- Título "NANOBÔ" (bitmap ×3) e subtítulo como hoje; botões **INICIAR**, **COMO JOGAR**, **RECORDES**; linha do recorde e "F11 tela cheia" ficam.
- Saem do menu os painéis "Como jogar" e "Objetivo da missão" (o conteúdo vai para a cena nova).

### Como jogar (`GAME_SCENE_HOWTO`)
- Fundo `fundo_como_jogar.png`; um painel central com uma coluna de ícone à esquerda (sprites do jogo ampliados ×2: Nanobô, scanner, ícones das ferramentas, ícones de vida) e até 6 linhas de texto.
- 4 páginas: mover e mirar; ler com o scanner; ferramentas; Integridade e Vitais. Rodapé `n/4` e a dica de teclas.
- Enter, Espaço ou clique avançam; na última página voltam; Esc volta a qualquer momento. Setas ←/→ andam entre páginas.
- **Transições novas:** `MENU→HOWTO`, `HOWTO→MENU`, `PAUSE→HOWTO`, `HOWTO→PAUSE`. A cena lembra a origem (`game.c`), e ao voltar entra nela; vinda da Pausa, a partida continua parada (só `GAME` avança a lógica). A página volta à 1 a cada entrada.

### HUD (40 px, `PLAY_H` 320 sem mudança)

| Linha | Esquerda (x 8) | Meio | Direita (até x 632) |
|---|---|---|---|
| 0 (y 328) | ícone do Nanobô 16×16 + barra de bateria 64×8 (Integridade) | livre | ferramenta · ícone · recarga · estado (como hoje) |
| 1 (y 344) | coração 16×16 pulsando + barra de ECG 64×8 (Vitais) | `pontos N · mm:ss` a partir de x 100 | `ameaças x/y · identificados a/b` |

- **Medidas:** coluna de vida = 16 + 4 + 64 = 84 px (x 8 a 92). Pior caso do bloco direito da linha 0 = `Neutralização · vírus livre` (216) + 4 + ícone 16 + 4 + recarga 38 + 6 + `recarga` (56) = 340 px, de x 292 a 632: sobram ~200 px. Linha 1: `pontos 9999 · 99:59` (19 car., 152 px) de x 100 a 252; contador à direita com 35 car. (280 px) de x 352: sobram ~100 px.
- **Barras:** 10 segmentos, preenchidos por `ceil(valor/10)`; a moldura vem do sprite e cada segmento é o `segmento.png` tingido. Integridade em `UI_CYAN`; Vitais em `UI_GREEN` ≥ `VITALS_MIN_BETWEEN`, `UI_AMBER` ≥ `VITALS_LOW`, `UI_RED` abaixo (as faixas atuais).
- **Coração:** quadros 0→1→2→0; período de 1,0 s com vitais cheios, caindo linearmente até 0,4 s com vitais em 0.
- O objetivo âmbar sai da linha 1.

### Aviso de objetivo
- Caixa centrada no topo do mundo (y 8), no estilo do aviso de feedback (placa `UI_INK`, borda âmbar, texto `UI_AMBER`), visível 4 s com esmaecer no último 0,5 s.
- Dispara: (1) no primeiro quadro da fase; (2) quando `threats_left()` chega a 0 (meta vira "vá ao destino"). Um aviso novo substitui o anterior. Não aparece com caixa do Diário aberta: espera ela fechar e então conta os 4 s.
- Com a ficha do scanner no topo, o aviso desce para baixo dela (mesma regra do feedback).

### Pausa
- Fundo: o mapa da fase atual (quadro congelado como hoje) com véu mais escuro; painel maior com título, o objetivo completo da fase, um lembrete curto das teclas e os botões **CONTINUAR** e **COMO JOGAR**. Esc continua voltando ao jogo.

### Ficha do scanner (624×50, posição atual)
- Coluna da esquerda (x 12 a 40): retrato 24×24 do agente (o próprio bitmap escalado; `silhueta` antes da camada 1; para fungos sem leitura, a silhueta como no mundo) em y+5, e 2 marcadores 6×6 com 2 px de vão em y+34, um por camada de leitura (a regra continua com 2 camadas, decisão do dono em 26/11): 1º = forma, 2º = Gram e espécie (em vírus, célula e fungo, "estrutura"). Apagado `UI_SLOT`, aceso `UI_AMBER`; o 2º acende verde na identificação.
- O texto passa de x 36 para x 44: **72 caracteres** por linha (era 74). O ícone do scanner sai da ficha.

### Resultados
- Fundo: o mapa da fase recém-concluída escurecido (`MAP_VEIL` mais forte), painéis como hoje; o painel "O que anotei nesta fase" já aprovado em 26/11.

### Abertura, Vitória e Game Over
- Abertura (`BRIEFING`): `hospital_abertura.png` atrás da caixa do Diário (o véu de 60% fica).
- Vitória: `hospital_vitoria.png`; uma linha na voz do Nanobô depois da frase da alta (os painéis descem 16 a 32 px, decisão de 26/11); painéis de estatística, Top 10 e nome nas posições atuais, com o campo de nome e o limite de 10 caracteres preservados.
- Game Over: `hospital_gameover.png`; texto da retirada do Nanobô pela equipe, sem morte, mais uma linha na voz do Nanobô; painel de estatística descendo o necessário.
- Controles no "como jogar" seguem o código (decisão de 26/11): botão direito escaneia, tecla X liga a defesa.

### Mira
- `mira.png` 48×16 (3 quadros 16×16, brancos), desenhada centrada no mouse, só em `GAME` e dentro da área de jogo, tingida com a cor da ferramenta atual.
- Quadro: 2 (travado) se `fire_locked()`; 1 (recarga) se `attack_cooldown_left() > 0`; senão 0 (pronto). Cor por ferramenta: a mesma usada no HUD/tiros para cada `Laser` (definida na implementação, uma tabela de 6 cores).

## Arte (#246)

Gerada por `../Nanobo-testes/gerar_interface.lua` (Aseprite 1.3.18, modo batch), paleta `INK`/`PALE`/`SHADE` + `UI_*`; aprovada pelo dono no chat antes de entrar em `assets/`.

| Arquivo | Tamanho | Uso |
|---|---|---|
| `sprites/icone_nanobo.png` | 16×16 | HUD, Integridade; "como jogar" p. 4 |
| `sprites/coracao_pulso.png` | 48×16 (3 quadros) | HUD, Vitais; "como jogar" p. 4 |
| `sprites/barra_bateria.png`, `sprites/barra_ecg.png` | 64×8 | molduras das barras |
| `sprites/segmento.png` | 5×4 | segmento tingido das duas barras |
| `sprites/mira.png` | 48×16 (3 quadros) | mira |
| `backgrounds/fundo_menu.png` | 640×360 | menu |
| `backgrounds/hemacias_deriva.png` | 640×360, transparente | camada do menu |
| `backgrounds/fundo_como_jogar.png` | 640×360 | como jogar |
| `backgrounds/hospital_abertura.png`, `hospital_vitoria.png`, `hospital_gameover.png` | 640×360 | abertura, Vitória, Game Over |

**Memória:** 6 fundos × ~0,9 MB carregados de uma vez (~5,5 MB) + sprites pequenos. Pico atual 79 MB; limite 100 MB. Se a medição passar de 90 MB, os três do hospital passam a ser carregados sob demanda, como o mapa da fase.

## Textos

Inventário "atual × proposto" por tela (menu, como jogar, HUD, aviso de objetivo, Pausa, ficha do scanner, feedback, Resultados, Vitória, Game Over), publicado numa página de aprovação com ok/comentário por linha. Regras: voz do Diário, frases curtas, um termo por linha onde ensina, fonte em `conteudo-biologico.md` para toda afirmação biológica, larguras medidas por área. Texto aprovado é registrado em `textos-do-jogo.md` com ID e contagem.

## O que fica para quem implementa

- Nomes das funções e estados novos no `main.c` (seguindo o estilo plano do arquivo).
- A tabela de cor por ferramenta da mira.
- O ajuste fino de posições (±2 px) depois das capturas.

## Verificação

Build Debug e Release com 0 avisos; harness verde (com testes novos das transições de cena, se o harness alcançar `game.c`); `conferir_textos.py` 0/0; capturas de cada tela e estado (menu com deriva; 4 páginas; como jogar pela Pausa e volta; HUD com vitais em 3 faixas; mira nos 3 quadros; aviso de objetivo; ficha antes e depois de cada camada; Resultados das 3 fases; Vitória com nome; Game Over pelas duas causas); memória com `medir_memoria.ps1` abaixo de 100 MB.
