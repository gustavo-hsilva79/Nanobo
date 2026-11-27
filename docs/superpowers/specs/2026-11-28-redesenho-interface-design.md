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
| Ordem de construção | **duas ondas** (escolha do dono em 26/11): 1ª o que aparece durante a fase, 2ª as telas fora dela | uma tela por tarefa em sequência (sem ponto natural de parada); por camada (estrutura → textos → visuais: só mostra resultado no fim e um corte deixa tudo pela metade) |

## Telas

Tela lógica 640×360, fonte de 8 px por caractere, sem maiúscula acentuada, botões em caixa alta ASCII. Os painéis continuam com `draw_panel` (`UI_INK`/`UI_PANEL` + `UI_FRAME`) por cima dos fundos.

### Menu
- Fundo `fundo_menu.png`; por cima, `hemacias_deriva.png` deslizando para a esquerda a ~8 px/s, desenhada duas vezes lado a lado para emendar; o Nanobô do spritesheet (linha 1, frente, quadros parados) à direita do título, oscilando ±2 px num seno de ~2 s.
- Título "NANOBÔ" (bitmap ×3) e subtítulo como hoje; botões **INICIAR**, **COMO JOGAR**, **RECORDES**; linha do recorde e "F11 tela cheia" ficam.
- Saem do menu os painéis "Como jogar" e "Objetivo da missão" (o conteúdo vai para a cena nova).

### Como jogar (`GAME_SCENE_HOWTO`)
- Fundo `fundo_como_jogar.png`; um painel central com uma coluna de ícone à esquerda (sprites do jogo ampliados ×2: Nanobô, scanner, ícones das armas, ícones de vida) e até 6 linhas de texto.
- Coluna de ícone de 48×48 (decisão do construtor): p. 1 o Nanobô do spritesheet em ×1 (48×48, frente); p. 2 o ícone do scanner ×2 com os 2 marcadores desenhados em código embaixo; p. 3 os 6 ícones de arma em ×1 (16×16) numa grade 2×3; p. 4 `icone_nanobo` e `coracao_pulso` ×2, um sobre o outro.
- 4 páginas: mover e mirar; ler com o scanner; armas; Integridade e Vitais. Rodapé `n/4` e a dica de teclas.
- Enter, Espaço ou clique avançam; na última página voltam; Esc volta a qualquer momento. Setas ←/→ andam entre páginas.
- **Transições novas:** `MENU→HOWTO`, `HOWTO→MENU`, `PAUSE→HOWTO`, `HOWTO→PAUSE`. A cena lembra a origem (`game.c`), e ao voltar entra nela; vinda da Pausa, a partida continua parada (só `GAME` avança a lógica). A página volta à 1 a cada entrada.

### HUD (40 px, `PLAY_H` 320 sem mudança)

| Linha | Esquerda (x 8) | Meio | Direita (até x 632) |
|---|---|---|---|
| 0 (y 328) | ícone do Nanobô 16×16 + barra de bateria 64×8 (Integridade) | livre | arma · ícone · recarga · estado (como hoje) |
| 1 (y 344) | coração 16×16 pulsando + barra de ECG 64×8 (Vitais) | `pontos N · mm:ss` a partir de x 100 | `ameaças x/y · identificados a/b` |

- **Medidas:** coluna de vida = 16 + 4 + 64 = 84 px (x 8 a 92). Pior caso do bloco direito da linha 0 = `Neutralização · vírus livre` (216) + 4 + ícone 16 + 4 + recarga 38 + 6 + `recarga` (56) = 340 px, de x 292 a 632: sobram ~200 px. Linha 1: `pontos 9999 · 99:59` (19 car., 152 px) de x 100 a 252; contador à direita com 35 car. (280 px) de x 352: sobram ~100 px.
- **Barras:** 10 segmentos, preenchidos por `ceil(valor/10)`; a moldura vem do sprite e cada segmento é o `segmento.png` tingido. Integridade em `UI_CYAN`; Vitais em `UI_GREEN` ≥ `VITALS_MIN_BETWEEN`, `UI_AMBER` ≥ `VITALS_LOW`, `UI_RED` abaixo (as faixas atuais).
- **Coração:** quadros 0→1→2→0; período de 1,0 s com vitais cheios, caindo linearmente até 0,4 s com vitais em 0.
- O objetivo âmbar sai da linha 1.

