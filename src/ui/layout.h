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

#define BUTTON_X 240
#define BUTTON_Y 266
#define BUTTON_W 160
#define BUTTON_H 28

// medidas do HUD: tudo derivado do painel, nunca coordenada magica.
// duas linhas de 8 px separadas por 8 px, com 8 px em cima e embaixo (#243)
#define HUD_MARGIN 8
#define HUD_LINES 2
#define HUD_LINE_H 16
#define HUD_FONT_H 8

// a fonte embutida tem 8 px: menos de 12 por linha encosta uma na outra
_Static_assert(HUD_LINE_H >= 12, "linha do HUD apertada demais para a fonte");
_Static_assert((HUD_LINES - 1) * HUD_LINE_H + HUD_FONT_H + 2 * HUD_MARGIN <= PANEL_H,
    "HUD nao cabe no painel");

// ficha do scanner: tres linhas flutuando na faixa do mundo, so com alvo (#243)
#define CARD_ROW_H 14
#define CARD_H (3 * CARD_ROW_H + 8)
#define CARD_MARGIN 8
// retrato do agente a esquerda e o texto depois dele; marcador 6x6 por camada (SC-01)
#define CARD_PORTRAIT 24
#define CARD_TEXT_X 46
#define CARD_MARK 6

// medidas do kit de interface, todas derivadas do painel e da fonte
#define UI_PAD 8
#define UI_GAP 12
#define UI_BAR_SEGMENTS 4
#define UI_SEG_W 8
#define UI_SEG_H 6
#define UI_SEG_GAP 2
#define UI_BAR_W (UI_BAR_SEGMENTS * UI_SEG_W + (UI_BAR_SEGMENTS - 1) * UI_SEG_GAP)

// largura de cada barra do HUD (integridade e vitais) e a faixa vermelha (#218, #234)
#define VITALS_BAR_W 36
#define VITALS_LOW 30.0f
#define TITLE_SCALE 3

// paleta clinica em azul-petroleo (decisao do dono, 27/11): os hex de decisoes.md
#define UI_INK       al_map_rgb(15, 25, 34)    // #0F1922
#define UI_PANEL     al_map_rgb(22, 36, 47)    // #16242F
#define UI_TEXT      al_map_rgb(238, 243, 246) // #EEF3F6
#define UI_DIM       al_map_rgb(157, 178, 194) // #9DB2C2
#define UI_AMBER     al_map_rgb(255, 201, 74)  // #FFC94A
#define UI_PURPLE    al_map_rgb(190, 110, 240) // #BE6EF0
#define UI_RED       al_map_rgb(255, 107, 107) // #FF6B6B
#define UI_PINK      al_map_rgb(240, 130, 165) // Gram-: rosa da safranina (#214)
#define UI_KEY       al_map_rgb(111, 231, 242) // #6FE7F2, tampa de tecla: nome e borda

// veu de tinta com alfa a (pre-multiplicado): escurece o fundo puxando para o petroleo
#define UI_VEIL(a)   al_map_rgba_f(0.059f * (a), 0.098f * (a), 0.133f * (a), (a))

// folga de cada lado do nome na tampa de tecla: [tecla] ocupa o texto + 4 px
#define KEY_CAP_PAD 3

// mapa recuado e silhueta neutra: os agentes precisam se destacar do vermelho (#220)
#define MAP_TINT        al_map_rgb_f(0.68f, 0.62f, 0.66f)

// o mapa do bronquio ja nasce escuro: recua menos que o vaso da Fase 1 (#89)
#define MAP_TINT_AIRWAY al_map_rgb_f(0.9f, 0.86f, 0.9f)

#define MAP_VEIL        al_map_rgba_f(0.05f * 0.28f, 0.07f * 0.28f, 0.11f * 0.28f, 0.28f)
#define CONTACT_SHADOW  al_map_rgba_f(0.0f, 0.0f, 0.0f, 0.35f)

// tingimento do sprite cinza da bacteria depois da camada 2 (#214)
#define GRAM_POS_TINT   al_map_rgb_f(0.85f, 0.55f, 1.0f)
#define GRAM_NEG_TINT   al_map_rgb_f(1.0f, 0.62f, 0.74f)
#define UI_CYAN      al_map_rgb(111, 231, 242) // #6FE7F2, o mesmo da UI_KEY
#define UI_GREEN     al_map_rgb(95, 214, 160)  // #5FD6A0
#define UI_WARM      al_map_rgb(255, 231, 168) // #FFE7A8
#define UI_SLOT      al_map_rgb(37, 57, 74)    // #25394A, slot vazio, sem brilho
#define UI_HOVER     al_map_rgb(58, 50, 24)    // #3A3218, fundo do botao sob o cursor
#define UI_LINE      al_map_rgb(34, 52, 63)    // #22343F, separador e molduras discretas
#define UI_FRAME     al_map_rgb(58, 85, 102)   // #3A5566, moldura de painel
#define SHOT_FRAME 12
#define IMPACT_FRAME 24

// mira: 3 quadros de 16x16 na ordem do AimFrame (#246)
#define AIM_FRAME_SIZE 16
// o desenho e simetrico em volta do pixel 7: e ele que fica sob o mouse
#define AIM_CENTER 7

// aviso de objetivo: placa de 16 px de altura e esmaecer no ultimo 0,5 s (#246)
#define NOTICE_H 16
#define NOTICE_FADE_TIME 0.5f

// caixa de dialogo do diario: faixa de baixo, retrato a esquerda e ate 3 linhas
#define DIALOG_X 16
#define DIALOG_W 608
#define DIALOG_H 88
#define DIALOG_Y (SCREEN_H - 8 - DIALOG_H)
#define DIALOG_PAD 8
#define DIALOG_LINE_H 16

// a area de texto comeca depois da coluna do retrato mesmo sem retrato
#define DIALOG_TEXT_X (DIALOG_X + DIALOG_PAD + PLAYER_FRAME + DIALOG_PAD)
#define DIALOG_TEXT_COLS ((DIALOG_X + DIALOG_W - DIALOG_PAD - DIALOG_TEXT_X) / 8)

_Static_assert(DIALOG_TEXT_COLS >= 66, "linha da caixa de dialogo estreita demais");
_Static_assert(DIALOG_PAD + DIALOG_LINE_H * 3 + 8 + DIALOG_PAD + 8 + DIALOG_PAD <= DIALOG_H,
    "cabecalho, 3 linhas e rodape nao cabem na caixa");

#endif
