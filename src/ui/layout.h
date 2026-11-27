#ifndef NANOBO_LAYOUT_H
#define NANOBO_LAYOUT_H

// medidas da tela e da interface: so o desenho usa; as regras so conhecem a
// faixa do mundo (VIEW_W, VIEW_H em gameplay.h)
#include "../gameplay/gameplay.h"

// tela logica: o jogo desenha num bitmap deste tamanho e a janela amplia
#define SCREEN_W 640
#define SCREEN_H 360

// a faixa do mundo em pixels da tela
#define PLAY_H VIEW_H

// HUD no rodape da tela logica: duas linhas; a ficha do scanner flutua no mundo
#define PANEL_H 40

// o painel comeca onde a faixa visivel do mundo acaba
#define SCAN_PANEL_Y PLAY_H

// a tela e a faixa mais o painel
_Static_assert(PANEL_H + PLAY_H == SCREEN_H, "tela = faixa do mundo + painel");
_Static_assert(SCREEN_W % 32 == 0, "largura da tela precisa ser multipla de 32");

#endif