### Aviso de objetivo
- Caixa centrada no topo do mundo (y 8), no estilo do aviso de feedback (placa `UI_INK`, borda âmbar, texto `UI_AMBER`), visível 4 s com esmaecer no último 0,5 s.
- Dispara: (1) no primeiro quadro da fase; (2) quando `threats_left()` chega a 0 (meta vira "vá ao destino"). Um aviso novo substitui o anterior. Não aparece com caixa do Diário aberta: espera ela fechar e então conta os 4 s.
- A meta é recalculada a cada quadro por `threats_left()`: a cada virada (>0 → 0 mostra o OB-04; 0 → >0, quando um esporo germina ou uma rajada de vírions está no ar, volta a meta da fase) sai um aviso novo. Viradas em menos de 1 s uma da outra não repetem o aviso (decisão do construtor).
- Com a ficha do scanner no topo, o aviso desce para baixo dela (mesma regra do feedback).

### Termo destacado fora da caixa (decisão do construtor, revisão de 26/11)
- O `{termo}` do inventário vale também fora do Diário: um desenho comum de linha com termo, reaproveitando a lógica da caixa. Aviso de objetivo: texto `UI_WARM`, termo `UI_AMBER`, borda âmbar. Páginas do "como jogar": texto `UI_TEXT`, termo `UI_AMBER`, como no Diário.

### Paleta e teclas
- Decisão do dono em 27/11 ([`decisoes.md`](../../game-design/decisoes.md) §"Interface (26/11/2026)", item 10): paleta clínica em azul-petróleo nas constantes `UI_*` de `main.c` (`UI_INK` `#0F1922`, `UI_PANEL` `#16242F`, `UI_FRAME` `#3A5566`, `UI_TEXT` `#EEF3F6`, `UI_DIM` `#9DB2C2`, `UI_AMBER` `#FFC94A`, `UI_KEY`/`UI_CYAN` `#6FE7F2`, `UI_RED` `#FF6B6B`, `UI_GREEN` `#5FD6A0`); véus de tela em tinta (`UI_VEIL`), não em preto.
- `[tecla]` no texto vira tampa de tecla: nome em `UI_KEY`, caixa de 14 px centrada na linha de 8 px, borda `UI_KEY`, fundo `UI_INK`, 3 px de folga de cada lado. O mesmo parser do `{termo}` (`draw_dialog_line`, `draw_marked_text`, `marked_text_width`, mais `draw_marked_text_aligned` para centro e direita); os colchetes não contam nos caracteres, cada tecla soma 6 px; a dica do mundo tem moldura neutra `UI_FRAME` para não dobrar a borda ciano.
- Vale nas páginas e no rodapé do "como jogar", no lembrete e no rodapé da Pausa, nos rodapés do menu, dos RECORDES, da abertura, dos Resultados, da Vitória e do Game Over, em `F11 tela cheia`, na dica do mundo, na ficha do scanner e nas instruções do nome na Vitória.

### Pausa
- Fundo: o mapa da fase atual (quadro congelado como hoje) com véu mais escuro; painel maior com título, o objetivo completo da fase, um lembrete curto das teclas e os botões **CONTINUAR** e **COMO JOGAR**. Esc continua voltando ao jogo.

### Ficha do scanner (624×50, posição atual)
- Segue o layout do **SC-01 aprovado no inventário** (que prevalece sobre o rascunho anterior desta seção): retrato 24×24 em x 14 (o próprio bitmap do agente escalado; `silhueta` antes da camada 1; fungo sem leitura com a silhueta como no mundo) e texto a partir de x 46.
- Os 2 marcadores ficam à direita da linha de estado, cada um com o rótulo aprovado: `forma` (SC-11) e `Gram e espécie` (SC-12), ou `estrutura` (SC-13) em vírus, célula e fungo. Apagado `UI_SLOT`, aceso `UI_AMBER`; o 2º acende verde na identificação. A regra continua com 2 camadas (decisão do dono em 26/11).
- Com os marcadores, estado + origem cabem em 50 caracteres na linha 0; as linhas 1 e 2 vão até a borda. O ícone do scanner sai da ficha.

