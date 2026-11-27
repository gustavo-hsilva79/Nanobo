#ifndef NANOBO_RENDER_H
#define NANOBO_RENDER_H

// desenho do mundo (world.c) e resposta visual (feel.c), mais os recursos e a
// janela que o main.c carrega e os modulos de desenho leem (27/11)
#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "../core/game.h"
#include "../entities/entities.h"
#include "../gameplay/gameplay.h"
#include "../ui/layout.h"

// ---------------------------------------------------------------
// resposta visual aos eventos (#221): so desenho, mais um hit-stop curto
// na eliminacao; um ponto de chamada por evento (SfxEvent) e tres niveis
// ---------------------------------------------------------------

#define FEEL_SHAKE_TIME 0.25f
#define FEEL_FLASH_TIME 0.20f
#define FEEL_RING_TIME 0.30f
// o impacto e uma animacao de 4 quadros de 60 ms (#242)
#define FEEL_IMPACT_TIME 0.24f
#define FEEL_POP_TIME 0.80f
#define FEEL_PARTICLE_TIME 0.45f
#define FEEL_PARTICLES 12
#define FEEL_HIT_STOP 0.05

typedef enum
{
    FEEL_LIGHT = 0,
    FEEL_MEDIUM = 1,
    FEEL_STRONG = 2
} FeelLevel;

typedef struct
{
    float x;
    float y;
    float vx;
    float vy;
    float life;
} FeelParticle;

// recursos, janela e estado que o main.c define
extern ALLEGRO_FONT* font;
extern ALLEGRO_BITMAP* nanobo;
extern bool game_over_patient;
extern ALLEGRO_BITMAP* scanner_icon;
extern ALLEGRO_BITMAP* rupture_icon;
extern ALLEGRO_BITMAP* dissolution_icon;
extern ALLEGRO_BITMAP* reserve_icon;
extern ALLEGRO_BITMAP* antibody_icon;
extern ALLEGRO_BITMAP* lyse_icon;
extern ALLEGRO_BITMAP* perforate_icon;
extern ALLEGRO_BITMAP* nanobo_icon;
extern ALLEGRO_BITMAP* heart_pulse;
extern ALLEGRO_BITMAP* battery_bar;
extern ALLEGRO_BITMAP* ecg_bar;
extern ALLEGRO_BITMAP* bar_segment;
extern ALLEGRO_BITMAP* aim_sprite;
extern ALLEGRO_BITMAP* menu_background;
extern ALLEGRO_BITMAP* menu_cells;
extern ALLEGRO_BITMAP* howto_background;
extern ALLEGRO_BITMAP* hospital_opening;
extern ALLEGRO_BITMAP* hospital_victory;
extern ALLEGRO_BITMAP* hospital_game_over;
extern ALLEGRO_BITMAP* shots_sheet;
extern ALLEGRO_BITMAP* impacts_sheet;
extern ALLEGRO_BITMAP* title_bitmap;
extern ALLEGRO_BITMAP* cell_sprite;
extern ALLEGRO_BITMAP* virus_sprite;
extern ALLEGRO_BITMAP* cell_sprite_b;
extern ALLEGRO_BITMAP* virus_sprite_b;
extern ALLEGRO_BITMAP* coccus_grey[SPRITE_VARIANTS];
extern ALLEGRO_BITMAP* bacillus_grey[SPRITE_VARIANTS];
extern ALLEGRO_BITMAP* silhouette_sprite;
extern ALLEGRO_BITMAP* toxin_sprite;
extern ALLEGRO_BITMAP* yeast_sprite[SPRITE_VARIANTS];
extern ALLEGRO_BITMAP* mold_sprite;
extern ALLEGRO_BITMAP* hypha_sprite;
extern ALLEGRO_BITMAP* spore_sprite;
extern ALLEGRO_BITMAP* world_map;
bool load_phase_map(int phase);
extern ALLEGRO_BITMAP* canvas;
extern float screen_scale;
extern float screen_off_x;
extern float screen_off_y;
extern float mouse_x;
extern float mouse_y;
extern int ui_focus;
extern int howto_page;
extern bool name_entry_active;
extern char name_entry[RANKING_NAME_BYTES];
extern int name_entry_chars;
extern int ranking_highlight;
extern bool ranking_save_failed;
extern bool ranking_panel_open;
extern bool show_debug;

// feel.c: tremor, flash, anel, impacto, pop, particulas e hit-stop (#221)
extern float feel_flash_time;
extern double feel_hit_stop;
void feel_trigger();
void feel_update(float dt);
float feel_offset_x();
float feel_offset_y();
void draw_feel_world();

// world.c: mapa, correntes, perigos, agentes, projeteis, destino e mira
ALLEGRO_BITMAP* species_sprite(Species species, int variant);
ALLEGRO_BITMAP* bacteria_sprite(Shape shape, int variant);
float view_x(float x);
float view_y(float y);
float view_offset(float camera_coord);
ALLEGRO_COLOR laser_color(Laser laser);
void draw_phase_map(float x, float y, float h);
void draw_game();

#endif
