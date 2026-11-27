#ifndef NANOBO_UI_H
#define NANOBO_UI_H

// interface: kit de paineis, botoes e texto (ui_kit.c), HUD e ficha do scanner
// (hud.c) e as telas fora da fase (screens.c) (27/11)
#include "../render/render.h"

// como jogar: quantas paginas a cena tem
#define HOWTO_PAGES 4

// botao de interface: retangulo, rotulo e os estados visuais
typedef struct
{
    float x;
    float y;
    float w;
    float h;
    const char* label;
} UiButton;

// uma entrada: cabecalho, paginas de ate 3 linhas e o termo entre chaves
typedef struct
{
    const char* header;
    int pages;
    const char* lines[DIALOG_MAX_PAGES][3];
    bool portrait;
} DialogEntry;

// abertura (#244): ficha do hospital e Anotacao 0, na caixa, com estado proprio
// fora da fila de cards; textos aprovados em 26/11, registrados em textos-do-jogo.md §1.1
#define OPENING_ENTRIES 2

// ui_kit.c
float to_logical_x(float window_x);
float to_logical_y(float window_y);
void draw_panel(float x, float y, float w, float h, ALLEGRO_COLOR fill, ALLEGRO_COLOR border);
void draw_rule(float center_x, float y, float w, ALLEGRO_COLOR color);
int button_hit(const UiButton* button, float x, float y);
const UiButton* screen_buttons(int* count);
void draw_buttons(const UiButton* buttons, int count);
void ui_move_focus(int step);
void activate_button(int index);
extern DialogState card_dialog;
extern ConceptCard card_dialog_entry;
void dialog_open_entry(DialogState* d, const DialogEntry* e);
void sync_card_dialog();
void press_card_dialog();
void skip_card_dialog();
ALLEGRO_COLOR faded_color(ALLEGRO_COLOR color, float alpha);
void draw_marked_text(float x, float y, ALLEGRO_COLOR base, const char* text);
int marked_text_width(const char* text);
void draw_marked_text_aligned(float x, float y, ALLEGRO_COLOR base, int flags, const char* text);
void draw_dialog(const DialogEntry* e, const DialogState* d);
void draw_concept_card();

// hud.c
void draw_stat(float x, float y, float right, const char* label, const char* value, ALLEGRO_COLOR color);
int scan_card_target(bool* from_cursor);
float scan_card_y(int target);
void draw_scan_card(int target, bool from_cursor, float card_y);
void draw_hud();

// screens.c
void draw_menu();
extern DialogState opening_dialog;
void start_opening(void);
void press_opening();
void draw_briefing();
void draw_pause();
void howto_return();
void howto_advance();
void draw_howto();
void draw_results();
void draw_victory();
void draw_game_over();

#endif