### Resultados
- Fundo: o mapa da fase recém-concluída escurecido (`MAP_VEIL` mais forte), painéis como hoje; o painel "O que anotei nesta fase" já aprovado em 26/11.

### Abertura, Vitória e Game Over
- Abertura (`BRIEFING`): `hospital_abertura.png` atrás da caixa do Diário (o véu de 60% fica).
- Vitória: `hospital_vitoria.png`; uma linha na voz do Nanobô depois da frase da alta (os painéis descem 16 a 32 px, decisão de 26/11); painéis de estatística, Top 10 e nome nas posições atuais, com o campo de nome e o limite de 10 caracteres preservados.
- Game Over: `hospital_gameover.png`; texto da retirada do Nanobô pela equipe, sem morte, mais uma linha na voz do Nanobô; painel de estatística descendo o necessário.
- Controles no "como jogar" seguem o código (decisão de 26/11): botão direito escaneia, tecla X liga a defesa.

### Mira
- `mira.png` 48×16 (3 quadros 16×16, brancos), desenhada centrada no mouse, só em `GAME` e dentro da área de jogo, tingida com a cor da arma atual.
- Quadro (decisão do construtor): o quadro diz se um tiro sairia agora. 1 (recarga) com a recarga do ataque correndo; 2 (travado) em todo estado do HUD em que o tiro não sai: `travado` (trava de 2 s), `defesa` ativa e Reserva `esgotada`; senão 0 (pronto). Cor por arma: a mesma usada no HUD/tiros para cada `Laser` (definida na implementação, uma tabela de 6 cores).

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

## Ordem de construção (duas ondas)

**Onda 1, durante a fase:** HUD com a coluna de vida (ícones, barras, coração pulsando, pontos e tempo na linha 1), mira em sprite, aviso de objetivo e ficha do scanner com retrato e 2 marcadores; com os textos aprovados dessas áreas (HD, OB, SC, FB). Termina com capturas para o dono.

**Onda 2, fora da fase:** cena `GAME_SCENE_HOWTO` e suas transições, menu com fundo e hemácias, Pausa nova, Resultados sobre o mapa, abertura, Vitória e Game Over sobre o hospital; com os textos aprovados (MN, HW, PA, CT, VI, GO). Termina com capturas e a medição de memória.

Por quê: o 29/11 é o dia mais apertado do calendário; se ele estourar, a onda 1 já é uma entrega completa e coerente e a onda 2 usa a folga de 30/11.

## Textos

Inventário "atual × proposto" por tela, **aprovado pelo dono em 26/11** (`.hermes/inventario-interface.json`, local; página de aprovação com 19 entradas, HW-01 e HW-03 ajustados para o formato "tecla: ação" e "arma"). Abrange (menu, como jogar, HUD, aviso de objetivo, Pausa, ficha do scanner, feedback, Resultados, Vitória, Game Over), publicado numa página de aprovação com ok/comentário por linha. Regras: voz do Diário, frases curtas, um termo por linha onde ensina, fonte em `conteudo-biologico.md` para toda afirmação biológica, larguras medidas por área. Texto aprovado é registrado em `textos-do-jogo.md` com ID e contagem.

## O que fica para quem implementa

- Nomes das funções e estados novos no `main.c` (seguindo o estilo plano do arquivo).
- A tabela de cor por arma da mira.
- O ajuste fino de posições (±2 px) depois das capturas.

## Verificação

Build Debug e Release com 0 avisos; harness verde (com testes novos das transições de cena, se o harness alcançar `game.c`); `conferir_textos.py` 0/0; capturas de cada tela e estado (menu com deriva; 4 páginas; como jogar pela Pausa e volta; HUD com vitais em 3 faixas; mira nos 3 quadros; aviso de objetivo; ficha antes e depois de cada camada; Resultados das 3 fases; Vitória com nome; Game Over pelas duas causas); memória com `medir_memoria.ps1` abaixo de 100 MB.
