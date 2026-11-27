#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "core/game.h"
#include "entities/entities.h"
#include "gameplay/gameplay.h"
#include "ui/layout.h"

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

// volume dos efeitos e da musica, de 0 a 1 (#31, #32)
#define SFX_GAIN 0.55f
#define MUSIC_GAIN 0.30f

// y de cada linha do HUD, contando de cima para baixo dentro do painel
int hud_y(int line)
{
    return SCAN_PANEL_Y + HUD_MARGIN + line * HUD_LINE_H;
}

// recursos do jogo, carregados uma vez antes do loop
ALLEGRO_FONT* font = NULL;
ALLEGRO_BITMAP* nanobo = NULL;

// game over pelos vitais do paciente (true) ou pela integridade do nanobo (#234)
bool game_over_patient = false;

// icones da interface: scanner e as duas armas
ALLEGRO_BITMAP* scanner_icon = NULL;
ALLEGRO_BITMAP* rupture_icon = NULL;
ALLEGRO_BITMAP* dissolution_icon = NULL;

// icones da Reserva, da Neutralizacao e da Lise, no traco das armas (#242)
ALLEGRO_BITMAP* reserve_icon = NULL;
ALLEGRO_BITMAP* antibody_icon = NULL;
ALLEGRO_BITMAP* lyse_icon = NULL;

// icone da Perfuracao: o anel do antifungico na membrana (Fase 3)
ALLEGRO_BITMAP* perforate_icon = NULL;

// arte nova da interface (redesenho): hud, mira, menu, como jogar e telas do hospital
ALLEGRO_BITMAP* nanobo_icon = NULL;
ALLEGRO_BITMAP* heart_pulse = NULL;
ALLEGRO_BITMAP* battery_bar = NULL;
ALLEGRO_BITMAP* ecg_bar = NULL;
ALLEGRO_BITMAP* bar_segment = NULL;
ALLEGRO_BITMAP* aim_sprite = NULL;
ALLEGRO_BITMAP* menu_background = NULL;
ALLEGRO_BITMAP* menu_cells = NULL;
ALLEGRO_BITMAP* howto_background = NULL;
ALLEGRO_BITMAP* hospital_opening = NULL;
ALLEGRO_BITMAP* hospital_victory = NULL;
ALLEGRO_BITMAP* hospital_game_over = NULL;

// tiros: uma linha por ferramenta, na ordem do Laser, x 2 quadros de 12x12;
// impactos: 3 linhas (dose certa, sem efeito, eliminado) x 4 quadros de 24x24 (#242)
ALLEGRO_BITMAP* shots_sheet = NULL;
ALLEGRO_BITMAP* impacts_sheet = NULL;
#define SHOT_FRAME 12
#define IMPACT_FRAME 24

// mira: 3 quadros de 16x16 na ordem do AimFrame (#246)
#define AIM_FRAME_SIZE 16
// o desenho e simetrico em volta do pixel 7: e ele que fica sob o mouse
#define AIM_CENTER 7

// aviso de objetivo: placa de 16 px de altura e esmaecer no ultimo 0,5 s (#246)
#define NOTICE_H 16
#define NOTICE_FADE_TIME 0.5f

// titulo do menu montado a partir da fonte, desenhado ampliado
ALLEGRO_BITMAP* title_bitmap = NULL;

// celula do paciente (64x64) e virus (16x16), duas variantes cada, ja no
// tamanho da escala biologica (#211, #241)
ALLEGRO_BITMAP* cell_sprite = NULL;
ALLEGRO_BITMAP* virus_sprite = NULL;
ALLEGRO_BITMAP* cell_sprite_b = NULL;
ALLEGRO_BITMAP* virus_sprite_b = NULL;

// bacterias em cinza neutro, uma por forma e variante: a cor de Gram vem do
// tingimento (#214). O coco e o cacho e a cadeia; o bacilo e o bastao com
// flagelo e o par de bastoes (#241)
ALLEGRO_BITMAP* coccus_grey[SPRITE_VARIANTS] = { NULL };
ALLEGRO_BITMAP* bacillus_grey[SPRITE_VARIANTS] = { NULL };

// silhueta do agente sem leitura e toxina do coco (#241)
ALLEGRO_BITMAP* silhouette_sprite = NULL;
ALLEGRO_BITMAP* toxin_sprite = NULL;

// fungos da Fase 3: levedura em 2 variantes (40x40, a b com broto), base do
// bolor (48x48), segmento da hifa (16x16, aponta para a direita) e esporo (8x8)
ALLEGRO_BITMAP* yeast_sprite[SPRITE_VARIANTS] = { NULL };
ALLEGRO_BITMAP* mold_sprite = NULL;
ALLEGRO_BITMAP* hypha_sprite = NULL;
ALLEGRO_BITMAP* spore_sprite = NULL;

// mapa do mundo da fase atual (1920x512), sempre atras do resto. So um fica na
// memoria por vez: cada mapa custa cerca de 15 MB carregado, e os dois juntos
// levavam o pico a 96 MB, perto do limite de 100 (#89)
ALLEGRO_BITMAP* world_map = NULL;
int world_map_phase = 0;

// arquivo do mapa de cada fase: a Fase 1 no vaso, a Fase 2 no bronquio (#89)
// e a Fase 3 nos alveolos do pulmao (#233)
const char* phase_map_path(int phase)
{
    if (phase == 3)
    {
        return "assets/backgrounds/mapa_fase3.png";
    }

    if (phase == 2)
    {
        return "assets/backgrounds/mapa_fase2.png";
    }

    return "assets/backgrounds/mapa_fase1.png";
}

// troca o mapa quando a fase muda: libera o anterior antes de abrir o novo,
// para o pico ficar em um mapa so (#89)
bool load_phase_map(int phase)
{
    // a mesma fase nao recarrega; se o arquivo falhou, nao tenta a cada quadro
    if (world_map_phase == phase)
    {
        return world_map != NULL;
    }

    if (world_map)
    {
        al_destroy_bitmap(world_map);
        world_map = NULL;
    }

    world_map = al_load_bitmap(phase_map_path(phase));
    world_map_phase = phase;

    if (!world_map)
    {
        printf("couldn't load world map\n");
        return false;
    }

    return true;
}

// fundos de tela de 640x360: so o conjunto da cena atual fica na memoria,
// como o mapa da fase (cada um custa muito mais que a textura bruta)
static const char* const BG_MENU = "assets/backgrounds/fundo_menu.png";
static const char* const BG_CELLS = "assets/backgrounds/hemacias_deriva.png";
static const char* const BG_HOWTO = "assets/backgrounds/fundo_como_jogar.png";
static const char* const BG_OPENING = "assets/backgrounds/hospital_abertura.png";
static const char* const BG_VICTORY = "assets/backgrounds/hospital_vitoria.png";
static const char* const BG_GAME_OVER = "assets/backgrounds/hospital_gameover.png";

// na carga so confere que os seis arquivos existem; faltar um continua fatal
bool screen_backgrounds_exist(void)
{
    return al_filename_exists(BG_MENU) && al_filename_exists(BG_CELLS)
        && al_filename_exists(BG_HOWTO) && al_filename_exists(BG_OPENING)
        && al_filename_exists(BG_VICTORY) && al_filename_exists(BG_GAME_OVER);
}

// libera o fundo que a cena nao quer (fase 0) ou abre o que ela quer (fase 1)
static void sync_one_background(ALLEGRO_BITMAP** slot, const char* path, bool wanted, bool loading)
{
    if (!loading && !wanted && *slot)
    {
        al_destroy_bitmap(*slot);
        *slot = NULL;
    }
    else if (loading && wanted && !*slot)
    {
        *slot = al_load_bitmap(path);
    }
}

// libera primeiro o que sai e so depois abre o que entra (um conjunto por vez)
void sync_screen_backgrounds(GameScene scene)
{
    int pass;

    for (pass = 0; pass < 2; pass++)
    {
        bool loading = (pass == 1);

        sync_one_background(&menu_background, BG_MENU, scene == GAME_SCENE_MENU, loading);
        sync_one_background(&menu_cells, BG_CELLS, scene == GAME_SCENE_MENU, loading);
        sync_one_background(&howto_background, BG_HOWTO, scene == GAME_SCENE_HOWTO, loading);
        sync_one_background(&hospital_opening, BG_OPENING, scene == GAME_SCENE_BRIEFING, loading);
        sync_one_background(&hospital_victory, BG_VICTORY, scene == GAME_SCENE_VICTORY, loading);
        sync_one_background(&hospital_game_over, BG_GAME_OVER, scene == GAME_SCENE_GAME_OVER, loading);
    }
}

// audio: opcional, o jogo abre mudo quando nao ha placa de som (#31)
bool audio_ready = false;
ALLEGRO_SAMPLE* sfx_scan = NULL;
ALLEGRO_SAMPLE* sfx_shot = NULL;
ALLEGRO_SAMPLE* sfx_hit = NULL;
ALLEGRO_SAMPLE* sfx_neutralized = NULL;
ALLEGRO_SAMPLE* sfx_penalty = NULL;
ALLEGRO_SAMPLE* sfx_defense = NULL;

// musica de fundo em loop, parada na pausa (#32)
ALLEGRO_SAMPLE* music = NULL;
ALLEGRO_SAMPLE_ID music_id;
bool music_playing = false;

// id do ultimo evento de audio que ja tocou: um som por evento (#31)
unsigned int sfx_seen = 0;

// janela, canvas logico e a escala inteira aplicada na saida
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_BITMAP* canvas = NULL;
float screen_scale = 1.0f;
float screen_off_x = 0.0f;
float screen_off_y = 0.0f;

// cursor em pixel da janela: quem desenha converte com to_logical_x
float mouse_x = 0.0f;
float mouse_y = 0.0f;

// botao focado na tela atual e a ultima cena desenhada
int ui_focus = 0;
void start_opening(void);

// como jogar: pagina atual (0 a 3), zerada a cada entrada na cena
#define HOWTO_PAGES 4
int howto_page = 0;

// Top 10 na interface (#81, #197): entrada de nome na Vitoria, painel do
// ranking no menu e destaque do registro novo; nenhuma cena nova
bool name_entry_active = false;
char name_entry[RANKING_NAME_BYTES] = "";
int name_entry_chars = 0;
int ranking_highlight = -1;
bool ranking_save_failed = false;
bool ranking_panel_open = false;

// o painel do Top 10 aparece no menu e na vitoria (#197)
void draw_ranking_panel(float x, float y, float w, int highlight, bool in_menu);

// linha marcada ([tecla] e {termo}): usada pelo menu e pela ficha antes de ser definida
void draw_marked_text(float x, float y, ALLEGRO_COLOR base, const char* text);
void draw_marked_text_aligned(float x, float y, ALLEGRO_COLOR base, int flags, const char* text);

// leitura de QA: F1 liga e desliga; FPS e tempo de quadro em todo build,
// o estado interno dos agentes so no Debug
bool show_debug = false;

// medicao do F1: soma de 1 s do tempo de atualizacao e de desenho (sem o flip)
double perf_window_start = 0.0;
double perf_update_time = 0.0;
double perf_draw_time = 0.0;
int perf_frames = 0;
int perf_fps = 0;
float perf_frame_ms = 0.0f;
GameScene ui_last_scene = GAME_SCENE_MENU;

// rele a posicao do cursor na janela: resize e fullscreen mudam o referencial
void refresh_mouse()
{
    ALLEGRO_MOUSE_STATE state;

    if (!display)
    {
        return;
    }

    al_get_mouse_state(&state);
    mouse_x = (float)state.x;
    mouse_y = (float)state.y;
}

// maior escala inteira que cabe na janela; abaixo de 1 encolhe para caber
void update_screen_fit()
{
    int w = al_get_display_width(display);
    int h = al_get_display_height(display);
    float fit_w;
    float fit_h;
    float fit;

    if (w < 1 || h < 1)
    {
        return;
    }

    fit_w = (float)w / (float)SCREEN_W;
    fit_h = (float)h / (float)SCREEN_H;
    fit = fit_w < fit_h ? fit_w : fit_h;

    // a escala acompanha a janela: maximizar ou redimensionar preenche a tela
    // inteira, e so sobra barra quando a proporcao da janela nao e 16:9
    screen_scale = fit;
    screen_off_x = floorf(((float)w - (float)SCREEN_W * fit) / 2.0f);
    screen_off_y = floorf(((float)h - (float)SCREEN_H * fit) / 2.0f);
}

// abre a janela no maior multiplo inteiro que cabe no monitor
bool create_window()
{
    ALLEGRO_MONITOR_INFO monitor;
    int scale = 2;

    if (al_get_monitor_info(0, &monitor))
    {
        int monitor_w = monitor.x2 - monitor.x1;
        int monitor_h = monitor.y2 - monitor.y1;
        int fit_w = (monitor_w - 80) / SCREEN_W;
        int fit_h = (monitor_h - 120) / SCREEN_H;

        scale = fit_w < fit_h ? fit_w : fit_h;

        if (scale < 1)
        {
            scale = 1;
        }

        if (scale > 6)
        {
            scale = 6;
        }
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_MAXIMIZED);
    display = al_create_display(SCREEN_W * scale, SCREEN_H * scale);

    if (!display)
    {
        printf("couldn't initialize display\n");
        return false;
    }

    canvas = al_create_bitmap(SCREEN_W, SCREEN_H);

    if (!canvas)
    {
        printf("couldn't initialize canvas\n");
        return false;
    }

    // a janela nasce maximizada, mas o allegro continua com o tamanho pedido na
    // criacao: sem este ack a escala sai menor que a janela e o clique do mouse
    // e convertido com um tamanho que nao e o da tela
    al_acknowledge_resize(display);
    update_screen_fit();

    // janela menor que a tela logica deixa a escala quebrada: 640x360 e o minimo
    al_set_window_constraints(display, SCREEN_W, SCREEN_H, 0, 0);
    al_apply_window_constraints(display, true);

    return true;
}

// F11 alterna fullscreen sem mudar a resolucao do monitor
void toggle_fullscreen()
{
    int flags = al_get_display_flags(display);

    if (flags & ALLEGRO_FULLSCREEN_WINDOW)
    {
        al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);
        // a janela volta para o tamanho pedido na criacao (1920x1080); maximizar
        // de novo devolve a janela ao tamanho do monitor da pessoa
        al_set_display_flag(display, ALLEGRO_MAXIMIZED, true);
    }
    else
    {
        al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
    }

    // a troca de modo muda o tamanho real da tela, e o allegro so conta isso
    // depois do ack: sem ele a escala e o clique ficam no tamanho anterior
    al_acknowledge_resize(display);
    update_screen_fit();
    refresh_mouse();

    // a troca de modo pode devolver o cursor do sistema: reaplica o estado da fase (#198)
    if (game_get_scene() == GAME_SCENE_GAME)
    {
        al_hide_mouse_cursor(display);
    }
}

// texto virado bitmap: o titulo amplia o mesmo glifo sem borrar
ALLEGRO_BITMAP* make_text_bitmap(const char* text, ALLEGRO_COLOR color)
{
    int w = al_get_text_width(font, text);
    int h = al_get_font_line_height(font);
    ALLEGRO_BITMAP* bmp;

    if (w <= 0 || h <= 0)
    {
        return NULL;
    }

    bmp = al_create_bitmap(w, h);

    if (!bmp)
    {
        return NULL;
    }

    al_set_target_bitmap(bmp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    al_draw_text(font, color, 0.0f, 0.0f, 0, text);
    al_set_target_backbuffer(display);

    return bmp;
}

// carrega tudo que o jogo precisa antes do loop por frame
bool load_resources()
{
    font = al_create_builtin_font();
    if (!font)
    {
        printf("couldn't initialize font\n");
        return false;
    }

    nanobo = al_load_bitmap("assets/sprites/nanobo_spritesheet.png");
    if (!nanobo)
    {
        printf("couldn't load nanobo spritesheet\n");
        return false;
    }

    // agentes finais da #241: cada um no tamanho da sua especie
    cell_sprite = al_load_bitmap("assets/sprites/celula_paciente_a.png");
    cell_sprite_b = al_load_bitmap("assets/sprites/celula_paciente_b.png");
    if (!cell_sprite || !cell_sprite_b)
    {
        printf("couldn't load cell\n");
        return false;
    }

    virus_sprite = al_load_bitmap("assets/sprites/virus_a.png");
    virus_sprite_b = al_load_bitmap("assets/sprites/virus_b.png");
    if (!virus_sprite || !virus_sprite_b)
    {
        printf("couldn't load virus\n");
        return false;
    }

    coccus_grey[0] = al_load_bitmap("assets/sprites/coco_a.png");
    coccus_grey[1] = al_load_bitmap("assets/sprites/coco_b.png");
    bacillus_grey[0] = al_load_bitmap("assets/sprites/bacilo_a.png");
    bacillus_grey[1] = al_load_bitmap("assets/sprites/bacilo_b.png");
    if (!coccus_grey[0] || !coccus_grey[1] || !bacillus_grey[0] || !bacillus_grey[1])
    {
        printf("couldn't load bacteria\n");
        return false;
    }

    silhouette_sprite = al_load_bitmap("assets/sprites/silhueta.png");
    toxin_sprite = al_load_bitmap("assets/sprites/toxina.png");
    if (!silhouette_sprite || !toxin_sprite)
    {
        printf("couldn't load silhouette or toxin\n");
        return false;
    }

    yeast_sprite[0] = al_load_bitmap("assets/sprites/levedura_a.png");
    yeast_sprite[1] = al_load_bitmap("assets/sprites/levedura_b.png");
    mold_sprite = al_load_bitmap("assets/sprites/bolor.png");
    hypha_sprite = al_load_bitmap("assets/sprites/hifa.png");
    spore_sprite = al_load_bitmap("assets/sprites/esporo.png");
    if (!yeast_sprite[0] || !yeast_sprite[1] || !mold_sprite || !hypha_sprite || !spore_sprite)
    {
        printf("couldn't load fungi\n");
        return false;
    }

    scanner_icon = al_load_bitmap("assets/sprites/icone_scanner.png");
    if (!scanner_icon)
    {
        printf("couldn't load scanner icon\n");
        return false;
    }

    rupture_icon = al_load_bitmap("assets/sprites/icone_ruptura.png");
    if (!rupture_icon)
    {
        printf("couldn't load ruptura icon\n");
        return false;
    }

    dissolution_icon = al_load_bitmap("assets/sprites/icone_dissolucao.png");
    if (!dissolution_icon)
    {
        printf("couldn't load dissolucao icon\n");
        return false;
    }

    reserve_icon = al_load_bitmap("assets/sprites/icone_reserva.png");
    antibody_icon = al_load_bitmap("assets/sprites/icone_neutralizacao.png");
    lyse_icon = al_load_bitmap("assets/sprites/icone_lise.png");
    perforate_icon = al_load_bitmap("assets/sprites/icone_perfuracao.png");
    if (!reserve_icon || !antibody_icon || !lyse_icon || !perforate_icon)
    {
        printf("couldn't load tool icons\n");
        return false;
    }

    shots_sheet = al_load_bitmap("assets/sprites/tiros.png");
    impacts_sheet = al_load_bitmap("assets/sprites/impactos.png");
    if (!shots_sheet || !impacts_sheet)
    {
        printf("couldn't load shots or impacts\n");
        return false;
    }

    nanobo_icon = al_load_bitmap("assets/sprites/icone_nanobo.png");
    heart_pulse = al_load_bitmap("assets/sprites/coracao_pulso.png");
    battery_bar = al_load_bitmap("assets/sprites/barra_bateria.png");
    ecg_bar = al_load_bitmap("assets/sprites/barra_ecg.png");
    bar_segment = al_load_bitmap("assets/sprites/segmento.png");
    aim_sprite = al_load_bitmap("assets/sprites/mira.png");
    if (!nanobo_icon || !heart_pulse || !battery_bar || !ecg_bar || !bar_segment || !aim_sprite || !screen_backgrounds_exist())
    {
        printf("couldn't load interface art\n");
        return false;
    }

    title_bitmap = make_text_bitmap("NANOBÔ", al_map_rgb(255, 255, 255));
    if (!title_bitmap)
    {
        printf("couldn't build title\n");
        return false;
    }

    // os mapas das outras fases abrem na troca de fase, mas o arquivo precisa
    // existir desde ja: asset ausente fecha o jogo na carga, como os outros (#89)
    if (!al_filename_exists(phase_map_path(2)))
    {
        printf("couldn't find airway map\n");
        return false;
    }

    if (!al_filename_exists(phase_map_path(3)))
    {
        printf("couldn't find lung map\n");
        return false;
    }

    if (!load_phase_map(1))
    {
        return false;
    }

    // sons curtos: sem audio instalado os ponteiros ficam nulos (#31)
    if (audio_ready)
    {
        sfx_scan = al_load_sample("assets/audio/sfx/scanner.wav");
        sfx_shot = al_load_sample("assets/audio/sfx/disparo.wav");
        sfx_hit = al_load_sample("assets/audio/sfx/impacto.wav");
        sfx_neutralized = al_load_sample("assets/audio/sfx/neutralizacao.wav");
        sfx_penalty = al_load_sample("assets/audio/sfx/penalidade.wav");
        sfx_defense = al_load_sample("assets/audio/sfx/defesa.wav");
        music = al_load_sample("assets/audio/music/fundo.wav");

        if (!sfx_scan || !sfx_shot || !sfx_hit || !sfx_neutralized
            || !sfx_penalty || !sfx_defense || !music)
        {
            printf("couldn't load sound effects\n");
            return false;
        }
    }

    return true;
}

// libera o que foi carregado, na ordem inversa
void free_resources()
{
    if (world_map)
    {
        al_destroy_bitmap(world_map);
        world_map = NULL;
        world_map_phase = 0;
    }

    if (silhouette_sprite)
    {
        al_destroy_bitmap(silhouette_sprite);
        silhouette_sprite = NULL;
    }

    if (toxin_sprite)
    {
        al_destroy_bitmap(toxin_sprite);
        toxin_sprite = NULL;
    }

    for (int v = 0; v < SPRITE_VARIANTS; v++)
    {
        if (yeast_sprite[v])
        {
            al_destroy_bitmap(yeast_sprite[v]);
            yeast_sprite[v] = NULL;
        }
    }

    if (mold_sprite)
    {
        al_destroy_bitmap(mold_sprite);
        mold_sprite = NULL;
    }

    if (hypha_sprite)
    {
        al_destroy_bitmap(hypha_sprite);
        hypha_sprite = NULL;
    }

    if (spore_sprite)
    {
        al_destroy_bitmap(spore_sprite);
        spore_sprite = NULL;
    }

    if (cell_sprite)
    {
        al_destroy_bitmap(cell_sprite);
        cell_sprite = NULL;
    }

    if (virus_sprite)
    {
        al_destroy_bitmap(virus_sprite);
        virus_sprite = NULL;
    }

    if (cell_sprite_b)
    {
        al_destroy_bitmap(cell_sprite_b);
        cell_sprite_b = NULL;
    }

    if (virus_sprite_b)
    {
        al_destroy_bitmap(virus_sprite_b);
        virus_sprite_b = NULL;
    }

    for (int v = 0; v < SPRITE_VARIANTS; v++)
    {
        if (coccus_grey[v])
        {
            al_destroy_bitmap(coccus_grey[v]);
            coccus_grey[v] = NULL;
        }

        if (bacillus_grey[v])
        {
            al_destroy_bitmap(bacillus_grey[v]);
            bacillus_grey[v] = NULL;
        }
    }

    if (reserve_icon)
    {
        al_destroy_bitmap(reserve_icon);
        reserve_icon = NULL;
    }

    if (antibody_icon)
    {
        al_destroy_bitmap(antibody_icon);
        antibody_icon = NULL;
    }

    if (lyse_icon)
    {
        al_destroy_bitmap(lyse_icon);
        lyse_icon = NULL;
    }

    if (perforate_icon)
    {
        al_destroy_bitmap(perforate_icon);
        perforate_icon = NULL;
    }

    if (shots_sheet)
    {
        al_destroy_bitmap(shots_sheet);
        shots_sheet = NULL;
    }

    if (hospital_game_over)
    {
        al_destroy_bitmap(hospital_game_over);
        hospital_game_over = NULL;
    }

    if (hospital_victory)
    {
        al_destroy_bitmap(hospital_victory);
        hospital_victory = NULL;
    }

    if (hospital_opening)
    {
        al_destroy_bitmap(hospital_opening);
        hospital_opening = NULL;
    }

    if (howto_background)
    {
        al_destroy_bitmap(howto_background);
        howto_background = NULL;
    }

    if (menu_cells)
    {
        al_destroy_bitmap(menu_cells);
        menu_cells = NULL;
    }

    if (menu_background)
    {
        al_destroy_bitmap(menu_background);
        menu_background = NULL;
    }

    if (aim_sprite)
    {
        al_destroy_bitmap(aim_sprite);
        aim_sprite = NULL;
    }

    if (bar_segment)
    {
        al_destroy_bitmap(bar_segment);
        bar_segment = NULL;
    }

    if (ecg_bar)
    {
        al_destroy_bitmap(ecg_bar);
        ecg_bar = NULL;
    }

    if (battery_bar)
    {
        al_destroy_bitmap(battery_bar);
        battery_bar = NULL;
    }

    if (heart_pulse)
    {
        al_destroy_bitmap(heart_pulse);
        heart_pulse = NULL;
    }

    if (nanobo_icon)
    {
        al_destroy_bitmap(nanobo_icon);
        nanobo_icon = NULL;
    }

    if (impacts_sheet)
    {
        al_destroy_bitmap(impacts_sheet);
        impacts_sheet = NULL;
    }

    if (dissolution_icon)
    {
        al_destroy_bitmap(dissolution_icon);
        dissolution_icon = NULL;
    }

    if (rupture_icon)
    {
        al_destroy_bitmap(rupture_icon);
        rupture_icon = NULL;
    }

    if (scanner_icon)
    {
        al_destroy_bitmap(scanner_icon);
        scanner_icon = NULL;
    }

    if (title_bitmap)
    {
        al_destroy_bitmap(title_bitmap);
        title_bitmap = NULL;
    }

    if (nanobo)
    {
        al_destroy_bitmap(nanobo);
        nanobo = NULL;
    }

    if (sfx_defense)
    {
        al_destroy_sample(sfx_defense);
        sfx_defense = NULL;
    }

    if (sfx_penalty)
    {
        al_destroy_sample(sfx_penalty);
        sfx_penalty = NULL;
    }

    if (sfx_neutralized)
    {
        al_destroy_sample(sfx_neutralized);
        sfx_neutralized = NULL;
    }

    if (sfx_hit)
    {
        al_destroy_sample(sfx_hit);
        sfx_hit = NULL;
    }

    if (sfx_shot)
    {
        al_destroy_sample(sfx_shot);
        sfx_shot = NULL;
    }

    if (sfx_scan)
    {
        al_destroy_sample(sfx_scan);
        sfx_scan = NULL;
    }

    if (music)
    {
        al_destroy_sample(music);
        music = NULL;
    }

    if (audio_ready)
    {
        al_uninstall_audio();
        audio_ready = false;
    }

    if (font)
    {
        al_destroy_font(font);
        font = NULL;
    }
}

// ---------------------------------------------------------------
// kit de interface: conversao, painel, botao, indicadores e linhas
// ---------------------------------------------------------------

// pixel da janela para pixel logico do canvas
float to_logical_x(float window_x)
{
    return (window_x - screen_off_x) / screen_scale;
}

float to_logical_y(float window_y)
{
    return (window_y - screen_off_y) / screen_scale;
}

// painel: fundo chapado e moldura de 1 px alinhada ao pixel
void draw_panel(float x, float y, float w, float h, ALLEGRO_COLOR fill, ALLEGRO_COLOR border)
{
    al_draw_filled_rectangle(x, y, x + w, y + h, fill);
    al_draw_rectangle(x + 0.5f, y + 0.5f, x + w - 0.5f, y + h - 0.5f, border, 1.0f);
}

// regua fina sob o titulo de uma tela
void draw_rule(float center_x, float y, float w, ALLEGRO_COLOR color)
{
    al_draw_line(floorf(center_x - w / 2.0f), y + 0.5f,
        floorf(center_x + w / 2.0f), y + 0.5f, color, 1.0f);
}

// botao de interface: retangulo, rotulo e os estados visuais
typedef struct
{
    float x;
    float y;
    float w;
    float h;
    const char* label;
} UiButton;

// 0 normal, 1 foco do teclado, 2 cursor em cima
void draw_button(const UiButton* button, int state)
{
    ALLEGRO_COLOR border = UI_DIM;
    ALLEGRO_COLOR label = UI_DIM;

    if (state >= 1)
    {
        border = UI_AMBER;
        label = UI_TEXT;
    }

    if (state == 2)
    {
        al_draw_filled_rectangle(button->x, button->y, button->x + button->w,
            button->y + button->h, UI_HOVER);
    }

    al_draw_rectangle(button->x + 0.5f, button->y + 0.5f,
        button->x + button->w - 0.5f, button->y + button->h - 0.5f, border, 1.0f);
    al_draw_text(font, label, floorf(button->x + button->w / 2.0f),
        floorf(button->y + (button->h - (float)al_get_font_line_height(font)) / 2.0f),
        ALLEGRO_ALIGN_CENTRE, button->label);
}

// ponto em pixel logico dentro do retangulo do botao
int button_hit(const UiButton* button, float x, float y)
{
    return x >= button->x && x <= button->x + button->w
        && y >= button->y && y <= button->y + button->h;
}

// botoes da tela atual: desenho e clique leem a mesma lista
const UiButton* screen_buttons(int* count)
{
    static const UiButton menu_items[] = {
        { (float)BUTTON_X - 180.0f, (float)BUTTON_Y, (float)BUTTON_W, (float)BUTTON_H, "INICIAR" },
        { (float)BUTTON_X, (float)BUTTON_Y, (float)BUTTON_W, (float)BUTTON_H, "COMO JOGAR" },
        { (float)BUTTON_X + 180.0f, (float)BUTTON_Y, (float)BUTTON_W, (float)BUTTON_H, "RECORDES" }
    };
    static const UiButton pause_items[] = {
        { 152.0f, 222.0f, 160.0f, 28.0f, "CONTINUAR" },
        { 328.0f, 222.0f, 160.0f, 28.0f, "COMO JOGAR" }
    };
    static const UiButton results_items[] = {
        { 240.0f, 256.0f, 160.0f, 28.0f, "CONTINUAR" }
    };
    static const UiButton victory_items[] = {
        { 240.0f, 276.0f, 160.0f, 28.0f, "NOVA PARTIDA" }
    };
    static const UiButton over_items[] = {
        { 240.0f, 216.0f, 160.0f, 28.0f, "NOVA PARTIDA" }
    };

    *count = 1;

    switch (game_get_scene())
    {
        case GAME_SCENE_MENU:
            *count = 3;
            return menu_items;

        case GAME_SCENE_PAUSE:
            *count = 2;
            return pause_items;

        case GAME_SCENE_RESULTS:
            return results_items;

        case GAME_SCENE_VICTORY:
            return victory_items;

        case GAME_SCENE_GAME_OVER:
            return over_items;

        default:
            *count = 0;
            return NULL;
    }
}

// desenha os botoes da tela com foco do teclado e cursor
void draw_buttons(const UiButton* buttons, int count)
{
    float lx = to_logical_x(mouse_x);
    float ly = to_logical_y(mouse_y);
    int i;

    if (buttons == NULL)
    {
        return;
    }

    for (i = 0; i < count; i++)
    {
        int state = i == ui_focus ? 1 : 0;

        if (button_hit(&buttons[i], lx, ly))
        {
            state = 2;
        }

        draw_button(&buttons[i], state);
    }
}

// anda o foco entre os botoes da tela, dando a volta nas pontas
void ui_move_focus(int step)
{
    int count;
    const UiButton* buttons = screen_buttons(&count);

    if (buttons == NULL || count <= 1)
    {
        return;
    }

    ui_focus = (ui_focus + step + count) % count;
}

// aciona o botao da tela atual: o efeito e da cena, nao do botao
void activate_button(int index)
{
    int count;
    const UiButton* buttons = screen_buttons(&count);

    if (buttons == NULL || index < 0 || index >= count)
    {
        return;
    }

    switch (game_get_scene())
    {
        case GAME_SCENE_MENU:
            // COMO JOGAR abre a cena propria, sempre na primeira pagina
            if (index == 1)
            {
                howto_page = 0;
                game_set_scene(GAME_SCENE_HOWTO);
                break;
            }

            // RECORDES abre o painel do Top 10 por cima do menu (#197)
            if (index == 2)
            {
                ranking_panel_open = true;
                break;
            }

            reset_match();
            start_opening();
            game_set_scene(GAME_SCENE_BRIEFING);
            break;

        case GAME_SCENE_PAUSE:
            // COMO JOGAR: a partida fica parada e a volta cai na pausa
            if (index == 1)
            {
                howto_page = 0;
                game_set_scene(GAME_SCENE_HOWTO);
                break;
            }

            game_set_scene(GAME_SCENE_GAME);
            break;

        case GAME_SCENE_RESULTS:
            // com duas fases, continuar avança; na última, conclui a run (#M4)
            if (has_next_phase())
            {
                start_next_phase();
                game_set_scene(GAME_SCENE_GAME);
            }
            else
            {
                // so a vitoria grava (#34): o nome e pedido quando a pontuacao
                // entra no Top 10 (#81)
                name_entry_active = ranking_qualifies(score);
                name_entry[0] = '\0';
                name_entry_chars = 0;
                ranking_highlight = -1;
                ranking_save_failed = false;
                game_set_scene(GAME_SCENE_VICTORY);
            }
            break;

        case GAME_SCENE_VICTORY:
        case GAME_SCENE_GAME_OVER:
            // briefing antes de toda partida, pulavel com ESC
            reset_match();
            start_opening();
            game_set_scene(GAME_SCENE_BRIEFING);
            break;

        default:
            break;
    }
}

// barra da recarga do ataque: segmento cheio quando o disparo esta liberado
void draw_cooldown_bar(float x, float y, float fraction)
{
    int i;
    int checks = (int)floorf((1.0f - fraction) * (float)UI_BAR_SEGMENTS + 0.5f);

    for (i = 0; i < UI_BAR_SEGMENTS; i++)
    {
        float sx = x + (float)i * (float)(UI_SEG_W + UI_SEG_GAP);
        ALLEGRO_COLOR color = i < checks ? UI_AMBER : UI_SLOT;

        al_draw_filled_rectangle(sx, y, sx + (float)UI_SEG_W, y + (float)UI_SEG_H, color);
    }
}

// linha de rotulo a esquerda e valor a direita
void draw_stat(float x, float y, float right, const char* label, const char* value,
    ALLEGRO_COLOR color)
{
    al_draw_text(font, UI_DIM, x, y, 0, label);
    al_draw_text(font, color, right, y, ALLEGRO_ALIGN_RIGHT, value);
}

// ---------------------------------------------------------------
// painel do scanner: um instrumento, nao uma linha de log
// ---------------------------------------------------------------

void draw_scan_swatch(Species species, float x, float y)
{
    // célula e vírus não têm coloração de Gram: sem amostra (#M4)
    if (species == SPECIES_CELL || species == SPECIES_VIRUS
        || species_is_fungus(species))
    {
        return;
    }

    al_draw_filled_rectangle(x, y + 1.0f, x + 8.0f, y + 9.0f,
        species == SPECIES_GRAM_POSITIVE ? UI_PURPLE : UI_PINK);
}

// a ficha precisa do y do agente na tela, definido mais abaixo com a camera
float view_y(float y);

// sprites dos agentes, definidos mais abaixo junto do desenho do mundo
ALLEGRO_BITMAP* species_sprite(Species species, int variant);
ALLEGRO_BITMAP* bacteria_sprite(Shape shape, int variant);

// alvo da ficha do scanner: o agente sob o cursor ou a ultima leitura, enquanto
// ela segue ativa e ao alcance do scanner; sem alvo, a ficha some (#243)
int scan_card_target(bool* from_cursor)
{
    float lx = to_logical_x(mouse_x);
    float ly = to_logical_y(mouse_y);
    float dx;
    float dy;
    int target;

    *from_cursor = false;

    // cursor sobre o painel do HUD nao tem alvo: o clique ali nao escaneia
    if (lx >= 0.0f && lx < (float)SCREEN_W && ly >= 0.0f && ly < (float)PLAY_H)
    {
        target = pick_target_at(lx + camera.x, ly + camera.y);

        if (target != -1)
        {
            *from_cursor = true;
            return target;
        }
    }

    target = last_scan_target();

    if (target == -1 || !agents[target].active)
    {
        return -1;
    }

    dx = agents[target].x - player.x;
    dy = agents[target].y - player.y;

    if (dx * dx + dy * dy > (float)(SCAN_RANGE * SCAN_RANGE))
    {
        return -1;
    }

    return target;
}

// a ficha fica embaixo da faixa do mundo e sobe quando o alvo esta na metade
// de baixo, para nunca cobrir o agente que ela descreve (#243)
float scan_card_y(int target)
{
    if (view_y(agents[target].y) > (float)PLAY_H / 2.0f)
    {
        return (float)CARD_MARGIN;
    }

    return (float)(PLAY_H - CARD_MARGIN - CARD_H);
}

// retrato 24x24 da ficha: silhueta antes da 1a leitura, como no mundo; a
// bacteria lida usa o sprite cinza da forma, tingido pelo Gram na 2a (SC-01)
void draw_scan_portrait(int target, float x, float y)
{
    ALLEGRO_BITMAP* bmp;
    ALLEGRO_COLOR tint = al_map_rgb_f(1.0f, 1.0f, 1.0f);

    if (agents[target].layers == 0)
    {
        bmp = silhouette_sprite;
    }
    else if (species_is_bacterium(agents[target].species))
    {
        bmp = bacteria_sprite(agents[target].shape, agents[target].variant);

        if (agents[target].layers >= MAX_LAYERS)
        {
            tint = agents[target].species == SPECIES_GRAM_POSITIVE ? GRAM_POS_TINT : GRAM_NEG_TINT;
        }
    }
    else
    {
        bmp = species_sprite(agents[target].species, agents[target].variant);
    }

    al_draw_tinted_scaled_bitmap(bmp, tint, 0.0f, 0.0f,
        (float)al_get_bitmap_width(bmp), (float)al_get_bitmap_height(bmp),
        x, y, (float)CARD_PORTRAIT, (float)CARD_PORTRAIT, 0);
}

// um marcador de camada: quadrado 6x6 e o rotulo; devolve o x depois dele
float draw_scan_marker(float x, float y, ALLEGRO_COLOR color, bool lit, const char* label)
{
    al_draw_filled_rectangle(x, y + 1.0f, x + (float)CARD_MARK, y + 1.0f + (float)CARD_MARK, color);
    al_draw_text(font, lit ? UI_TEXT : UI_DIM, x + (float)CARD_MARK + 4.0f, y, 0, label);

    return x + (float)CARD_MARK + 4.0f + (float)al_get_text_width(font, label);
}

void draw_scan_card(int target, bool from_cursor, float card_y)
{
    int text_x = CARD_TEXT_X;
    int row_state = (int)card_y + 6;
    int row_info = row_state + CARD_ROW_H;
    int row_action = row_info + CARD_ROW_H;
    const char* state;
    const char* category;
    const char* source;
    const char* second_label;
    ALLEGRO_COLOR source_color;
    ALLEGRO_COLOR second_color;
    float marks_x;
    bool is_bacteria = species_is_bacterium(agents[target].species);

    draw_panel(8.0f, card_y, (float)SCREEN_W - 16.0f, (float)CARD_H, UI_PANEL, UI_FRAME);
    draw_scan_portrait(target, 14.0f, card_y + 5.0f);

    // de onde vem a leitura: o cursor manda, a ultima leitura e o retorno (#199)
    if (from_cursor)
    {
        source = " · alvo sob o cursor";
        source_color = UI_CYAN;
    }
    else
    {
        source = " · última leitura";
        source_color = UI_DIM;
    }

    if (agents[target].layers == 0)
    {
        state = "Alvo detectado";
    }
    else if (agents[target].layers < MAX_LAYERS)
    {
        state = "Leitura incompleta";
    }
    else
    {
        state = "Identificação concluída";
    }

    // a categoria so e desenhada sem leitura, entao a celula e sempre "celula"
    if (agents[target].species == SPECIES_CELL)
    {
        category = "célula";
    }
    else if (agents[target].species == SPECIES_VIRUS)
    {
        category = "vírus";
    }
    else if (species_is_fungus(agents[target].species))
    {
        category = "fungo";
    }
    else
    {
        category = "bactéria";
    }

    al_draw_text(font, UI_AMBER, (float)text_x, (float)row_state, 0, state);
    al_draw_text(font, source_color, (float)(text_x + al_get_text_width(font, state)),
        (float)row_state, 0, source);
    // a categoria so aparece sem leitura: depois, as linhas 1 e 2 ja dizem o
    // tipo e a linha de estado precisa do espaco para os marcadores
    marks_x = (float)(SCREEN_W - 16);

    if (agents[target].layers == 0)
    {
        al_draw_text(font, UI_TEXT, (float)(SCREEN_W - 16), (float)row_state,
            ALLEGRO_ALIGN_RIGHT, category);
        marks_x = marks_x - (float)(al_get_text_width(font, category) + 8);
    }

    // marcadores das 2 camadas, encostados a direita: forma acende na 1a;
    // o 2o acende na 2a e fica verde com a identificacao (SC-11 a SC-13)
    second_label = is_bacteria ? "Gram e espécie" : "estrutura";
    marks_x = marks_x
        - (float)(2 * (CARD_MARK + 4) + 8)
        - (float)al_get_text_width(font, "forma") - (float)al_get_text_width(font, second_label);
    second_color = UI_SLOT;

    if (agents[target].layers >= MAX_LAYERS)
    {
        second_color = agents[target].identified ? UI_GREEN : UI_AMBER;
    }

    marks_x = draw_scan_marker(marks_x, (float)row_state,
        agents[target].layers >= 1 ? UI_AMBER : UI_SLOT, agents[target].layers >= 1, "forma");
    draw_scan_marker(marks_x + 8.0f, (float)row_state, second_color,
        agents[target].layers >= MAX_LAYERS, second_label);

    if (agents[target].layers == 0)
    {
        draw_marked_text((float)text_x, (float)row_info, UI_TEXT,
            "[botão direito]: escanear");
        al_draw_text(font, UI_DIM, (float)text_x, (float)row_action, 0,
            "cada clique revela uma camada");
        return;
    }

    // camada 1: so a forma, sem amostra de cor que entregue o grupo (#214)
    if (agents[target].layers < MAX_LAYERS)
    {
        al_draw_text(font, UI_TEXT, (float)(text_x + 12), (float)row_info, 0,
            layer_one_text(target));
        draw_marked_text((float)text_x, (float)row_action, UI_DIM,
            is_bacteria
                ? "[botão direito] de novo: revela a coloração de Gram"
                : "[botão direito] de novo: conclui a leitura");
        return;
    }

    // camada 2: coloracao, estrutura e o que a celula e, sem indicar a arma (#214)
    draw_scan_swatch(agents[target].species, (float)text_x, (float)row_info);
    draw_marked_text((float)(text_x + 12), (float)row_info, UI_WARM,
        layer_two_text(target));
    al_draw_text(font, UI_WARM, (float)text_x, (float)row_action, 0,
        layer_two_reason(target));
}

// ---------------------------------------------------------------
// menu inicial
// ---------------------------------------------------------------


void draw_menu()
{
    char record[48];
    int count;

    const UiButton* buttons = screen_buttons(&count);

    // tempo real: o menu nao tem match_time
    double now = al_get_time();

    /* Fundo: vaso sanguineo com hemacias deslizando para a esquerda */
    if (menu_background)
    {
        al_draw_bitmap(menu_background, 0.0f, 0.0f, 0);
    }

    if (menu_cells)
    {
        float offset = (float)fmod(now * 8.0, (double)SCREEN_W);

        al_draw_bitmap(menu_cells, -offset, 0.0f, 0);
        al_draw_bitmap(menu_cells, (float)SCREEN_W - offset, 0.0f, 0);
    }

    /* Titulo */
    if (title_bitmap)
    {
        int tw = al_get_bitmap_width(title_bitmap);
        int th = al_get_bitmap_height(title_bitmap);

        al_draw_scaled_bitmap(
            title_bitmap,
            0.0f,
            0.0f,
            (float)tw,
            (float)th,
            floorf(
                ((float)SCREEN_W -
                    (float)(tw * TITLE_SCALE)) / 2.0f
            ),
            26.0f,
            (float)(tw * TITLE_SCALE),
            (float)(th * TITLE_SCALE),
            0
        );

        // Nanobo parado, flutuando a direita do titulo
        if (nanobo)
        {
            float title_right = floorf(((float)SCREEN_W + (float)(tw * TITLE_SCALE)) / 2.0f);
            int frame = (int)(now / 0.22) % 4;
            float float_y = 2.0f * (float)sin(2.0 * ALLEGRO_PI * now / 2.0);

            al_draw_bitmap_region(nanobo, (float)(frame * PLAYER_FRAME),
                (float)(PLAYER_FACING_DOWN * PLAYER_FRAME),
                (float)PLAYER_FRAME, (float)PLAYER_FRAME,
                title_right + 24.0f, 26.0f + float_y, 0);
        }
    }

    al_draw_text(
        font,
        UI_DIM,
        SCREEN_W / 2,
        52,
        ALLEGRO_ALIGN_CENTRE,
        "DEFENDA O ORGANISMO"
    );

    /* =========================================
       BOTAO E INSTRUCOES
       ========================================= */

    draw_buttons(buttons, count);

    draw_marked_text_aligned(SCREEN_W / 2, BUTTON_Y + 36, UI_DIM, ALLEGRO_ALIGN_CENTRE,
        "[setas] escolhem · [enter] confirma");

    // o melhor do Top 10, com o nome (#34)
    if (ranking_count() > 0)
    {
        snprintf(record, sizeof(record), "recorde %d · %s", hi_score(), ranking_entry(0)->name);
    }
    else
    {
        snprintf(record, sizeof(record), "nenhum recorde ainda");
    }
    al_draw_text(font, UI_DIM, SCREEN_W / 2, BUTTON_Y + 52, ALLEGRO_ALIGN_CENTRE, record);

    draw_marked_text_aligned(SCREEN_W - HUD_MARGIN, 336, UI_DIM, ALLEGRO_ALIGN_RIGHT,
        "[F11] tela cheia");

    // painel do Top 10 por cima do menu, aberto pelo RECORDES (#197)
    if (ranking_panel_open)
    {
        al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H,
            UI_VEIL(0.85f));
        // painel de 184 px: termina em 244 e o rodape fica acima dos botoes (y 261)
        draw_ranking_panel(177.0f, 60.0f, 286.0f, -1, true);
        draw_marked_text_aligned(SCREEN_W / 2, 250, UI_DIM, ALLEGRO_ALIGN_CENTRE, "[enter] ou [esc] fecha");
    }
}


// sprite de cada especie e variante, alinhado com as constantes do entities.h
// variante 1 usa o segundo desenho da especie; qualquer outra, o primeiro (#211)
ALLEGRO_BITMAP* species_sprite(Species species, int variant)
{
    // bacteria: o sprite cinza da forma; quem desenha usa o bacteria_sprite (#214)
    if (species_is_bacterium(species))
    {
        return coccus_grey[variant == 1 ? 1 : 0];
    }

    if (species == SPECIES_VIRUS)
    {
        return variant == 1 ? virus_sprite_b : virus_sprite;
    }

    // fungos da Fase 3: a levedura tem as duas variantes; colonia e esporo, uma
    if (species == SPECIES_YEAST)
    {
        return yeast_sprite[variant == 1 ? 1 : 0];
    }

    if (species == SPECIES_MOLD)
    {
        return mold_sprite;
    }

    if (species == SPECIES_SPORE)
    {
        return spore_sprite;
    }

    return variant == 1 ? cell_sprite_b : cell_sprite;
}

// sprite cinza da bacteria pela forma e variante; o grupo vem do tingimento (#214)
ALLEGRO_BITMAP* bacteria_sprite(Shape shape, int variant)
{
    int v = variant == 1 ? 1 : 0;

    if (shape == SHAPE_BACILLUS)
    {
        return bacillus_grey[v];
    }

    return coccus_grey[v];
}

// coordenada do mundo para o pixel do canvas, arredondada para nao tremer
float view_x(float x)
{
    return floorf(x - camera.x);
}

float view_y(float y)
{
    return floorf(y - camera.y);
}

// quando o destino esta fora da faixa visivel, uma marca na borda mostra o caminho (#19);
// top e bottom limitam a marca a parte livre da faixa, fora da ficha do scanner (#243)
void draw_destination_hint(float top, float bottom)
{
    float sx = view_x(DESTINATION_X);
    float sy = view_y(DESTINATION_Y);
    int label_w = al_get_text_width(font, "destino");
    int label_h = al_get_font_line_height(font);
    float margin_x = (float)label_w / 2.0f + 2.0f;
    float margin_y = 10.0f + (float)label_h + 6.0f;
    float cx;
    float cy;

    if (sx >= 0.0f && sx <= (float)SCREEN_W && sy >= 0.0f && sy <= (float)PLAY_H)
    {
        return;
    }

    cx = sx;
    cy = sy;

    if (cx < margin_x)
    {
        cx = margin_x;
    }

    if (cx > (float)SCREEN_W - margin_x)
    {
        cx = (float)SCREEN_W - margin_x;
    }

    if (cy < top + margin_y)
    {
        cy = top + margin_y;
    }

    if (cy > bottom - margin_y)
    {
        cy = bottom - margin_y;
    }

    al_draw_filled_circle(cx, cy, 6.0f, UI_AMBER);

    // na metade de baixo o rotulo sobe, para nao encostar na linha do painel
    if (cy > (top + bottom) / 2.0f)
    {
        al_draw_text(font, UI_AMBER, cx, cy - 10.0f - (float)label_h,
            ALLEGRO_ALIGN_CENTRE, "destino");
    }
    else
    {
        al_draw_text(font, UI_AMBER, cx, cy + 10.0f, ALLEGRO_ALIGN_CENTRE, "destino");
    }
}

// ---------------------------------------------------------------
// HUD: vitais, objetivo e painel do scanner em dois blocos
// ---------------------------------------------------------------

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

// tremor de cada nivel, em pixels do canvas
static const float feel_shake_power_by_level[3] = { 0.0f, 1.0f, 3.0f };

unsigned int feel_seen = 0;
float feel_shake_time = 0.0f;
float feel_shake_power = 0.0f;
float feel_flash_time = 0.0f;
float feel_ring_time = 0.0f;
float feel_ring_x = 0.0f;
float feel_ring_y = 0.0f;
float feel_impact_time = 0.0f;
float feel_impact_x = 0.0f;
float feel_impact_y = 0.0f;
bool feel_impact_dull = false;

// linha da folha de impactos: 0 dose certa, 1 sem efeito, 2 eliminado (#242)
int feel_impact_kind = 0;
float feel_pop_time = 0.0f;
float feel_pop_x = 0.0f;
float feel_pop_y = 0.0f;
double feel_hit_stop = 0.0;
FeelParticle feel_particles[FEEL_PARTICLES];

// intensidade de cada evento: quanto mais decide a partida, mais forte
FeelLevel feel_level(SfxEvent kind)
{
    if (kind == SFX_EVENT_DAMAGE || kind == SFX_EVENT_NEUTRALIZED || kind == SFX_EVENT_PHASE_DONE)
    {
        return FEEL_STRONG;
    }

    if (kind == SFX_EVENT_HIT || kind == SFX_EVENT_WRONG)
    {
        return FEEL_MEDIUM;
    }

    return FEEL_LIGHT;
}

// le o evento novo do gameplay e liga a resposta dele; nao usa rand(),
// para nao mexer na sequencia que o gameplay sorteia
void feel_trigger()
{
    SfxEvent kind;
    FeelLevel level;
    float x;
    float y;
    int i;

    if (sfx_event_id() == feel_seen)
    {
        return;
    }

    feel_seen = sfx_event_id();
    kind = sfx_event_kind();
    level = feel_level(kind);
    sfx_event_position(&x, &y);

    if (feel_shake_power_by_level[level] >= feel_shake_power || feel_shake_time <= 0.0f)
    {
        feel_shake_power = feel_shake_power_by_level[level];
        feel_shake_time = FEEL_SHAKE_TIME;
    }

    if (kind == SFX_EVENT_SCAN)
    {
        // anel do scan sai do nanobo ate o alcance do scanner
        feel_ring_time = FEEL_RING_TIME;
        feel_ring_x = player.x;
        feel_ring_y = player.y;
    }
    else if (kind == SFX_EVENT_HIT || kind == SFX_EVENT_WRONG)
    {
        // dose certa pisca claro; sem efeito pisca apagado
        feel_impact_time = FEEL_IMPACT_TIME;
        feel_impact_x = x;
        feel_impact_y = y;
        feel_impact_dull = kind == SFX_EVENT_WRONG;
        feel_impact_kind = feel_impact_dull ? 1 : 0;
    }
    else if (kind == SFX_EVENT_DAMAGE)
    {
        feel_flash_time = FEEL_FLASH_TIME;
    }
    else if (kind == SFX_EVENT_NEUTRALIZED)
    {
        feel_impact_time = FEEL_IMPACT_TIME;
        feel_impact_x = x;
        feel_impact_y = y;
        feel_impact_dull = false;
        feel_impact_kind = 2;
        feel_pop_time = FEEL_POP_TIME;
        feel_pop_x = x;
        feel_pop_y = y;
        feel_hit_stop = FEEL_HIT_STOP;

        for (i = 0; i < FEEL_PARTICLES; i++)
        {
            float angle = (float)i * 2.0f * ALLEGRO_PI / (float)FEEL_PARTICLES;
            float speed = 70.0f + (float)(i % 3) * 25.0f;

            feel_particles[i].x = x;
            feel_particles[i].y = y;
            feel_particles[i].vx = cosf(angle) * speed;
            feel_particles[i].vy = sinf(angle) * speed;
            feel_particles[i].life = FEEL_PARTICLE_TIME;
        }
    }
}

// os efeitos decaem sozinhos; nada aqui toca no gameplay
void feel_update(float dt)
{
    int i;

    feel_shake_time = fmaxf(feel_shake_time - dt, 0.0f);
    feel_flash_time = fmaxf(feel_flash_time - dt, 0.0f);
    feel_ring_time = fmaxf(feel_ring_time - dt, 0.0f);
    feel_impact_time = fmaxf(feel_impact_time - dt, 0.0f);
    feel_pop_time = fmaxf(feel_pop_time - dt, 0.0f);

    for (i = 0; i < FEEL_PARTICLES; i++)
    {
        if (feel_particles[i].life <= 0.0f)
        {
            continue;
        }

        feel_particles[i].x = feel_particles[i].x + feel_particles[i].vx * dt;
        feel_particles[i].y = feel_particles[i].y + feel_particles[i].vy * dt;
        feel_particles[i].vx = feel_particles[i].vx * 0.92f;
        feel_particles[i].vy = feel_particles[i].vy * 0.92f;
        feel_particles[i].life = feel_particles[i].life - dt;
    }
}

// deslocamento do tremor: decai ate zero e so mexe no desenho do mundo
float feel_offset_x()
{
    if (feel_shake_time <= 0.0f)
    {
        return 0.0f;
    }

    return roundf(feel_shake_power * (feel_shake_time / FEEL_SHAKE_TIME) * sinf(feel_shake_time * 95.0f));
}

float feel_offset_y()
{
    if (feel_shake_time <= 0.0f)
    {
        return 0.0f;
    }

    return roundf(feel_shake_power * (feel_shake_time / FEEL_SHAKE_TIME) * cosf(feel_shake_time * 71.0f));
}

// efeitos presos ao mundo: anel do scan, impacto, particulas e "+10"
void draw_feel_world()
{
    int i;
    char pop[16];

    if (feel_ring_time > 0.0f)
    {
        float t = 1.0f - feel_ring_time / FEEL_RING_TIME;
        float a = feel_ring_time / FEEL_RING_TIME;
        float r = (float)PLAYER_RADIUS + t * ((float)SCAN_RANGE - (float)PLAYER_RADIUS);

        al_draw_circle(view_x(feel_ring_x), view_y(feel_ring_y), r,
            al_map_rgba_f(0.435f * a, 0.906f * a, 0.949f * a, a), 2.0f);
    }

    // impacto do resultado: dose certa, sem efeito ou eliminado, 4 quadros (#242)
    if (feel_impact_time > 0.0f)
    {
        int frame = (int)((1.0f - feel_impact_time / FEEL_IMPACT_TIME) * 4.0f);

        if (frame > 3)
        {
            frame = 3;
        }

        al_draw_bitmap_region(impacts_sheet, (float)(frame * IMPACT_FRAME),
            (float)(feel_impact_kind * IMPACT_FRAME), (float)IMPACT_FRAME, (float)IMPACT_FRAME,
            view_x(feel_impact_x) - (float)IMPACT_FRAME / 2.0f,
            view_y(feel_impact_y) - (float)IMPACT_FRAME / 2.0f, 0);
    }

    for (i = 0; i < FEEL_PARTICLES; i++)
    {
        float a;

        if (feel_particles[i].life <= 0.0f)
        {
            continue;
        }

        a = feel_particles[i].life / FEEL_PARTICLE_TIME;
        al_draw_filled_rectangle(view_x(feel_particles[i].x) - 1.0f, view_y(feel_particles[i].y) - 1.0f,
            view_x(feel_particles[i].x) + 1.0f, view_y(feel_particles[i].y) + 1.0f,
            al_map_rgba_f(1.0f * a, 0.906f * a, 0.659f * a, a));
    }

    if (feel_pop_time > 0.0f)
    {
        float a = feel_pop_time / FEEL_POP_TIME;
        float rise = (1.0f - a) * 24.0f;

        snprintf(pop, sizeof(pop), "+%d", SCORE_PER_THREAT);
        al_draw_text(font, al_map_rgba_f(0.373f * a, 0.839f * a, 0.627f * a, a),
            view_x(feel_pop_x), view_y(feel_pop_y) - (float)AGENT_RADIUS - 12.0f - rise,
            ALLEGRO_ALIGN_CENTRE, pop);
    }
}

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

// uma entrada: cabecalho, paginas de ate 3 linhas e o termo entre chaves
typedef struct
{
    const char* header;
    int pages;
    const char* lines[DIALOG_MAX_PAGES][3];
    bool portrait;
} DialogEntry;

// anotacoes do diario e interludios (#222), registrados em textos-do-jogo.md §3.1
static const DialogEntry diary_entries[CARD_COUNT] =
{
    [CARD_INTERLUDE_2] =
    {
        "Enquanto isso, no hospital", 2,
        {
            {
                "A infecção no sangue de Léo cedeu. Mas, na enfermaria,",
                "ele pegou um {vírus respiratório}.",
                NULL
            },
            {
                "O Nanobô segue para as {vias aéreas} dele, onde o vírus",
                "infecta as células que forram os brônquios.",
                NULL
            }
        },
        false
    },
    [CARD_INTERLUDE_3] =
    {
        "Enquanto isso, no hospital", 3,
        {
            {
                "O vírus foi contido, mas Léo segue com as {defesas} fracas.",
                NULL,
                NULL
            },
            {
                "{Esporos de bolor}, que estão no ar,",
                "chegam ao pulmão dele.",
                NULL
            },
            {
                "O Nanobô desce até o {pulmão} para agir antes que",
                "o fungo se instale.",
                NULL
            }
        },
        false
    },
    [CARD_GRAM] =
    {
        "Coloração de Gram", 4,
        {
            {
                "Li uma bactéria até o fim! A forma não basta:",
                "existe coco <p:Gram+> e coco <r:Gram->. Quem mostra o grupo",
                "é a {coloração de Gram}, um corante de laboratório."
            },
            {
                "O corante roxo fica preso ou sai conforme a parede.",
                "<p:Gram+>: parede grossa de {peptidoglicano}, o roxo fica.",
                "<r:Gram->: parede fina e membrana externa; perde o roxo, fica rosa."
            },
            {
                "Anotado: no jogo, todas aparecem juntas no sangue para",
                "{simplificar}. A Moraxella vive nas vias respiratórias,",
                "e o Clostridium tetani entra por ferimentos."
            },
            {
                "Minhas armas são {regra do jogo}, inspiradas em antibióticos",
                "reais, como a penicilina. Na vida real há exceções:",
                "muitas cepas de S. aureus resistem à penicilina."
            }
        },
        true
    },
    [CARD_RESISTANCE] =
    {
        "Resistência e Reserva", 4,
        {
            {
                "Errei... anotado! Dei uma dose e demorei para dar a outra.",
                "Esta bactéria {resistiu}: o mesmo antibiótico não age",
                "mais nela nem nas filhas dela."
            },
            {
                "Regra do jogo: duas doses, a segunda em até 6 segundos.",
                "Na vida real, um tratamento mal feito faz {seleção}:",
                "morrem as sensíveis e sobram as que já resistiam."
            },
            {
                "As sobreviventes se multiplicam e passam a resistência",
                "para as filhas. Com antibiótico de verdade, a regra é:",
                "{siga a prescrição até o fim}."
            },
            {
                "A {Reserva} age na resistente, mas tem 2 cargas por fase.",
                "Até ela pode deixar de funcionar se for usada à toa.",
                "Vou guardar para quando precisar!"
            }
        },
        true
    },
    [CARD_FLAGELLUM] =
    {
        "Flagelo", 3,
        {
            {
                "Ei! Esse bacilo veio atrás de mim e me atacou!",
                "Ele tem {flagelo}, um chicote que gira e o faz nadar.",
                NULL
            },
            {
                "O coco não nada sozinho: ele {gruda} no tecido",
                "e fica ali, soltando toxinas que ferem as células.",
                NULL
            },
            {
                "Anotado: na natureza há {exceções}. Aqui, todo bacilo",
                "do jogo nada e todo coco gruda.",
                NULL
            }
        },
        true
    },
    [CARD_BIOFILM] =
    {
        "Biofilme", 3,
        {
            {
                "Fiquei preso numa gosma! Bactérias grudadas fazem",
                "uma camada pegajosa: o {biofilme}.",
                NULL
            },
            {
                "Dentro dele, o antibiótico {age menos}: entra devagar,",
                "e as bactérias de dentro funcionam mais devagar.",
                NULL
            },
            {
                "Regra do jogo: no biofilme a bactéria pede uma {dose} a mais,",
                "e eu ando devagar.",
                NULL
            }
        },
        true
    },
    [CARD_ENDOTOXIN] =
    {
        "Endotoxina", 3,
        {
            {
                "Ai, esta nuvem me fere! Ela veio de uma <r:Gram-> eliminada.",
                "A membrana externa da <r:Gram-> guarda uma {toxina}.",
                NULL
            },
            {
                "Quando a <r:Gram-> morre ou se divide, essa toxina se solta",
                "e causa {inflamação} e febre no corpo.",
                NULL
            },
            {
                "Regra do jogo: a nuvem me fere enquanto dura.",
                "Anotado: {sair da nuvem} bem rápido!",
                NULL
            }
        },
        true
    },
    [CARD_VIRUS] =
    {
        "Vírus não é célula", 4,
        {
            {
                "Um {vírus}! Ele não é célula: não tem parede,",
                "nem metabolismo, nem ribossomos próprios.",
                NULL
            },
            {
                "Ele só se multiplica {dentro de uma célula},",
                "usando os ribossomos dela.",
                NULL
            },
            {
                "{Antibiótico} não age nele: não trata gripe nem resfriado.",
                "Se gastei um tiro de antibiótico aqui, errei... anotado!",
                NULL
            },
            {
                "A <wN:Neutralização> leva {anticorpos}: eles se prendem ao vírus",
                "livre e o impedem de entrar nas células.",
                "No corpo, quem faz anticorpos são os linfócitos B."
            }
        },
        true
    },
    [CARD_CYCLE] =
    {
        "Lisogênico e lítico", 4,
        {
            {
                "Esta célula do Léo tem um vírus escondido dentro!",
                "{Lisogênico}: o vírus fica na célula sem destruí-la ainda.",
                NULL
            },
            {
                "{Lítico}: o vírus faz cópias, a célula se rompe",
                "e libera vírus novos, que infectam outras células.",
                NULL
            },
            {
                "O {Ataque T} destrói a célula infectada antes que ela",
                "se rompa, como faz o linfócito T no corpo.",
                "No jogo, ele custa um pouco dos vitais do Léo."
            },
            {
                "Anotado: lisogênico e lítico foram descritos em vírus de",
                "bactérias, os {bacteriófagos}. Aqui servem de comparação.",
                "Em nós, o mais parecido é o vírus da catapora, nos nervos."
            }
        },
        true
    },
    [CARD_WEAKENED] =
    {
        "Organismo enfraquecido", 3,
        {
            {
                "Estes {esporos} são de Aspergillus, um bolor comum no ar.",
                "A maioria das pessoas respira esporos e não adoece.",
                NULL
            },
            {
                "Fungos {oportunistas} se instalam com mais facilidade",
                "em quem está com a defesa enfraquecida.",
                "O risco vem da defesa fraca, não de respirar mais esporos."
            },
            {
                "O Léo está com a {defesa} fraca. Regra do jogo: aqui,",
                "defesa mais fraca aparece como mais esporos no chão.",
                NULL
            }
        },
        true
    },
    [CARD_FUNGUS] =
    {
        "Fungo", 4,
        {
            {
                "Um fungo! Ele tem núcleo, como as células do Léo:",
                "é um {eucarionte}, bem diferente de uma bactéria.",
                NULL
            },
            {
                "A parede dele é de {quitina}, não de peptidoglicano.",
                "Antibiótico de bactéria não tem alvo nele.",
                "Se gastei antibiótico aqui, errei... anotado!"
            },
            {
                "A membrana dele tem {ergosterol}; a nossa tem colesterol.",
                "Essa diferença é o alvo de muitos antifúngicos.",
                NULL
            },
            {
                "A {Perfuração} se inspira na anfotericina B: ela se prende",
                "ao ergosterol e abre poros na membrana do fungo.",
                "Esse remédio de verdade também pode fazer mal ao paciente."
            }
        },
        true
    }
};

// estado da caixa da fila de cards e a entrada que ela mostra
DialogState card_dialog;
ConceptCard card_dialog_entry = CARD_NONE;

// marca de cor <c:texto> (27/11): devolve os bytes de "<c:" (codigo de 1 ou
// 2 letras) ou 0 se ali nao comeca uma marca
int color_mark_length(const char* c)
{
    int code = 0;

    if (*c != '<')
    {
        return 0;
    }

    while (code < 2 && c[1 + code] != '\0' && c[1 + code] != ':')
    {
        code = code + 1;
    }

    if (code == 0 || c[1 + code] != ':')
    {
        return 0;
    }

    return code + 2;
}

// cor da marca: p Gram+, r Gram-, g ganho, x perda; wR wD wS wN wT wF a arma
// na mesma cor da mira e do tiro
ALLEGRO_COLOR laser_color(Laser laser);

ALLEGRO_COLOR color_mark_color(const char* c)
{
    if (c[1] == 'p')
    {
        return UI_PURPLE;
    }
    if (c[1] == 'r')
    {
        return UI_PINK;
    }
    if (c[1] == 'g')
    {
        return UI_GREEN;
    }
    if (c[1] == 'x')
    {
        return UI_RED;
    }

    switch (c[2])
    {
    case 'R':
        return laser_color(LASER_GRAM_POSITIVE);
    case 'D':
        return laser_color(LASER_GRAM_NEGATIVE);
    case 'S':
        return laser_color(LASER_RESERVE);
    case 'N':
        return laser_color(LASER_NEUTRALIZE);
    case 'T':
        return laser_color(LASER_LYSE);
    default:
        return laser_color(LASER_PERFORATE);
    }
}

// caracteres que aparecem na tela: conta por letra utf-8 e pula chaves,
// colchetes e as marcas de cor
int dialog_visible_chars(const char* line)
{
    int count = 0;
    const unsigned char* c = (const unsigned char*)line;

    if (line == NULL)
    {
        return 0;
    }

    for (; *c != '\0'; c++)
    {
        int skip = color_mark_length((const char*)c);

        if (skip > 0)
        {
            c = c + skip - 1;
            continue;
        }

        if (*c != '{' && *c != '}' && *c != '[' && *c != ']' && *c != '>' && (*c & 0xC0) != 0x80)
        {
            count = count + 1;
        }
    }

    return count;
}

// abre a caixa na pagina 1 com a contagem de letras de cada pagina
void dialog_open_entry(DialogState* d, const DialogEntry* e)
{
    int chars[DIALOG_MAX_PAGES] = { 0, 0, 0, 0 };
    int page;
    int line;

    for (page = 0; page < e->pages && page < DIALOG_MAX_PAGES; page++)
    {
        for (line = 0; line < 3; line++)
        {
            chars[page] = chars[page] + dialog_visible_chars(e->lines[page][line]);
        }
    }

    dialog_open(d, e->pages, chars);
}

// a entrada pendente mudou (nova, a seguinte da fila ou nenhuma): a caixa recomeca
void sync_card_dialog()
{
    ConceptCard card = pending_card();

    if (card == card_dialog_entry)
    {
        return;
    }

    card_dialog_entry = card;

    if (card != CARD_NONE)
    {
        dialog_open_entry(&card_dialog, &diary_entries[card]);
    }
}

// enter, espaco ou clique: completa, vira a pagina ou fecha a entrada
void press_card_dialog()
{
    sync_card_dialog();

    if (card_dialog_entry == CARD_NONE)
    {
        return;
    }

    dialog_press(&card_dialog);

    if (!card_dialog.open)
    {
        dismiss_card();
        sync_card_dialog();
    }
}

// esc fecha a entrada inteira; a seguinte da fila abre do inicio
void skip_card_dialog()
{
    // esc duplo nao pode fechar a entrada seguinte antes de ler
    if (card_dialog_entry != CARD_NONE && card_dialog.page_age < DIALOG_DEBOUNCE)
    {
        return;
    }

    dismiss_card();
    sync_card_dialog();
}

// cor com o alfa multiplicado (o blender do allegro usa alfa pre-multiplicado)
ALLEGRO_COLOR faded_color(ALLEGRO_COLOR color, float alpha)
{
    float r, g, b, a;

    al_unmap_rgba_f(color, &r, &g, &b, &a);
    return al_map_rgba_f(r * alpha, g * alpha, b * alpha, a * alpha);
}

// tampa de tecla: nome em ciano numa caixa de 14 px com borda ciano e fundo
// de tinta, KEY_CAP_PAD de folga de cada lado; devolve a largura ocupada
float draw_key_cap(float x, float y, const char* name, float alpha)
{
    float w = (float)(al_get_text_width(font, name) + 2 * KEY_CAP_PAD);

    // 14 px de altura: a borda nao encosta nas letras nem nos acentos e cedilhas
    al_draw_filled_rectangle(x, y - 3.0f, x + w, y + 11.0f, faded_color(UI_INK, alpha));
    al_draw_rectangle(x + 0.5f, y - 2.5f, x + w - 0.5f, y + 10.5f, faded_color(UI_KEY, alpha), 1.0f);
    al_draw_text(font, faded_color(UI_KEY, alpha), x + (float)KEY_CAP_PAD, y, 0, name);

    return w;
}

// uma linha com no maximo budget letras; o termo entre chaves sai em ambar,
// a tecla entre colchetes sai em tampa de tecla, os dois com o alfa da cor
// base, a marca <c:texto> na cor do codigo (color_mark_color) com o mesmo alfa,
// e o resto na cor base
int draw_dialog_line(const char* line, float x, float y, ALLEGRO_COLOR base, int budget)
{
    char segment[192];
    int length = 0;
    bool term = false;
    bool key = false;
    bool span = false;
    const char* c;
    float r, g, b, a;
    ALLEGRO_COLOR term_color;
    ALLEGRO_COLOR span_color;

    al_unmap_rgba_f(base, &r, &g, &b, &a);
    term_color = faded_color(UI_AMBER, a);
    span_color = base;

    for (c = line; *c != '\0'; c++)
    {
        bool lead = (*c & 0xC0) != 0x80;
        int color_mark = color_mark_length(c);
        bool mark = *c == '{' || *c == '}' || *c == '[' || *c == ']' || *c == '>' || color_mark > 0;

        if (mark || (lead && budget == 0))
        {
            segment[length] = '\0';

            if (key)
            {
                // tecla pela metade na maquina de escrever: a caixa cresce com o texto
                if (length > 0)
                {
                    x = x + draw_key_cap(x, y, segment, a);
                }
            }
            else
            {
                al_draw_text(font, term ? term_color : (span ? span_color : base), x, y, 0, segment);
                x = x + (float)al_get_text_width(font, segment);
            }
            length = 0;

            if (!mark)
            {
                return 0;
            }

            if (color_mark > 0)
            {
                // a marca de cor comeca: pula "<c:" inteiro
                span = true;
                span_color = faded_color(color_mark_color(c), a);
                c = c + color_mark - 1;
            }
            else if (*c == '>')
            {
                span = false;
            }
            else if (*c == '{' || *c == '}')
            {
                term = *c == '{';
            }
            else
            {
                key = *c == '[';
            }
            continue;
        }

        if (lead)
        {
            budget = budget - 1;
        }

        if (length < (int)sizeof(segment) - 1)
        {
            segment[length] = *c;
            length = length + 1;
        }
    }

    segment[length] = '\0';

    if (key)
    {
        if (length > 0)
        {
            draw_key_cap(x, y, segment, a);
        }
    }
    else
    {
        al_draw_text(font, term ? term_color : (span ? span_color : base), x, y, 0, segment);
    }

    return budget;
}

// linha inteira com {termo} em ambar, [tecla] em tampa e <c:texto> na cor do codigo
// (aviso de objetivo, como jogar, rodapes)
void draw_marked_text(float x, float y, ALLEGRO_COLOR base, const char* text)
{
    if (text == NULL)
    {
        return;
    }

    draw_dialog_line(text, x, y, base, dialog_visible_chars(text));
}

// largura em px da linha sem chaves, colchetes e marcas de cor, com a folga
// de cada tampa de tecla
int marked_text_width(const char* text)
{
    char plain[192];
    int length = 0;
    int caps = 0;
    const char* c;

    if (text == NULL)
    {
        return 0;
    }

    for (c = text; *c != '\0' && length < (int)sizeof(plain) - 1; c++)
    {
        int skip = color_mark_length(c);

        if (skip > 0)
        {
            c = c + skip - 1;
            continue;
        }

        if (*c == '[')
        {
            caps = caps + 1;
        }

        if (*c != '{' && *c != '}' && *c != '[' && *c != ']' && *c != '>')
        {
            plain[length] = *c;
            length = length + 1;
        }
    }

    plain[length] = '\0';
    return al_get_text_width(font, plain) + caps * 2 * KEY_CAP_PAD;
}

// linha marcada alinhada como o al_draw_text (centro ou direita em x)
void draw_marked_text_aligned(float x, float y, ALLEGRO_COLOR base, int flags, const char* text)
{
    float w = (float)marked_text_width(text);

    if (flags & ALLEGRO_ALIGN_CENTRE)
    {
        x = floorf(x - w / 2.0f);
    }
    else if (flags & ALLEGRO_ALIGN_RIGHT)
    {
        x = x - w;
    }

    draw_marked_text(x, y, base, text);
}

// idade da pagina desde que ficou completa (natural ou por toque): o triangulo pisca daqui
float dialog_complete_age(const DialogEntry* e, const DialogState* d)
{
    static const DialogEntry* last_entry = NULL;
    static int last_page = -1;
    static float complete_at = -1.0f;
    float natural = (float)d->page_chars[d->page] / DIALOG_CHARS_PER_SECOND;

    if (e != last_entry || d->page != last_page || d->page_age < complete_at)
    {
        last_entry = e;
        last_page = d->page;
        complete_at = -1.0f;
    }

    if (complete_at < 0.0f)
    {
        complete_at = fminf(d->page_age, natural);
    }

    return d->page_age - complete_at;
}

// caixa sobre a fase parada: veu, retrato, cabecalho, linhas que aparecem
// letra a letra e o rodape com n/N, o triangulo ou o fechar
void draw_dialog(const DialogEntry* e, const DialogState* d)
{
    int budget = (int)d->shown;
    int line;
    char counter[16];
    float right = (float)(DIALOG_X + DIALOG_W - DIALOG_PAD);
    float footer_y = (float)(DIALOG_Y + DIALOG_H - DIALOG_PAD - 8);

    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H,
        UI_VEIL(0.6f));
    draw_panel((float)DIALOG_X, (float)DIALOG_Y, (float)DIALOG_W, (float)DIALOG_H, UI_INK, UI_AMBER);

    // retrato: linha de frente do spritesheet, quadro ocioso 0
    if (e->portrait && nanobo)
    {
        float px = (float)(DIALOG_X + DIALOG_PAD);
        float py = (float)(DIALOG_Y + DIALOG_PAD);

        draw_panel(px - 1.0f, py - 1.0f, (float)PLAYER_FRAME + 2.0f, (float)PLAYER_FRAME + 2.0f,
            UI_PANEL, UI_FRAME);
        al_draw_bitmap_region(nanobo, 0.0f, (float)(PLAYER_FACING_DOWN * PLAYER_FRAME),
            (float)PLAYER_FRAME, (float)PLAYER_FRAME, px, py, 0);
    }

    al_draw_text(font, UI_DIM, (float)DIALOG_TEXT_X, (float)(DIALOG_Y + DIALOG_PAD), 0, e->header);

    for (line = 0; line < 3 && budget > 0; line++)
    {
        const char* text = e->lines[d->page][line];

        if (text == NULL)
        {
            break;
        }

        budget = draw_dialog_line(text, (float)DIALOG_TEXT_X,
            (float)(DIALOG_Y + DIALOG_PAD + DIALOG_LINE_H * (line + 1)), UI_TEXT, budget);
    }

    snprintf(counter, sizeof(counter), "%d/%d", d->page + 1, d->pages);
    al_draw_text(font, UI_DIM, right, footer_y, ALLEGRO_ALIGN_RIGHT, counter);

    if (!dialog_page_complete(d))
    {
        return;
    }

    // a ultima pagina fecha; as outras piscam o triangulo a cada 0,5 s
    if (dialog_last_page(d))
    {
        al_draw_text(font, UI_AMBER, right - 32.0f, footer_y, ALLEGRO_ALIGN_RIGHT, "fechar");
    }
    else if (fmodf(dialog_complete_age(e, d), 1.0f) < 0.5f)
    {
        float tx = right - 40.0f;

        al_draw_filled_triangle(tx, footer_y + 1.0f, tx + 8.0f, footer_y + 1.0f,
            tx + 4.0f, footer_y + 7.0f, UI_AMBER);
    }
}

// card pendente da fila na caixa de dialogo
void draw_concept_card()
{
    DialogEntry shown;
    char header[96];

    sync_card_dialog();

    if (card_dialog_entry == CARD_NONE)
    {
        return;
    }

    shown = diary_entries[card_dialog_entry];

    // a anotacao leva o numero da ordem em que abriu; o interludio fica sem numero
    if (card_note_number(card_dialog_entry) > 0)
    {
        snprintf(header, sizeof(header), "Nanobô · Anotação %d · %s",
            card_note_number(card_dialog_entry), diary_entries[card_dialog_entry].header);
        shown.header = header;
    }

    draw_dialog(&shown, &card_dialog);
}

// abertura (#244): ficha do hospital e Anotacao 0, na caixa, com estado proprio
// fora da fila de cards; textos aprovados em 26/11, registrados em textos-do-jogo.md §1.1
#define OPENING_ENTRIES 2

static const DialogEntry opening_entries[OPENING_ENTRIES] =
{
    {
        "Hospital · ficha do paciente", 3,
        {
            {
                "Paciente: {Léo Martins Ferreira}, 16 anos.",
                "Chegou ao hospital com febre alta.",
                NULL
            },
            {
                "Exames: uma {infecção} que se espalha pelo sangue.",
                NULL,
                NULL
            },
            {
                "Conduta: testar um {nanorrobô} médico, ainda protótipo,",
                "dentro do corpo de Léo.",
                NULL
            }
        },
        false
    },
    {
        "Nanobô · Anotação 0", 3,
        {
            {
                "{Sistema ligado}! Minha memória está vazia.",
                "Minha missão: aprender a reconhecer as ameaças e eliminá-las.",
                NULL
            },
            {
                "Meu paciente é o {Léo}, de 16 anos, com febre alta.",
                "Estou dentro do sangue dele.",
                NULL
            },
            {
                "Vou descobrir o que está {atacando} o Léo.",
                "Anoto aqui tudo o que aprender, até os meus erros.",
                NULL
            }
        },
        true
    }
};

DialogState opening_dialog;
int opening_entry = 0;

// entrar em BRIEFING (menu ou Game Over) recomeca a abertura da ficha
void start_opening(void)
{
    opening_entry = 0;
    dialog_open_entry(&opening_dialog, &opening_entries[0]);
}

// enter, espaco ou clique: avanca; o fim da ultima entrada entra na Fase 1
void press_opening()
{
    dialog_press(&opening_dialog);

    if (opening_dialog.open)
    {
        return;
    }

    if (opening_entry + 1 < OPENING_ENTRIES)
    {
        opening_entry = opening_entry + 1;
        dialog_open_entry(&opening_dialog, &opening_entries[opening_entry]);
    }
    else
    {
        game_set_scene(GAME_SCENE_GAME);
    }
}

// fundo escuro, a caixa da entrada atual e o rodape (BR-04)
void draw_briefing()
{
    // o quarto do Léo atrás da ficha; o véu de 60% vem do draw_dialog
    if (hospital_opening)
    {
        al_draw_bitmap(hospital_opening, 0.0f, 0.0f, 0);
    }
    draw_dialog(&opening_entries[opening_entry], &opening_dialog);

    draw_marked_text_aligned(SCREEN_W / 2, 24, UI_DIM, ALLEGRO_ALIGN_CENTRE,
        "[enter] ou [clique] avança · [esc] pula");
}

// moldura da barra de vida e os segmentos cheios tingidos (10 de 5x4)
void draw_life_bar(ALLEGRO_BITMAP* frame, float x, float y, int segments, ALLEGRO_COLOR color)
{
    int i;

    al_draw_bitmap(frame, x, y, 0);

    for (i = 0; i < segments && i < LIFE_SEGMENTS; i++)
    {
        al_draw_tinted_bitmap(bar_segment, color, x + 2.0f + 6.0f * (float)i, y + 2.0f, 0);
    }
}

void draw_hud()
{
    int x;
    ALLEGRO_COLOR vitals_color;
    float period;
    double beat;
    int heart_frame;
    int total_seconds;
    int x_weapon;
    int x_icon;
    int x_bar;
    int x_word;
    char line[96];
    char score_text[32];
    char timer_text[16];
    const char* state_text;
    const char* module_name;
    char reserve_text[40];
    ALLEGRO_BITMAP* laser_icon;

    al_draw_filled_rectangle(0.0f, (float)SCAN_PANEL_Y, (float)SCREEN_W, (float)SCREEN_H, UI_INK);
    al_draw_line(0.0f, (float)SCAN_PANEL_Y - 0.5f, (float)SCREEN_W,
        (float)SCAN_PANEL_Y - 0.5f, UI_LINE, 1.0f);

    // nada do HUD pode vazar para a faixa do mundo; o clip vale so no painel
    al_set_clipping_rectangle(0, SCAN_PANEL_Y, SCREEN_W, PANEL_H);

    // coluna de vida: icone 16x16 centrado na linha + barra 64x8 em x 28
    // linha 0: integridade do nanobo, bateria em ciano
    al_draw_bitmap(nanobo_icon, (float)HUD_MARGIN, (float)(hud_y(0) - 4), 0);
    draw_life_bar(battery_bar, 28.0f, (float)hud_y(0),
        life_segments(player.health, PLAYER_MAX_HEALTH), UI_CYAN);

    // linha 1: vitais do paciente no ECG, na cor da faixa
    if (patient_vitals >= VITALS_MIN_BETWEEN)
    {
        vitals_color = UI_GREEN;
    }
    else if (patient_vitals >= VITALS_LOW)
    {
        vitals_color = UI_AMBER;
    }
    else
    {
        vitals_color = UI_RED;
    }

    // coracao: batida (1), contracao (2) e repouso (0); bate mais rapido com vitais baixos
    period = heart_period(patient_vitals);
    beat = fmod(match_time, (double)period);
    heart_frame = 0;
    if (beat < 0.15 * period)
    {
        heart_frame = 1;
    }
    else if (beat < 0.30 * period)
    {
        heart_frame = 2;
    }
    al_draw_bitmap_region(heart_pulse, (float)(heart_frame * 16), 0.0f, 16.0f, 16.0f,
        (float)HUD_MARGIN, (float)(hud_y(1) - 4), 0);
    draw_life_bar(ecg_bar, 28.0f, (float)hud_y(1),
        life_segments(patient_vitals, PATIENT_VITALS_MAX), vitals_color);

    // linha 1, depois da coluna de vida: pontos e tempo a partir de x 100
    total_seconds = (int)match_time;
    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", total_seconds / 60, total_seconds % 60);
    snprintf(score_text, sizeof(score_text), "pontos %d", score);
    x = 100;
    al_draw_text(font, UI_TEXT, (float)x, (float)hud_y(1), 0, score_text);
    x += al_get_text_width(font, score_text) + 4;
    al_draw_text(font, UI_DIM, (float)x, (float)hud_y(1), 0, "·");
    x += al_get_text_width(font, "·") + 4;
    al_draw_text(font, UI_DIM, (float)x, (float)hud_y(1), 0, timer_text);

    // linha 0, a direita: a ferramenta com o alvo dela, o icone e a recarga,
    // tudo medido da borda
    if (defense_state() == 1)
    {
        state_text = "defesa";
    }
    else if (fire_locked())
    {
        state_text = "travado";
    }
    else if (attack_ready())
    {
        state_text = "pronto";
    }
    else
    {
        state_text = "recarga";
    }

    laser_icon = player.laser == LASER_GRAM_POSITIVE ? rupture_icon : dissolution_icon;

    // o HUD diz a ferramenta pelo nome e o que ela atinge (#199, #243)
    module_name = player.laser == LASER_GRAM_POSITIVE ? "<wR:Ruptura> · parede" : "<wD:Dissolução> · membrana";

    // Fase 2: anticorpos e Lise (#88)
    if (player.laser == LASER_NEUTRALIZE)
    {
        module_name = "<wN:Neutralização> · vírus livre";
    }
    else if (player.laser == LASER_LYSE)
    {
        module_name = "<wT:Ataque T> · célula infectada";
    }
    else if (player.laser == LASER_PERFORATE)
    {
        module_name = "<wF:Perfuração> · antifúngico";
    }

    // Reserva: nome com as cargas e a palavra de estado quando acabam (#216)
    if (player.laser == LASER_RESERVE)
    {
        snprintf(reserve_text, sizeof(reserve_text), "<wS:Reserva> %d/%d · resistente",
            reserve_charges_left(), RESERVE_CHARGES);
        module_name = reserve_text;

        if (reserve_charges_left() <= 0 && defense_state() != 1)
        {
            state_text = "esgotada";
        }
    }

    x_word = SCREEN_W - HUD_MARGIN - al_get_text_width(font, state_text);
    x_bar = x_word - 6 - UI_BAR_W;
    x_icon = x_bar - 4 - al_get_bitmap_width(laser_icon);
    x_weapon = x_icon - 4 - marked_text_width(module_name);

    // o nome da arma sai na cor dela (<wX:...>)
    draw_marked_text((float)x_weapon, (float)hud_y(0), UI_TEXT, module_name);

    // cada arma desenha o proprio icone ao lado do nome
    if (player.laser == LASER_RESERVE)
    {
        al_draw_bitmap(reserve_icon, (float)x_icon, (float)(hud_y(0) - 4), 0);
    }
    else if (player.laser == LASER_NEUTRALIZE)
    {
        al_draw_bitmap(antibody_icon, (float)x_icon, (float)(hud_y(0) - 4), 0);
    }
    else if (player.laser == LASER_LYSE)
    {
        al_draw_bitmap(lyse_icon, (float)x_icon, (float)(hud_y(0) - 4), 0);
    }
    else if (player.laser == LASER_PERFORATE)
    {
        al_draw_bitmap(perforate_icon, (float)x_icon, (float)(hud_y(0) - 4), 0);
    }
    else
    {
        al_draw_bitmap(laser_icon, (float)x_icon, (float)(hud_y(0) - 4), 0);
    }
    draw_cooldown_bar((float)x_bar, (float)(hud_y(0) + 1),
        fire_locked() ? fire_lock_left() : attack_cooldown_left());
    al_draw_text(font, UI_DIM, (float)x_word, (float)hud_y(0), 0, state_text);

    // linha 1, a direita: o objetivo saiu do HUD (vira o aviso de objetivo)
    // ameacas com contador e identificados, que e o progresso do bonus
    snprintf(line, sizeof(line), "ameaças %d/%d · identificados %d/%d",
        threats_total() - threats_left(), threats_total(),
        identified_count, identification_goal());
    al_draw_text(font, UI_DIM, (float)(SCREEN_W - HUD_MARGIN), (float)hud_y(1),
        ALLEGRO_ALIGN_RIGHT, line);

    // o quadro seguinte desenha o mundo no mesmo canvas: sem isso ele sai cortado
    al_reset_clipping_rectangle();
}

// quadro do spritesheet: 1-4 parado (220 ms), 5-8 andando (110 ms) (#30)
int player_frame()
{
    int base;
    double frame_time;
    bool moving = player.vx != 0.0f || player.vy != 0.0f;

    if (moving)
    {
        base = 4;
        frame_time = 0.110;
    }
    else
    {
        base = 0;
        frame_time = 0.220;
    }

    return base + (int)(match_time / frame_time) % 4;
}

// marcas do tratamento sobre a bacteria: o arco ambar esvazia ate a 2a dose
// e a resistente ganha anel vermelho e um R (#215)
void draw_treatment_marks(int index)
{
    float x = view_x(agents[index].x);
    float y = view_y(agents[index].y);

    if (agents[index].dose_timer > 0.0f)
    {
        float left = agents[index].dose_timer / (float)INCOMPLETE_TIME;

        al_draw_arc(x, y, agent_radius(index) + 7.0f, -ALLEGRO_PI / 2.0f,
            left * 2.0f * ALLEGRO_PI, UI_AMBER, 2.0f);
    }

    if (agents[index].resistant)
    {
        al_draw_circle(x, y, agent_radius(index) + 7.0f, UI_RED, 2.0f);
        al_draw_text(font, UI_RED, x, y - agent_radius(index) - 18.0f,
            ALLEGRO_ALIGN_CENTRE, "R");
    }

    // aviso do ataque: anel claro que pisca enquanto o invasor se prepara (#235)
    if (agents[index].windup > 0.0f && (int)(agents[index].windup * 20.0f) % 2 == 0)
    {
        al_draw_circle(x, y, agent_radius(index) + 4.0f, UI_TEXT, 2.0f);
    }
}

// fungo sem leitura: a mesma silhueta da bacteria, na escala do raio dele (#232, D7)
void draw_fungus_silhouette(int index)
{
    float size = 2.0f * agent_radius(index) + 2.0f;

    al_draw_scaled_bitmap(silhouette_sprite, 0.0f, 0.0f,
        (float)al_get_bitmap_width(silhouette_sprite), (float)al_get_bitmap_height(silhouette_sprite),
        view_x(agents[index].x) - size / 2.0f, view_y(agents[index].y) - size / 2.0f,
        size, size, 0);
}

// marcas do fungo: as do tratamento e o anel de identificado na 2a camada (#232)
void draw_fungus_marks(int index)
{
    if (agents[index].layers >= MAX_LAYERS)
    {
        al_draw_circle(view_x(agents[index].x), view_y(agents[index].y),
            agent_radius(index) + 2.0f, UI_GREEN, 1.0f);
    }

    draw_treatment_marks(index);
}

// cor de cada arma na mira, na ordem do Laser (#246)
ALLEGRO_COLOR laser_color(Laser laser)
{
    switch (laser)
    {
    case LASER_GRAM_POSITIVE:
        return UI_PURPLE;
    case LASER_GRAM_NEGATIVE:
        return UI_PINK;
    case LASER_RESERVE:
        return UI_AMBER;
    case LASER_NEUTRALIZE:
        return UI_CYAN;
    case LASER_LYSE:
        return UI_GREEN;
    case LASER_PERFORATE:
    default:
        return UI_WARM;
    }
}

// aviso de objetivo (OB-01..OB-04): placa no topo do mundo, esmaece no ultimo
// 0,5 s; com a ficha do scanner em cima, desce para logo abaixo dela
void draw_objective_notice(int card_target, float card_y)
{
    static const char* phase_goals[] = {
        "Objetivo: elimine as bactérias e chegue ao {destino}",
        "Objetivo: elimine os vírus e chegue ao {destino}",
        "Objetivo: elimine os fungos e chegue ao {destino}"
    };
    float left = objective_notice_left();
    float alpha = 1.0f;
    int phase = current_phase() - 1;
    const char* text;
    int text_w;
    float box_x;
    float box_y = 8.0f;

    // com a caixa do diario aberta o aviso espera ela fechar
    if (left <= 0.0f || pending_card() != CARD_NONE)
    {
        return;
    }

    if (objective_notice_goal() == GOAL_DESTINATION)
    {
        text = "Ameaças eliminadas! Agora vá ao {destino}";
    }
    else if (phase >= 0 && phase < (int)(sizeof(phase_goals) / sizeof(phase_goals[0])))
    {
        text = phase_goals[phase];
    }
    else
    {
        return;
    }

    if (left < NOTICE_FADE_TIME)
    {
        alpha = left / NOTICE_FADE_TIME;
    }

    if (card_target != -1 && card_y < (float)PLAY_H / 2.0f)
    {
        box_y = card_y + (float)CARD_H + 8.0f;
    }

    text_w = marked_text_width(text);
    box_x = floorf(((float)SCREEN_W - (float)text_w) / 2.0f);

    draw_panel(box_x - UI_PAD, box_y, (float)text_w + 2.0f * UI_PAD, (float)NOTICE_H,
        faded_color(UI_INK, alpha), faded_color(UI_AMBER, alpha));
    draw_marked_text(box_x, box_y + (float)(NOTICE_H - HUD_FONT_H) / 2.0f,
        faded_color(UI_WARM, alpha), text);
}

// mapa da fase atual com o tinte da fase e o veu azulado ate a altura h;
// o jogo e os resultados desenham o mesmo fundo
void draw_phase_map(float x, float y, float h)
{
    load_phase_map(current_phase());

    if (world_map)
    {
        al_draw_tinted_bitmap(world_map, phase_has_viruses() ? MAP_TINT_AIRWAY : MAP_TINT,
            x, y, 0);
    }
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, h, MAP_VEIL);
}

void draw_game()
{
    int i;
    bool is_bacteria;
    ALLEGRO_TRANSFORM shake;
    ALLEGRO_COLOR nanobo_tint = al_map_rgb_f(1.0f, 1.0f, 1.0f);
    const char* message;
    float aim_x;
    float aim_y;
    float px;
    float py;
    int card_target;
    bool card_from_cursor;
    float card_y;

    // o mundo para na faixa jogavel: sprite nenhum entra no painel
    al_set_clipping_rectangle(0, 0, SCREEN_W, PLAY_H);

    // tremor so no desenho do mundo: a camera e a simulacao nao mudam (#221)
    al_identity_transform(&shake);
    al_translate_transform(&shake, feel_offset_x(), feel_offset_y());
    al_use_transform(&shake);

    // mapa do mundo deslocado pela camera, atras de destino, agentes e nanobo
    // recuado: escurecido e com um veu azulado, sem mudar o PNG (#220); a fase
    // nova troca o mapa no primeiro quadro dela (#89)
    draw_phase_map(-floorf(camera.x), -floorf(camera.y), (float)PLAY_H);

    // destino da fase: anel e rotulo visiveis desde o inicio (#26)
    al_draw_filled_circle(view_x(DESTINATION_X), view_y(DESTINATION_Y), DESTINATION_RADIUS,
        al_map_rgb(40, 34, 18));
    al_draw_circle(view_x(DESTINATION_X), view_y(DESTINATION_Y), DESTINATION_RADIUS,
        UI_AMBER, 3);
    al_draw_text(font, UI_AMBER, view_x(DESTINATION_X),
        view_y(DESTINATION_Y) - 40, ALLEGRO_ALIGN_CENTRE, "destino");

    // correnteza: tracos claros que andam na direcao do fluxo (#237)
    {
        const Phase* ph = &phases[current_phase() - 1];
        int c;
        int k;

        for (c = 0; c < ph->current_count; c++)
        {
            const Current* cur = &ph->currents[c];
            float w = cur->x1 - cur->x0;
            float h = cur->y1 - cur->y0;

            for (k = 0; k < 40; k++)
            {
                float sx = cur->x0 + fmodf((float)(k * 97) + (float)match_time * cur->vx, w);
                float sy = cur->y0 + fmodf((float)(k * 37), h);

                al_draw_line(view_x(sx), view_y(sy), view_x(sx) + 6.0f, view_y(sy),
                    al_map_rgba_f(0.18f, 0.18f, 0.2f, 0.18f), 1.0f);
            }
        }
    }

    // perigos: biofilme amarelado e nuvem de endotoxina arroxeada, por baixo dos agentes (#237)
    for (i = 0; i < MAX_HAZARDS; i++)
    {
        float a;

        if (!hazards[i].active)
        {
            continue;
        }

        if (hazards[i].kind == HAZARD_BIOFILM)
        {
            a = 0.30f;
            al_draw_filled_circle(view_x(hazards[i].x), view_y(hazards[i].y), hazards[i].radius,
                al_map_rgba_f(0.62f * a, 0.66f * a, 0.30f * a, a));
            al_draw_circle(view_x(hazards[i].x), view_y(hazards[i].y), hazards[i].radius,
                al_map_rgba_f(0.8f * 0.6f, 0.85f * 0.6f, 0.45f * 0.6f, 0.6f), 1.0f);
        }
        else
        {
            a = 0.30f + 0.10f * sinf((float)match_time * 9.0f);
            al_draw_filled_circle(view_x(hazards[i].x), view_y(hazards[i].y),
                hazards[i].radius * (0.9f + 0.1f * (hazards[i].life / ENDOTOXIN_TIME)),
                al_map_rgba_f(0.70f * a, 0.22f * a, 0.70f * a, a));
        }
    }

    for (i = 0; i < agents_total(); i++)
    {
        // agente neutralizado sai do desenho; o virus de dentro da celula e
        // desenhado junto com ela (#85)
        if (!agents[i].active || virus_inside(i))
        {
            continue;
        }

        is_bacteria = species_is_bacterium(agents[i].species);

        // sombra de contato: o agente deixa de parecer colado no fundo (#220);
        // no tamanho de cada especie, do virus de 16 px a celula de 64 (#235)
        al_draw_filled_ellipse(view_x(agents[i].x), view_y(agents[i].y) + 0.85f * agent_radius(i),
            0.85f * agent_radius(i), 0.3f * agent_radius(i), CONTACT_SHADOW);

        // bacteria sem leitura: silhueta azulada, sem forma nem cor que entregue o grupo (#220)
        // o virus fica de fora: o estado dele e lido no mundo, sem scanner (#84)
        if (is_bacteria && agents[i].layers == 0)
        {
            // disco de holograma com "?": ainda nao se sabe o que e (#241)
            al_draw_bitmap(silhouette_sprite, view_x(agents[i].x) - (float)AGENT_RADIUS,
                view_y(agents[i].y) - (float)AGENT_RADIUS, 0);
            draw_treatment_marks(i);
            continue;
        }

        // bacteria lida: sprite cinza da forma; na camada 2, tingido pelo Gram e com anel (#214)
        if (is_bacteria)
        {
            ALLEGRO_COLOR tint = al_map_rgb_f(1.0f, 1.0f, 1.0f);
            bool stained = agents[i].layers >= MAX_LAYERS;

            if (stained)
            {
                tint = agents[i].species == SPECIES_GRAM_POSITIVE ? GRAM_POS_TINT : GRAM_NEG_TINT;
            }

            al_draw_tinted_bitmap(bacteria_sprite(agents[i].shape, agents[i].variant), tint,
                view_x(agents[i].x) - AGENT_RADIUS,
                view_y(agents[i].y) - AGENT_RADIUS, 0);

            if (stained)
            {
                al_draw_circle(view_x(agents[i].x), view_y(agents[i].y),
                    (float)AGENT_RADIUS + 2.0f,
                    agents[i].species == SPECIES_GRAM_POSITIVE ? UI_PURPLE : UI_PINK, 1.0f);
            }

            draw_treatment_marks(i);
            continue;
        }

        // hifa: um segmento septado de 16 px por vez, girado na direcao dela;
        // desenhada antes da colonia, que cobre a base (#229)
        if (agents[i].species == SPECIES_MOLD && agents[i].hypha_len > 0)
        {
            float tx;
            float ty;
            float angle = atan2f(agents[i].hypha_dy, agents[i].hypha_dx);
            // hifa para a esquerda: espelha na vertical para o brilho ficar em cima
            int flip = agents[i].hypha_dx < 0.0f ? ALLEGRO_FLIP_VERTICAL : 0;
            int k;

            for (k = 1; k <= agents[i].hypha_len; k++)
            {
                hypha_segment(i, k, &tx, &ty);
                al_draw_rotated_bitmap(hypha_sprite,
                    (float)al_get_bitmap_width(hypha_sprite) / 2.0f,
                    (float)al_get_bitmap_height(hypha_sprite) / 2.0f,
                    view_x(tx), view_y(ty), angle, flip);
            }
        }

        // fungos: silhueta ate a 1a leitura, depois o sprite no tamanho dele
        // (levedura 40, colonia 48, esporo 8); a hifa acima aparece desde o inicio (#232)
        if (species_is_fungus(agents[i].species))
        {
            if (agents[i].layers == 0)
            {
                draw_fungus_silhouette(i);
            }
            else
            {
                ALLEGRO_BITMAP* bmp = species_sprite(agents[i].species, agents[i].variant);

                al_draw_bitmap(bmp, view_x(agents[i].x) - (float)(al_get_bitmap_width(bmp) / 2),
                    view_y(agents[i].y) - (float)(al_get_bitmap_height(bmp) / 2), 0);
            }

            draw_fungus_marks(i);
            continue;
        }

        // virus e celula em escala biologica: 16 px o virus e 64 px a celula,
        // e o sprite ja tem esse tamanho (#235, #241)
        {
            ALLEGRO_BITMAP* bmp = species_sprite(agents[i].species, agents[i].variant);
            float size = 2.0f * agent_radius(i)
                + (agents[i].species == SPECIES_CELL ? 8.0f : 2.0f);
            ALLEGRO_COLOR tint = al_map_rgb_f(1.0f, 1.0f, 1.0f);

            al_draw_tinted_scaled_bitmap(bmp, tint, 0.0f, 0.0f,
                (float)al_get_bitmap_width(bmp), (float)al_get_bitmap_height(bmp),
                view_x(agents[i].x) - size / 2.0f, view_y(agents[i].y) - size / 2.0f,
                size, size, 0);
        }

        // celula infectada (#85, #87): o virus pisca la dentro, no ritmo de
        // LYSOGENIC_BLINK; no eclipse ele ja fez copias e a celula pulsa vermelha,
        // o aviso de que vai romper; depois da leitura, o arco ambar mostra o tempo
        if (agents[i].species == SPECIES_CELL && cell_virus(i) != -1)
        {
            int v = cell_virus(i);
            float cx = view_x(agents[i].x);
            float cy = view_y(agents[i].y);
            ALLEGRO_BITMAP* vb = species_sprite(SPECIES_VIRUS, agents[v].variant);
            float vw = (float)al_get_bitmap_width(vb);
            float vh = (float)al_get_bitmap_height(vb);
            bool eclipse = agents[v].virus_state == VIRUS_ECLIPSE;

            if (eclipse)
            {
                float a = 0.22f + 0.18f * sinf((float)match_time * 18.0f);

                al_draw_filled_circle(cx, cy, agent_radius(i) + 2.0f,
                    al_map_rgba_f(0.9f * a, 0.2f * a, 0.25f * a, a));
                al_draw_circle(cx, cy, agent_radius(i) + 4.0f, UI_RED, 2.0f);
                al_draw_scaled_bitmap(vb, 0.0f, 0.0f, vw, vh, cx + 2.0f, cy - 16.0f, 12.0f, 12.0f, 0);
                al_draw_scaled_bitmap(vb, 0.0f, 0.0f, vw, vh, cx - 14.0f, cy - 4.0f, 12.0f, 12.0f, 0);
                al_draw_scaled_bitmap(vb, 0.0f, 0.0f, vw, vh, cx + 4.0f, cy + 4.0f, 12.0f, 12.0f, 0);
            }
            else if ((int)(match_time / LYSOGENIC_BLINK) % 2 == 0)
            {
                // fundo escuro: o virus nao se perde entre as organelas
                al_draw_filled_circle(cx + 11.0f, cy - 7.0f, 9.0f, al_map_rgba_f(0.04f, 0.06f, 0.1f, 0.75f));
                al_draw_scaled_bitmap(vb, 0.0f, 0.0f, vw, vh, cx + 4.0f, cy - 14.0f, 14.0f, 14.0f, 0);
            }

            if (agents[i].layers > 0 && !eclipse)
            {
                float left = fminf(agents[v].lysogenic_timer / LYSOGENIC_TIME, 1.0f);

                al_draw_arc(cx, cy, agent_radius(i) + 6.0f, -ALLEGRO_PI / 2.0f,
                    left * 2.0f * ALLEGRO_PI, UI_AMBER, 2.0f);
            }
        }

        // celula colonizada: barra curta da vida dela, acima do sprite (#235)
        if (agents[i].species == SPECIES_CELL && agents[i].cell_health < CELL_HEALTH)
        {
            float bx = view_x(agents[i].x) - 16.0f;
            float by = view_y(agents[i].y) - agent_radius(i) - 8.0f;

            al_draw_filled_rectangle(bx, by, bx + 32.0f, by + 3.0f, UI_SLOT);
            al_draw_filled_rectangle(bx, by, bx + 32.0f * agents[i].cell_health / CELL_HEALTH,
                by + 3.0f, UI_RED);
        }
    }

    // dica do tutorial presa ao agente: some no primeiro scan, volta no tiro sem leitura (#219)
    if (tutorial_hint_target() != -1)
    {
        const char* hint = "[botão direito]: escanear";
        int hint_w = marked_text_width(hint);
        float hx = floorf(view_x(agents[tutorial_hint_target()].x) - (float)hint_w / 2.0f);
        float hy = view_y(agents[tutorial_hint_target()].y) - agent_radius(tutorial_hint_target()) - 20.0f;

        // a tampa de tecla de 14 px cabe com folga; a moldura neutra nao briga
        // com a borda ciano da tecla
        al_draw_filled_rectangle(hx - 4.0f, hy - 6.0f, hx + (float)hint_w + 4.0f, hy + 14.0f, UI_INK);
        al_draw_rectangle(hx - 3.5f, hy - 5.5f, hx + (float)hint_w + 3.5f, hy + 13.5f, UI_FRAME, 1.0f);
        draw_marked_text(hx, hy, UI_CYAN, hint);
    }

    // a sombra do nanobo fica sob o anel magnetico, que flutua embaixo (#220, #240)
    al_draw_filled_ellipse(view_x(player.x), view_y(player.y) + 21.0f,
        13.0f, 3.0f, CONTACT_SHADOW);

    // o nanobo e desenhado onde o mundo deixou ele, ja descontada a camera
    // a linha do spritesheet e a direcao quando a arte tem as quatro (#30)
    int nanobo_row = (al_get_bitmap_height(nanobo) >= 4 * PLAYER_FRAME) ? (int)player.facing : 0;

    // o nanobo pisca enquanto a invulnerabilidade dura (#221)
    if (invulnerability_left() > 0.0f && (int)(invulnerability_left() * 12.0f) % 2 == 1)
    {
        nanobo_tint = al_map_rgba_f(0.3f, 0.3f, 0.3f, 0.3f);
    }

    // quadro de 48 px centrado no nanobo: a cabeca e o corpo ficam no circulo
    // de colisao e o anel magnetico flutua logo abaixo (#240)
    al_draw_tinted_bitmap_region(nanobo, nanobo_tint, (float)(player_frame() * PLAYER_FRAME),
        (float)(nanobo_row * PLAYER_FRAME), (float)PLAYER_FRAME, (float)PLAYER_FRAME,
        view_x(player.x) - (float)PLAYER_FRAME / 2.0f, view_y(player.y) - (float)PLAYER_FRAME / 2.0f, 0);

    // anel da defesa: ciano enquanto protege, apagado durante a recarga (#17);
    // no raio do escudo que destroi os projeteis (#236)
    if (defense_state() == 1)
    {
        al_draw_circle(view_x(player.x), view_y(player.y), SHIELD_RADIUS, UI_CYAN, 2);
    }
    else if (defense_state() == 2)
    {
        al_draw_circle(view_x(player.x), view_y(player.y), SHIELD_RADIUS,
            UI_FRAME, 2);
    }

    // projetil de cor neutra, a forma mostra o mecanismo (RF-04, #242): cunha
    // da Ruptura, gota da Dissolucao, losango da Reserva, Y da Neutralizacao e
    // anel da Lise; o anel liso da Perfuracao e a 6a linha da folha; o desenho
    // aponta para a direita e gira na direcao do voo
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        int frame = (int)(match_time / 0.08) % 2;

        if (!projectiles[i].active)
        {
            continue;
        }

        px = view_x(projectiles[i].x);
        py = view_y(projectiles[i].y);

        al_draw_tinted_scaled_rotated_bitmap_region(shots_sheet,
            (float)(frame * SHOT_FRAME), (float)((int)projectiles[i].laser * SHOT_FRAME),
            (float)SHOT_FRAME, (float)SHOT_FRAME, al_map_rgb_f(1.0f, 1.0f, 1.0f),
            (float)SHOT_FRAME / 2.0f, (float)SHOT_FRAME / 2.0f, px, py, 1.0f, 1.0f,
            atan2f(projectiles[i].dy, projectiles[i].dx), 0);
    }

    // projeteis inimigos: a toxina e uma gota verde-amarela com contorno (#236, #241)
    for (i = 0; i < MAX_ENEMY_SHOTS; i++)
    {
        if (!enemy_shots[i].active)
        {
            continue;
        }

        px = view_x(enemy_shots[i].x);
        py = view_y(enemy_shots[i].y);

        // virus da rajada: o proprio sprite do virus, pequeno, em voo (#86)
        if (enemy_shots[i].kind == SHOT_VIRION)
        {
            al_draw_scaled_bitmap(virus_sprite, 0.0f, 0.0f,
                (float)al_get_bitmap_width(virus_sprite), (float)al_get_bitmap_height(virus_sprite),
                px - 6.0f, py - 6.0f, 12.0f, 12.0f, 0);
            continue;
        }

        // esporo do bolor em voo: o conidio de 8x8, o mesmo do pousado (#229)
        if (enemy_shots[i].kind == SHOT_SPORE)
        {
            al_draw_bitmap(spore_sprite, px - 4.0f, py - 4.0f, 0);
            continue;
        }

        // toxina: a gota de 8x8 centrada no projetil (#241)
        al_draw_bitmap(toxin_sprite, px - 4.0f, py - 4.0f, 0);
    }

    draw_feel_world();

    // o resto da tela nao treme
    al_identity_transform(&shake);
    al_use_transform(&shake);

    // flash vermelho curto na faixa do mundo ao levar dano (#221)
    if (feel_flash_time > 0.0f)
    {
        float a = 0.35f * feel_flash_time / FEEL_FLASH_TIME;

        al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)PLAY_H,
            al_map_rgba_f(0.9f * a, 0.1f * a, 0.1f * a, a));
    }

    // ficha do scanner so com alvo (#243): calculada aqui para o aviso desviar dela
    card_target = scan_card_target(&card_from_cursor);
    card_y = card_target != -1 ? scan_card_y(card_target) : -1.0f;

    // feedback educacional curto da ultima decisao (#178), com placa de fundo
    message = feedback_text();

    if (message != NULL)
    {
        int text_w = marked_text_width(message);
        float box_x = floorf(((float)SCREEN_W - (float)text_w) / 2.0f);
        float box_y = (float)(PLAY_H - 30);

        // com a ficha embaixo, o aviso sobe para cima dela
        if (card_target != -1 && card_y > (float)PLAY_H / 2.0f)
        {
            box_y = card_y - 22.0f;
        }

        al_draw_filled_rectangle(box_x - UI_PAD, box_y - 4.0f, box_x + (float)text_w + UI_PAD,
            box_y + 12.0f, UI_INK);
        al_draw_rectangle(box_x - UI_PAD + 0.5f, box_y - 4.0f + 0.5f,
            box_x + (float)text_w + UI_PAD - 0.5f, box_y + 12.0f - 0.5f,
            UI_FRAME, 1.0f);
        draw_marked_text(box_x, box_y, UI_WARM, message);
    }

    // o aviso de objetivo fica no topo; o feedback continua embaixo (#246)
    draw_objective_notice(card_target, card_y);

    // marca o destino quando ele esta fora da faixa visivel (#19), sem passar
    // por baixo da ficha do scanner
    if (card_target == -1)
    {
        draw_destination_hint(0.0f, (float)PLAY_H);
    }
    else if (card_y > (float)PLAY_H / 2.0f)
    {
        draw_destination_hint(0.0f, card_y);
    }
    else
    {
        draw_destination_hint(card_y + (float)CARD_H, (float)PLAY_H);
    }

    // a ficha fica dentro do recorte do mundo: nada dela vaza para o HUD
    if (card_target != -1)
    {
        draw_scan_card(card_target, card_from_cursor, card_y);
    }

    // mira livre: o cursor do sistema some na fase e vira esta marca (#198);
    // o quadro diz se o tiro sai agora e a cor e a da arma (#246); com a caixa
    // do diario aberta ela some
    aim_x = to_logical_x(mouse_x);
    aim_y = to_logical_y(mouse_y);

    if (game_get_scene() == GAME_SCENE_GAME && pending_card() == CARD_NONE
        && aim_x >= 0.0f && aim_x < (float)SCREEN_W
        && aim_y >= 0.0f && aim_y < (float)PLAY_H)
    {
        al_draw_tinted_bitmap_region(aim_sprite, laser_color(player.laser),
            (float)((int)aim_frame() * AIM_FRAME_SIZE), 0.0f,
            (float)AIM_FRAME_SIZE, (float)AIM_FRAME_SIZE,
            floorf(aim_x) - (float)AIM_CENTER,
            floorf(aim_y) - (float)AIM_CENTER, 0);
    }

#ifdef _DEBUG
    // F1 mostra o estado e o tempo de cada virus da fase (#84)
    if (show_debug)
    {
        char line[96];
        int used = snprintf(line, sizeof(line), "QA virus");

        for (i = 0; i < agents_total() && used > 0 && used < (int)sizeof(line); i++)
        {
            if (agents[i].species != SPECIES_VIRUS || !agents[i].active)
            {
                continue;
            }

            used += snprintf(line + used, sizeof(line) - (size_t)used, " %d:%s%.1f",
                agents[i].id, agents[i].virus_state == VIRUS_LYSOGENIC ? "L"
                    : agents[i].virus_state == VIRUS_ECLIPSE ? "E" : "F",
                agents[i].lysogenic_timer);
        }

        al_draw_text(font, UI_DIM, 8, 8, 0, line);
    }
#endif

    // o clip do painel nao pode intersectar o clip do mundo, senao o HUD some
    al_reset_clipping_rectangle();

    draw_hud();
}

// pausa: o jogo fica visivel e congelado atras do painel
void draw_pause()
{
    static const char* phase_names[] = {
        "Fase 1 · bactérias no sangue",
        "Fase 2 · vírus nas vias aéreas",
        "Fase 3 · fungos no pulmão"
    };
    // objetivo completo da fase atual: tres linhas por fase (PA-03)
    static const char* goals[][3] = {
        {
            "Elimine as bactérias do sangue do Léo e vá ao destino.",
            "<p:Gram+>: <wR:Ruptura>. <r:Gram->: <wD:Dissolução>. Resistente: <wS:Reserva>.",
            "Leia antes de disparar. Ler todos vale <g:+15 pontos>."
        },
        {
            "Elimine os vírus nas vias aéreas do Léo e vá ao destino.",
            "Vírus livre: <wN:Neutralização>. Célula infectada: <wT:Ataque T>.",
            "Leia a célula antes: o <wT:Ataque T> custa vitais do Léo."
        },
        {
            "Elimine os fungos no pulmão do Léo e vá ao destino.",
            "Antibiótico não tem alvo no fungo: use a <wF:Perfuração>.",
            "No bolor, mire na base da colônia, não na hifa."
        }
    };
    int count;
    const UiButton* buttons = screen_buttons(&count);
    int phase = current_phase() - 1;
    int i;

    if (phase < 0 || phase > 2)
    {
        phase = 0;
    }

    // veu mais escuro que o antigo: o painel maior precisa de contraste
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H,
        UI_VEIL(210.0f / 255.0f));
    draw_panel(80.0f, 70.0f, 480.0f, 216.0f, UI_INK, UI_FRAME);
    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 82, ALLEGRO_ALIGN_CENTRE, "PAUSA");
    draw_rule((float)SCREEN_W / 2, 96.0f, 200.0f, UI_FRAME);
    al_draw_text(font, UI_AMBER, SCREEN_W / 2, 104, ALLEGRO_ALIGN_CENTRE, phase_names[phase]);

    // texto de x 96 a 544: a maior linha tem 56 caracteres (448 px)
    for (i = 0; i < 3; i++)
    {
        draw_marked_text(96.0f, 124.0f + 16.0f * (float)i, UI_TEXT, goals[phase][i]);
    }

    draw_marked_text(96.0f, 180.0f, UI_DIM, "[WASD] mover · [clique] dispara · [botão direito] lê");
    draw_marked_text(96.0f, 196.0f, UI_DIM, "[X] defesa · [1] [2] [3] ou [roda]: arma");

    draw_buttons(buttons, count);
    draw_marked_text_aligned(SCREEN_W / 2, 264, UI_DIM, ALLEGRO_ALIGN_CENTRE, "[esc] continua");
}

// ---------------------------------------------------------------
// como jogar: 4 paginas, abre do menu e da pausa
// ---------------------------------------------------------------

#define HOWTO_PANEL_Y 104.0f
#define HOWTO_ICON_X  40.0f   // coluna de icone 48x48 entre x 24 e 104
#define HOWTO_TEXT_X  120.0f
#define HOWTO_LINES   6

// volta para a cena de onde o como jogar abriu (menu ou pausa)
void howto_return()
{
    game_set_scene(game_howto_origin());
}

// enter, espaco ou clique: avanca; na ultima pagina, volta
void howto_advance()
{
    if (howto_page >= HOWTO_PAGES - 1)
    {
        howto_return();
        return;
    }

    howto_page++;
}

// coluna de icone da pagina, em 48x48
void draw_howto_icon(float x, float y)
{
    float row_y;
    int frame;

    if (howto_page == 0)
    {
        // o Nanobo de frente, quadro parado 0
        al_draw_bitmap_region(nanobo, 0.0f, (float)(PLAYER_FACING_DOWN * PLAYER_FRAME),
            (float)PLAYER_FRAME, (float)PLAYER_FRAME, x, y, 0);
    }
    else if (howto_page == 1)
    {
        // scanner x2 e os dois marcadores da ficha: camada 1 lida, camada 2 vazia
        al_draw_scaled_bitmap(scanner_icon, 0.0f, 0.0f, 16.0f, 16.0f, x + 8.0f, y, 32.0f, 32.0f, 0);
        al_draw_filled_rectangle(x + 15.0f, y + 38.0f, x + 15.0f + (float)CARD_MARK, y + 38.0f + (float)CARD_MARK, UI_AMBER);
        al_draw_filled_rectangle(x + 27.0f, y + 38.0f, x + 27.0f + (float)CARD_MARK, y + 38.0f + (float)CARD_MARK, UI_SLOT);
    }
    else if (howto_page == 2)
    {
        // as 6 armas: fase 1 em cima, fases 2 e 3 embaixo
        row_y = y + 4.0f;
        al_draw_bitmap(rupture_icon, x, row_y, 0);
        al_draw_bitmap(dissolution_icon, x + 16.0f, row_y, 0);
        al_draw_bitmap(reserve_icon, x + 32.0f, row_y, 0);
        row_y = y + 28.0f;
        al_draw_bitmap(antibody_icon, x, row_y, 0);
        al_draw_bitmap(lyse_icon, x + 16.0f, row_y, 0);
        al_draw_bitmap(perforate_icon, x + 32.0f, row_y, 0);
    }
    else
    {
        // cabeca do robo ao lado das linhas da integridade, coracao ao lado dos vitais
        frame = (int)fmod(al_get_time() / 0.25, 3.0);
        al_draw_scaled_bitmap(nanobo_icon, 0.0f, 0.0f, 16.0f, 16.0f, x + 8.0f, y - 24.0f, 32.0f, 32.0f, 0);
        al_draw_scaled_bitmap(heart_pulse, (float)(frame * 16), 0.0f, 16.0f, 16.0f,
            x + 8.0f, y + 8.0f, 32.0f, 32.0f, 0);
    }
}

void draw_howto()
{
    static const char* titles[HOWTO_PAGES] = {
        "Mover e mirar",
        "Ler com o scanner",
        "Armas",
        "As duas barras"
    };
    static const char* lines[HOWTO_PAGES][HOWTO_LINES] = {
        {
            "[Setas] ou [WASD]: mover o Nanobô.",
            "[Mouse]: mirar. [Botão esquerdo]: disparar a arma.",
            "[Botão direito]: escanear o alvo sob o cursor.",
            "[X]: defesa por 2 s. Contato não fere; toxinas, vírus e",
            "esporos em voo se desfazem. A hifa passa. Recarga: 3 s.",
            "[Esc]: pausa. [F11]: tela cheia."
        },
        {
            "[Botão direito] num agente: eu leio uma camada por vez.",
            "Camada 1: a {forma}. Coco, bacilo, levedura, bolor...",
            "Camada 2: a {coloração de Gram} e o nome da espécie.",
            "A forma não basta: existe coco <p:Gram+> e coco <r:Gram->.",
            "Com as duas camadas lidas, o agente está identificado.",
            "Identificar todos os agentes da fase vale <g:+15 pontos>."
        },
        {
            "[1], [2], [3] ou [roda do mouse]: trocar de arma.",
            "Fase 1: [1] <wR:Ruptura> · [2] <wD:Dissolução> · [3] <wS:Reserva> (2 cargas)",
            "Cada bactéria pede 2 doses, a segunda em até 6 segundos.",
            "Fase 2: [1] <wN:Neutralização> (vírus livre) · [2] <wT:Ataque T>",
            "Fase 3: [1] <wF:Perfuração> · [2] <wR:Ruptura> (não age em fungo)",
            "Regra do jogo: as armas se inspiram em remédios reais."
        },
        {
            "Cabeça de robô: a minha {Integridade}. Ela cai quando",
            "um invasor, uma toxina, a hifa ou a nuvem me acertam.",
            "Coração: os {Vitais} do Léo. Caem quando uma célula dele",
            "é destruída ou atingida por engano, e no <wT:Ataque T>.",
            "Se uma das barras zera, a equipe me retira do corpo.",
            "Nova fase: Integridade cheia e Vitais de no mínimo 60."
        }
    };
    char counter[16];
    float line_y = HOWTO_PANEL_Y + 36.0f;
    int page = howto_page;
    int i;

    if (page < 0 || page >= HOWTO_PAGES)
    {
        page = 0;
        howto_page = 0;
    }

    if (howto_background)
    {
        al_draw_bitmap(howto_background, 0.0f, 0.0f, 0);
    }
    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 16, ALLEGRO_ALIGN_CENTRE, "Como jogar");
    draw_rule((float)SCREEN_W / 2, 30.0f, 200.0f, UI_FRAME);

    // painel central quase opaco (cor pre-multiplicada): o radar do fundo aparece de leve
    draw_panel(16.0f, HOWTO_PANEL_Y, (float)SCREEN_W - 32.0f, 144.0f,
        UI_VEIL(230.0f / 255.0f), UI_FRAME);
    al_draw_text(font, UI_AMBER, HOWTO_TEXT_X, HOWTO_PANEL_Y + 12.0f, 0, titles[page]);
    snprintf(counter, sizeof(counter), "%d/%d", page + 1, HOWTO_PAGES);
    al_draw_text(font, UI_DIM, (float)SCREEN_W - 24.0f, HOWTO_PANEL_Y + 12.0f, ALLEGRO_ALIGN_RIGHT, counter);

    draw_howto_icon(HOWTO_ICON_X, line_y + 20.0f);

    for (i = 0; i < HOWTO_LINES; i++)
    {
        draw_marked_text(HOWTO_TEXT_X, line_y + 16.0f * (float)i, UI_TEXT, lines[page][i]);
    }

    draw_marked_text_aligned(SCREEN_W / 2, 336, UI_DIM, ALLEGRO_ALIGN_CENTRE,
        page == HOWTO_PAGES - 1
            ? "[enter], [espaço] ou [clique] volta · [setas] trocam · [esc] volta"
            : "[enter], [espaço] ou [clique] avança · [setas] trocam · [esc] volta");
}

// ---------------------------------------------------------------
// fim de fase: resultados, vitoria e game over
// ---------------------------------------------------------------


// relatorio da fase em linguagem de jogador: cada linha retoma um conteudo
// com os numeros da partida, sem os codigos internos dos objetivos (#223)
void results_report_line(int row, char* line, size_t size)
{
    int neutralized = threats_total() - threats_left();
    int reserve_spent = progress.reserve_on_resistant + progress.reserve_on_common;

    line[0] = '\0';

    // fase de fungos: o que foi eliminado, o antibiotico que nao age e a carga de esporos (#232)
    if (phase_has_fungi())
    {
        if (row == 0)
        {
            snprintf(line, size, "eliminei %d leveduras", progress.yeasts_eliminated);
        }
        else if (row == 1)
        {
            snprintf(line, size, "eliminei %d colônias (com as germinadas)", progress.molds_eliminated);
        }
        else if (row == 2)
        {
            snprintf(line, size, "gastei %d tiros de antibiótico em fungo", progress.antibiotic_on_fungus);
        }
        else if (row == 3)
        {
            snprintf(line, size, "esporos pela defesa fraca do Léo: %d", spore_load_current());
        }
        else
        {
            snprintf(line, size, "anotei: fungo tem quitina e ergosterol");
        }

        return;
    }

    // fase de virus: anticorpos, Lise e ciclo litico (#88)
    if (phase_has_viruses())
    {
        if (row == 0)
        {
            snprintf(line, size, "prendi %d vírus livres com anticorpos", progress.viruses_neutralized);
        }
        else if (row == 1)
        {
            snprintf(line, size, "<wT:Ataque T>: contive %d células a tempo", progress.cells_lysed);
        }
        else if (row == 2)
        {
            snprintf(line, size, "%d células romperam no ciclo lítico", progress.cells_burst);
        }
        else if (row == 3)
        {
            snprintf(line, size, "eliminei %d de %d ameaças", neutralized, threats_total());
        }
        else
        {
            snprintf(line, size, "anotei: vírus sem parede nem ribossomos");
        }

        return;
    }

    if (row == 0)
    {
        snprintf(line, size, "li %d <p:Gram+> e %d <r:Gram-> até a coloração",
            progress.scans_gram_positive, progress.scans_gram_negative);
    }
    else if (row == 1)
    {
        // nenhuma resistencia nao e perda: o zero fica sem o coral
        snprintf(line, size, progress.resistances > 0
                ? "tratei %d até o fim; <x:%d resistiram>"
                : "tratei %d até o fim; %d resistiram",
            progress.treatments_completed, progress.resistances);
    }
    else if (row == 2)
    {
        if (progress.wrong_decisions == 0)
        {
            snprintf(line, size, "não errei nenhum antibiótico");
        }
        else
        {
            snprintf(line, size, "<x:errei %d antibióticos> (%d pela forma)",
                progress.wrong_decisions, progress.shape_guesses);
        }
    }
    else if (row == 3)
    {
        if (reserve_spent == 0)
        {
            snprintf(line, size, "guardei a <wS:Reserva>: nenhuma carga gasta");
        }
        else
        {
            snprintf(line, size, "<wS:Reserva>: %d na resistente, %d na comum",
                progress.reserve_on_resistant, progress.reserve_on_common);
        }
    }
    else
    {
        snprintf(line, size, "acertei %d células do Léo por engano", progress.cell_attacks);
    }
}


void draw_results()
{
    char line[128];
    char value[32];
    int count;
    const UiButton* buttons = screen_buttons(&count);
    int neutralized = threats_total() - threats_left();
    float left = 152.0f;
    float right = (float)SCREEN_W - 152.0f;
    float row;
    int report_row;
    ALLEGRO_COLOR bonus_color;
    float map_y = camera.y;

    // fundo: o mapa da fase concluida onde a camera parou, na tela inteira
    // (sem passar da borda de baixo do PNG) e com um veu escuro por cima
    if (world_map && map_y > (float)(al_get_bitmap_height(world_map) - SCREEN_H))
    {
        map_y = fmaxf(0.0f, (float)(al_get_bitmap_height(world_map) - SCREEN_H));
    }

    draw_phase_map(-floorf(camera.x), -floorf(map_y), (float)SCREEN_H);
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H,
        UI_VEIL(160.0f / 255.0f));

    snprintf(line, sizeof(line), "Fase %d concluída", current_phase());
    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 24, ALLEGRO_ALIGN_CENTRE, line);
    draw_rule((float)SCREEN_W / 2, 38.0f, 200.0f, UI_FRAME);

    // interludio (#244): entre as fases, o estado do paciente e o proximo
    // invasor; os vitais ja com o piso de 60 que a passagem aplica (#218)
    if (has_next_phase())
    {
        int vitals = (int)fmaxf(patient_vitals, VITALS_MIN_BETWEEN);
        const char* state = vitals >= 70 ? "estável" : (vitals >= 40 ? "em observação" : "grave");

        snprintf(line, sizeof(line), "vitais do Léo: %s (%d) · a seguir: %s",
            state, vitals, current_phase() == 1 ? "vírus nas vias aéreas" : "fungos no pulmão");
        al_draw_text(font, UI_DIM, SCREEN_W / 2, 46, ALLEGRO_ALIGN_CENTRE, line);
    }
    else
    {
        // ultima fase: so o estado e os vitais reais, sem o piso; a alta vem na vitoria
        int vitals = (int)fmaxf(patient_vitals, 0.0f);
        const char* state = vitals >= 70 ? "estável" : (vitals >= 40 ? "em observação" : "grave");

        snprintf(line, sizeof(line), "vitais do Léo: %s (%d)", state, vitals);
        al_draw_text(font, UI_DIM, SCREEN_W / 2, 46, ALLEGRO_ALIGN_CENTRE, line);
    }

    // resultado principal: pontos e ameacas, o que fecha a fase
    draw_panel(140.0f, 58.0f, 360.0f, 44.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d", score);
    draw_stat(left, 66.0f, right, "pontos", value, UI_AMBER);

    snprintf(value, sizeof(value), "%d/%d", neutralized, threats_total());
    draw_stat(left, 82.0f, right, "ameaças eliminadas", value, UI_TEXT);

    // objetivos secundarios: identificacao e bonus
    draw_panel(140.0f, 106.0f, 360.0f, 52.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d/%d", identified_count, identification_goal());
    draw_stat(left, 114.0f, right, "agentes identificados", value, UI_TEXT);

    if (identification_bonus_awarded())
    {
        bonus_color = UI_GREEN;
        snprintf(value, sizeof(value), "+%d", IDENTIFICATION_BONUS);
    }
    else
    {
        bonus_color = UI_DIM;
        snprintf(value, sizeof(value), "não obtido");
    }

    draw_stat(left, 130.0f, right, "bônus de identificação", value, bonus_color);

    // o bonus dos vitais fecha o painel dos objetivos secundarios (#218)
    snprintf(value, sizeof(value), "+%d", vitals_bonus_awarded());
    draw_stat(left, 146.0f, right, "bônus pelos vitais do Léo", value,
        vitals_bonus_awarded() > 0 ? UI_GREEN : UI_DIM);

    // resumo educativo: so contadores da partida atual (#174, #202)
    draw_panel(140.0f, 162.0f, 360.0f, 92.0f, UI_INK, UI_FRAME);

    al_draw_text(font, UI_AMBER, left, 170.0f, 0, "O que anotei nesta fase");


    // relatorio da fase: cinco linhas com os numeros da partida (#223)
    row = 184.0f;

    for (report_row = 0; report_row < 5; report_row++)
    {
        results_report_line(report_row, line, sizeof(line));
        draw_marked_text(left, row, UI_TEXT, line);
        row = row + 14.0f;
    }

    draw_buttons(buttons, count);

    draw_marked_text_aligned(SCREEN_W / 2, 296, UI_DIM, ALLEGRO_ALIGN_CENTRE,
        "[enter] também continua");

#ifdef _DEBUG
    // contadores da #200 fora da interface normal: F1 liga e desliga (#202)
    if (show_debug)
    {
        snprintf(line, sizeof(line), "QA scans %d/%d decisoes %d/%d celulas %d",
            progress.scans_gram_positive, progress.scans_gram_negative,
            progress.correct_decisions, progress.wrong_decisions, progress.cell_attacks);
        al_draw_text(font, UI_DIM, SCREEN_W / 2, 340, ALLEGRO_ALIGN_CENTRE, line);
    }
#endif
}

// painel do Top 10 (#197): posicao, nome e pontos; o registro novo em ambar
void draw_ranking_panel(float x, float y, float w, int highlight, bool in_menu)
{
    char line[32];
    int i;

    draw_panel(x, y, w, 184.0f, UI_INK, UI_FRAME);
    al_draw_text(font, UI_AMBER, x + w / 2.0f, y + 8.0f, ALLEGRO_ALIGN_CENTRE, "Top 10");

    if (ranking_count() == 0)
    {
        al_draw_text(font, UI_DIM, x + w / 2.0f, y + 86.0f, ALLEGRO_ALIGN_CENTRE,
            "nenhum recorde ainda");
        // a dica so no menu: na vitoria o Leo ja deu alta
        if (in_menu)
        {
            al_draw_text(font, UI_DIM, x + w / 2.0f, y + 102.0f, ALLEGRO_ALIGN_CENTRE,
                "dê alta ao Léo para entrar aqui");
        }
        return;
    }

    for (i = 0; i < ranking_count(); i++)
    {
        const RankingEntry* e = ranking_entry(i);
        float row = y + 28.0f + (float)i * 15.0f;
        ALLEGRO_COLOR color = i == highlight ? UI_AMBER : UI_TEXT;

        if (i == highlight)
        {
            al_draw_filled_rectangle(x + 4.0f, row - 3.0f, x + w - 4.0f, row + 11.0f, UI_HOVER);
        }

        snprintf(line, sizeof(line), "%2d", i + 1);
        al_draw_text(font, UI_DIM, x + 12.0f, row, 0, line);
        al_draw_text(font, color, x + 40.0f, row, 0, e->name);
        snprintf(line, sizeof(line), "%d", e->score);
        al_draw_text(font, color, x + w - 12.0f, row, ALLEGRO_ALIGN_RIGHT, line);
    }
}

// confirma o nome digitado: insere no Top 10 e grava (#79, #80, #81)
void confirm_name_entry()
{
    if (!name_entry_active)
    {
        return;
    }

    name_entry_active = false;
    ranking_highlight = ranking_insert(score, name_entry);
    ranking_save_failed = !ranking_save();
}

// caractere digitado na entrada de nome: ate 10 caracteres do Latin-1, que a
// fonte desenha; maiuscula acentuada vira minuscula, porque a fonte erra o
// glifo dela (#81)
void handle_key_char(int keycode, int unichar)
{
    char bytes[8];
    int len;

    if (game_get_scene() != GAME_SCENE_VICTORY || !name_entry_active)
    {
        return;
    }

    if (keycode == ALLEGRO_KEY_BACKSPACE)
    {
        size_t n = strlen(name_entry);

        if (n == 0)
        {
            return;
        }

        // volta ate o inicio do ultimo caractere UTF-8
        n--;
        while (n > 0 && ((unsigned char)name_entry[n] & 0xC0) == 0x80)
        {
            n--;
        }

        name_entry[n] = '\0';
        name_entry_chars = name_entry_chars - 1;
        return;
    }

    if (unichar < 32 || unichar == 127 || unichar > 0xFF || (unichar >= 0x80 && unichar < 0xA0))
    {
        return;
    }

    if (unichar >= 0xC0 && unichar <= 0xDE && unichar != 0xD7)
    {
        unichar = unichar + 0x20;
    }

    if (name_entry_chars >= RANKING_NAME_CHARS)
    {
        return;
    }

    len = (int)al_utf8_encode(bytes, unichar);

    if (strlen(name_entry) + (size_t)len >= sizeof(name_entry))
    {
        return;
    }

    bytes[len] = '\0';
    strcat_s(name_entry, sizeof(name_entry), bytes);
    name_entry_chars = name_entry_chars + 1;
}

void draw_victory()
{
    char value[48];
    char timer_text[16];
    int total_seconds = (int)run_time;
    int count;
    const UiButton* buttons = screen_buttons(&count);
    float left = 36.0f;
    float right = 302.0f;

    // o quarto de dia, leito arrumado: o Leo foi para casa; o veu segura o texto
    if (hospital_victory)
    {
        al_draw_bitmap(hospital_victory, 0.0f, 0.0f, 0);
    }
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H, UI_VEIL(150.0f / 255.0f));

    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 18, ALLEGRO_ALIGN_CENTRE, "Vitória");
    draw_rule((float)SCREEN_W / 2, 32.0f, 200.0f, UI_FRAME);
    // final da narrativa (#244): a alta do Leo e a linha do Nanobo
    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 38, ALLEGRO_ALIGN_CENTRE,
        "O Léo recebeu alta e vai para casa. Missão cumprida!");
    al_draw_text(font, UI_CYAN, SCREEN_W / 2, 54, ALLEGRO_ALIGN_CENTRE,
        "Anotei tudo o que aprendi. Obrigado por me guiar!");

    // o resultado da run, a esquerda
    draw_panel(24.0f, 74.0f, 290.0f, 92.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d", score);
    draw_stat(left, 84.0f, right, "pontuação final", value, UI_AMBER);

    snprintf(value, sizeof(value), "%d/%d", run_progress.neutralized, run_progress.threats);
    draw_stat(left, 102.0f, right, "ameaças eliminadas", value, UI_TEXT);

    snprintf(value, sizeof(value), "%d/%d", run_progress.identified, run_progress.agents);
    draw_stat(left, 120.0f, right, "agentes identificados", value, UI_TEXT);

    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", total_seconds / 60,
        total_seconds % 60);
    draw_stat(left, 138.0f, right, "tempo total", timer_text, UI_TEXT);

    // o Top 10, a direita, com o registro novo em destaque (#197)
    draw_ranking_panel(330.0f, 74.0f, 286.0f, ranking_highlight, false);

    // entrada do nome quando a pontuacao entra no Top 10 (#81)
    draw_panel(24.0f, 174.0f, 290.0f, 84.0f, UI_INK, name_entry_active ? UI_AMBER : UI_FRAME);

    if (name_entry_active)
    {
        float tx = 40.0f;
        bool blink = (int)(al_get_time() * 2.0) % 2 == 0;

        al_draw_text(font, UI_AMBER, 169.0f, 182.0f, ALLEGRO_ALIGN_CENTRE, "novo recorde no Top 10!");
        draw_marked_text_aligned(169.0f, 198.0f, UI_DIM, ALLEGRO_ALIGN_CENTRE, "digite seu nome e tecle [enter]");
        al_draw_filled_rectangle(tx, 214.0f, 298.0f, 232.0f, UI_PANEL);
        al_draw_rectangle(tx + 0.5f, 214.5f, 297.5f, 231.5f, UI_FRAME, 1.0f);
        al_draw_text(font, UI_TEXT, tx + 6.0f, 219.0f, 0, name_entry);

        if (blink && name_entry_chars < RANKING_NAME_CHARS)
        {
            al_draw_text(font, UI_CYAN, tx + 6.0f + (float)al_get_text_width(font, name_entry), 219.0f, 0, "_");
        }

        // o limite de 10 caracteres dito na tela
        draw_marked_text_aligned(169.0f, 240.0f, UI_DIM, ALLEGRO_ALIGN_CENTRE, "até 10 letras · [backspace] apaga");
    }
    else if (ranking_highlight >= 0)
    {
        snprintf(value, sizeof(value), "posição %d no Top 10", ranking_highlight + 1);
        al_draw_text(font, UI_GREEN, 169.0f, 202.0f, ALLEGRO_ALIGN_CENTRE, value);

        if (ranking_save_failed)
        {
            al_draw_text(font, UI_RED, 169.0f, 222.0f, ALLEGRO_ALIGN_CENTRE,
                "não foi possível gravar o recorde");
        }
    }
    else
    {
        al_draw_text(font, UI_DIM, 169.0f, 210.0f, ALLEGRO_ALIGN_CENTRE,
            "a pontuação não entrou no Top 10");
    }

    // com o nome sendo digitado, o botao espera a confirmacao
    if (!name_entry_active)
    {
        draw_buttons(buttons, count);
        draw_marked_text_aligned(SCREEN_W / 2, 312, UI_DIM, ALLEGRO_ALIGN_CENTRE,
            "[enter] também começa de novo");
    }
}

void draw_game_over()
{
    char value[32];
    char timer_text[16];
    int total_seconds = (int)run_time;
    int count;
    const UiButton* buttons = screen_buttons(&count);
    float left = 152.0f;
    float right = (float)SCREEN_W - 152.0f;

    // o quarto em luz baixa, monitor estavel: o Leo segue internado
    if (hospital_game_over)
    {
        al_draw_bitmap(hospital_game_over, 0.0f, 0.0f, 0);
    }
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H, UI_VEIL(100.0f / 255.0f));

    al_draw_text(font, UI_RED, SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    draw_rule((float)SCREEN_W / 2, 44.0f, 200.0f, UI_RED);
    // game over da narrativa (#244): a equipe retira o Nanobô e diz por que;
    // o Leo nao morre, segue no tratamento convencional
    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 52, ALLEGRO_ALIGN_CENTRE,
        game_over_patient ? "Os vitais do Léo caíram demais: a equipe retirou o Nanobô."
                          : "O Nanobô foi danificado: a equipe o retirou do corpo.");
    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 68, ALLEGRO_ALIGN_CENTRE,
        "O Léo segue no tratamento convencional do hospital.");
    al_draw_text(font, UI_CYAN, SCREEN_W / 2, 84, ALLEGRO_ALIGN_CENTRE,
        "Anotei os meus erros. Vamos tentar de novo?");

    draw_panel(140.0f, 104.0f, 360.0f, 96.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d", score);
    draw_stat(left, 114.0f, right, "pontos", value, UI_TEXT);

    snprintf(value, sizeof(value), "%d/%d", run_progress.neutralized, run_progress.threats);
    draw_stat(left, 134.0f, right, "ameaças eliminadas", value, UI_TEXT);

    snprintf(value, sizeof(value), "%d/%d", run_progress.identified, run_progress.agents);
    draw_stat(left, 154.0f, right, "agentes identificados", value, UI_TEXT);

    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", total_seconds / 60,
        total_seconds % 60);
    draw_stat(left, 174.0f, right, "tempo total", timer_text, UI_TEXT);

    draw_buttons(buttons, count);

    draw_marked_text_aligned(SCREEN_W / 2, 260, UI_DIM, ALLEGRO_ALIGN_CENTRE,
        "[enter] também reinicia");
}

// cada estado desenha so o que e dele, tudo no canvas logico
// fecha a janela de 1 s da medicao do F1: quadros desenhados e tempo medio
// de atualizacao + desenho por quadro, sem a espera do flip
void perf_sample(double now)
{
    double elapsed = now - perf_window_start;

    if (elapsed < 1.0)
    {
        return;
    }

    perf_fps = (int)((double)perf_frames / elapsed + 0.5);
    perf_frame_ms = perf_frames > 0
        ? (float)((perf_update_time + perf_draw_time) * 1000.0 / (double)perf_frames)
        : 0.0f;
    perf_window_start = now;
    perf_update_time = 0.0;
    perf_draw_time = 0.0;
    perf_frames = 0;
}

// F1: fps e tempo de quadro no canto de cima, por cima de qualquer cena
void draw_perf_readout()
{
    char line[48];
    int w;

    snprintf(line, sizeof(line), "fps %d · quadro %.1f ms", perf_fps, perf_frame_ms);
    w = al_get_text_width(font, line);
    al_draw_filled_rectangle((float)(SCREEN_W - w - 12), 2.0f, (float)(SCREEN_W - 2), 14.0f, UI_INK);
    al_draw_text(font, UI_DIM, (float)(SCREEN_W - 7), 4.0f, ALLEGRO_ALIGN_RIGHT, line);
}

void render()
{
    GameScene scene = game_get_scene();
    double draw_start = al_get_time();

    // trocou de tela, o foco volta para o primeiro botao
    if (scene != ui_last_scene)
    {
        // de volta do como jogar, a pausa e o menu focam o COMO JOGAR de onde saiu
        ui_focus = (scene == GAME_SCENE_PAUSE || scene == GAME_SCENE_MENU) && ui_last_scene == GAME_SCENE_HOWTO ? 1 : 0;
        ui_last_scene = scene;

        // na fase o cursor do sistema vira mira; nas outras telas ele volta (#198)
        if (scene == GAME_SCENE_GAME)
        {
            al_hide_mouse_cursor(display);
        }
        else
        {
            al_show_mouse_cursor(display);
        }
    }

    update_screen_fit();
    sync_screen_backgrounds(scene);
    al_set_target_bitmap(canvas);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (scene == GAME_SCENE_MENU)
    {
        draw_menu();
    }
    else if (scene == GAME_SCENE_BRIEFING)
    {
        draw_briefing();
    }
    else if (scene == GAME_SCENE_GAME)
    {
        draw_game();
        draw_concept_card();
    }
    else if (scene == GAME_SCENE_PAUSE)
    {
        // o jogo congelado continua visivel atras do painel
        draw_game();
        draw_pause();
    }
    else if (scene == GAME_SCENE_RESULTS)
    {
        draw_results();
    }
    else if (scene == GAME_SCENE_VICTORY)
    {
        draw_victory();
    }
    else if (scene == GAME_SCENE_GAME_OVER)
    {
        draw_game_over();
    }
    else if (scene == GAME_SCENE_HOWTO)
    {
        draw_howto();
    }

    if (show_debug)
    {
        draw_perf_readout();
    }

    // o canvas vai para a janela com escala inteira e barras pretas
    al_set_target_backbuffer(display);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(canvas, 0, 0, SCREEN_W, SCREEN_H,
        screen_off_x, screen_off_y,
        (float)SCREEN_W * screen_scale, (float)SCREEN_H * screen_scale, 0);

    perf_draw_time = perf_draw_time + (al_get_time() - draw_start);
    perf_frames = perf_frames + 1;
    al_flip_display();
}

// teclado, cada estado so olha as teclas que sao dele
void handle_key_down(int key)
{
    GameScene scene = game_get_scene();

    if (key == ALLEGRO_KEY_F11)
    {
        toggle_fullscreen();
        return;
    }

    // F1 liga a leitura de QA sobre a tela, em qualquer cena (#202)
    if (key == ALLEGRO_KEY_F1)
    {
        show_debug = !show_debug;
        return;
    }

    // fora do jogo quem responde e a interface: foco e confirmacao
    if (scene != GAME_SCENE_GAME)
    {
        // nome sendo digitado: enter ou esc confirmam; o resto e texto (#81)
        if (scene == GAME_SCENE_VICTORY && name_entry_active)
        {
            if (key == ALLEGRO_KEY_ENTER || key == ALLEGRO_KEY_PAD_ENTER || key == ALLEGRO_KEY_ESCAPE)
            {
                confirm_name_entry();
            }

            return;
        }

        // painel do Top 10 no menu: enter ou esc fecham (#197)
        if (scene == GAME_SCENE_MENU && ranking_panel_open)
        {
            if (key == ALLEGRO_KEY_ENTER || key == ALLEGRO_KEY_ESCAPE)
            {
                ranking_panel_open = false;
            }

            return;
        }

        // como jogar: enter, espaco avancam; setas trocam a pagina; esc volta a origem
        if (scene == GAME_SCENE_HOWTO)
        {
            if (key == ALLEGRO_KEY_ENTER || key == ALLEGRO_KEY_PAD_ENTER || key == ALLEGRO_KEY_SPACE)
            {
                howto_advance();
            }
            else if (key == ALLEGRO_KEY_LEFT && howto_page > 0)
            {
                howto_page--;
            }
            else if (key == ALLEGRO_KEY_RIGHT && howto_page < HOWTO_PAGES - 1)
            {
                howto_page++;
            }
            else if (key == ALLEGRO_KEY_ESCAPE)
            {
                howto_return();
            }

            return;
        }

        // botoes lado a lado: as setas laterais tambem andam o foco (#243)
        if (scene != GAME_SCENE_BRIEFING && (key == ALLEGRO_KEY_LEFT || key == ALLEGRO_KEY_A))
        {
            ui_move_focus(-1);
            return;
        }

        if (scene != GAME_SCENE_BRIEFING && (key == ALLEGRO_KEY_RIGHT || key == ALLEGRO_KEY_D))
        {
            ui_move_focus(1);
            return;
        }

        if (key == ALLEGRO_KEY_UP || key == ALLEGRO_KEY_W)
        {
            ui_move_focus(-1);
            return;
        }

        if (key == ALLEGRO_KEY_DOWN || key == ALLEGRO_KEY_S)
        {
            ui_move_focus(1);
            return;
        }

        // abertura: enter ou espaco avancam a caixa (#244)
        if (scene == GAME_SCENE_BRIEFING && (key == ALLEGRO_KEY_ENTER || key == ALLEGRO_KEY_SPACE))
        {
            press_opening();
            return;
        }

        if (scene == GAME_SCENE_BRIEFING && key == ALLEGRO_KEY_ESCAPE)
        {
            game_set_scene(GAME_SCENE_GAME);
            return;
        }

        if (key == ALLEGRO_KEY_ENTER)
        {
            activate_button(ui_focus);
            return;
        }

        if (scene == GAME_SCENE_PAUSE && key == ALLEGRO_KEY_ESCAPE)
        {
            game_set_scene(GAME_SCENE_GAME);
        }

        return;
    }

    // caixa do diario na tela: enter ou espaco avancam, esc fecha a entrada;
    // as setas e o WASD continuam registrados, para o movimento nao se perder (#222)
    if (pending_card() != CARD_NONE)
    {
        bool movement = key == ALLEGRO_KEY_UP || key == ALLEGRO_KEY_W
            || key == ALLEGRO_KEY_DOWN || key == ALLEGRO_KEY_S
            || key == ALLEGRO_KEY_LEFT || key == ALLEGRO_KEY_A
            || key == ALLEGRO_KEY_RIGHT || key == ALLEGRO_KEY_D;

        if (key == ALLEGRO_KEY_ENTER || key == ALLEGRO_KEY_SPACE)
        {
            press_card_dialog();
            return;
        }

        if (key == ALLEGRO_KEY_ESCAPE)
        {
            skip_card_dialog();
            return;
        }

        if (!movement)
        {
            return;
        }
    }

    if (key == ALLEGRO_KEY_UP || key == ALLEGRO_KEY_W)
    {
        key_up = true;
    }
    else if (key == ALLEGRO_KEY_DOWN || key == ALLEGRO_KEY_S)
    {
        key_down = true;
    }
    else if (key == ALLEGRO_KEY_LEFT || key == ALLEGRO_KEY_A)
    {
        key_left = true;
    }
    else if (key == ALLEGRO_KEY_RIGHT || key == ALLEGRO_KEY_D)
    {
        key_right = true;
    }
    else if (key == ALLEGRO_KEY_ESCAPE)
    {
        game_set_scene(GAME_SCENE_PAUSE);
    }
    else if (key == ALLEGRO_KEY_X)
    {
        activate_defense();
    }
    // 1, 2 e 3 escolhem a ferramenta da posicao na fase atual (#216, #88)
    else if (key == ALLEGRO_KEY_1)
    {
        select_tool(0);
    }
    else if (key == ALLEGRO_KEY_2)
    {
        select_tool(1);
    }
    else if (key == ALLEGRO_KEY_3)
    {
        select_tool(2);
    }
#ifdef _DEBUG
    // teclas de QA: so existem no Debug, nunca no Release (#76)
    // F forca a condicao real de conclusao para testar a transicao da #70
    else if (key == ALLEGRO_KEY_F)
    {
        debug_complete_phase(false);
    }
    // G forca Game Over; vida zerada continua sendo o caminho real
    else if (key == ALLEGRO_KEY_G)
    {
        game_set_scene(GAME_SCENE_GAME_OVER);
    }
    // L encurta o lisogenico de todo virus para 1 s, para testar o fim (#84)
    else if (key == ALLEGRO_KEY_L)
    {
        debug_short_lysogenic();
    }
    // K poe um biofilme e uma nuvem de endotoxina perto do nanobo (#237)
    else if (key == ALLEGRO_KEY_K)
    {
        qa_spawn_hazards();
    }
    // J avanca uma camada do scan em todo agente, para conferir o desenho (#214)
    else if (key == ALLEGRO_KEY_J)
    {
        debug_add_layer();
    }
    // H identifica os 9 e conclui, para testar o caminho do bonus (#67)
    else if (key == ALLEGRO_KEY_H)
    {
        debug_complete_phase(true);
    }
#endif
}

// soltou a tecla, para de andar naquela direcao
// vale em qualquer cena: se o soltar fosse ignorado durante a pausa, a tecla
// solta ali deixaria o nanobo andando sozinho ao voltar pro jogo
void handle_key_up(int key)
{
    if (key == ALLEGRO_KEY_UP || key == ALLEGRO_KEY_W)
    {
        key_up = false;
    }

    if (key == ALLEGRO_KEY_DOWN || key == ALLEGRO_KEY_S)
    {
        key_down = false;
    }

    if (key == ALLEGRO_KEY_LEFT || key == ALLEGRO_KEY_A)
    {
        key_left = false;
    }

    if (key == ALLEGRO_KEY_RIGHT || key == ALLEGRO_KEY_D)
    {
        key_right = false;
    }
}

// clique na fase: esquerdo atira para o cursor, direito escaneia o alvo sob ele (#198)
void handle_game_click(int button, float lx, float ly)
{
    float wx;
    float wy;

    // caixa do diario na tela: o clique esquerdo avanca a caixa e nao atira
    // nem escaneia; volta antes do codigo de tiro e de scan (#222)
    if (pending_card() != CARD_NONE)
    {
        if (button == 1)
        {
            press_card_dialog();
        }

        return;
    }

    // clique no painel do HUD e na barra preta da janela nao e acao de mundo
    if (lx < 0.0f || lx >= (float)SCREEN_W || ly < 0.0f || ly >= (float)PLAY_H)
    {
        return;
    }

    wx = lx + camera.x;
    wy = ly + camera.y;

    if (button == 1)
    {
        shoot_toward(wx, wy);
    }
    else if (button == 2)
    {
        scan_at(wx, wy);
    }
}

// clique do mouse vem em pixel da janela: converte para pixel da tela logica
void handle_mouse_click(int button, float x, float y)
{
    float lx = to_logical_x(x);
    float ly = to_logical_y(y);
    int count;
    const UiButton* buttons;
    int i;

    // na fase o clique e acao de jogo, nao botao de tela (#198)
    if (game_get_scene() == GAME_SCENE_GAME)
    {
        handle_game_click(button, lx, ly);
        return;
    }

    // o painel do Top 10 fecha com um clique; o nome sendo digitado espera o enter
    if (game_get_scene() == GAME_SCENE_MENU && ranking_panel_open)
    {
        ranking_panel_open = false;
        return;
    }

    if (game_get_scene() == GAME_SCENE_VICTORY && name_entry_active)
    {
        return;
    }

    // como jogar: o clique esquerdo avanca a pagina (na ultima, volta)
    if (game_get_scene() == GAME_SCENE_HOWTO)
    {
        if (button == 1)
        {
            howto_advance();
        }

        return;
    }

    // abertura: o clique esquerdo avanca a caixa
    if (game_get_scene() == GAME_SCENE_BRIEFING)
    {
        if (button == 1)
        {
            press_opening();
        }

        return;
    }

    buttons = screen_buttons(&count);

    if (buttons == NULL)
    {
        return;
    }

    for (i = 0; i < count; i++)
    {
        if (button_hit(&buttons[i], lx, ly))
        {
            activate_button(i);
            return;
        }
    }
}

// toca o som do ultimo evento de gameplay, uma vez por evento (#31)
void play_pending_sfx()
{
    SfxEvent kind;

    if (!audio_ready || sfx_event_id() == sfx_seen)
    {
        return;
    }

    sfx_seen = sfx_event_id();
    kind = sfx_event_kind();

    if (kind == SFX_EVENT_SCAN)
    {
        al_play_sample(sfx_scan, SFX_GAIN, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (kind == SFX_EVENT_SHOT)
    {
        al_play_sample(sfx_shot, SFX_GAIN, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (kind == SFX_EVENT_HIT)
    {
        al_play_sample(sfx_hit, SFX_GAIN, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (kind == SFX_EVENT_NEUTRALIZED || kind == SFX_EVENT_PHASE_DONE)
    {
        al_play_sample(sfx_neutralized, SFX_GAIN, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (kind == SFX_EVENT_WRONG || kind == SFX_EVENT_DAMAGE)
    {
        al_play_sample(sfx_penalty, SFX_GAIN, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (kind == SFX_EVENT_DEFENSE)
    {
        al_play_sample(sfx_defense, SFX_GAIN, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

// a musica para na pausa e volta ao sair dela (#32)
void sync_music()
{
    // o como jogar aberto pela pausa continua sem musica
    bool paused = game_get_scene() == GAME_SCENE_PAUSE
        || (game_get_scene() == GAME_SCENE_HOWTO && game_howto_origin() == GAME_SCENE_PAUSE);
    bool want = audio_ready && music != NULL && !paused;

    if (want && !music_playing)
    {
        al_play_sample(music, MUSIC_GAIN, 0.0f, 1.0f, ALLEGRO_PLAYMODE_LOOP, &music_id);
        music_playing = true;
    }
    else if (!want && music_playing)
    {
        al_stop_sample(&music_id);
        music_playing = false;
    }
}

int main()
{
    // o titulo da janela sai do nome do app
    al_set_app_name("Nanobo");

    if (!al_init())
    {
        printf("couldn't initialize allegro\n");
        return 1;
    }

	if (!al_init_font_addon())
	{
		printf("couldn't initialize font addon\n");
		return 1;
	}

	if (!al_init_ttf_addon())
	{
		printf("couldn't initialize ttf addon\n");
		return 1;
	}

    if (!al_init_image_addon())
    {
        printf("couldn't initialize image addon\n");
        return 1;
    }

    // audio opcional: sem placa de som o jogo abre mudo (#31)
    if (al_install_audio() && al_init_acodec_addon())
    {
        if (!al_reserve_samples(8))
        {
            printf("couldn't reserve audio samples\n");
        }

        audio_ready = true;
    }
    else
    {
        printf("couldn't initialize audio: the game runs silent\n");
    }

    if (!al_install_keyboard())
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }

    if (!al_install_mouse()) {
        printf("couldn't initialize mouse\n");
        return 1;
    }
    
    const int FPS = 60;
    const double deltaTime = 1.0 / FPS;

    ALLEGRO_TIMER* timer = al_create_timer(deltaTime);
    if (!timer)
    {
        printf("couldn't initialize timer\n");
        return 1;
    }

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue)
    {
        printf("couldn't initialize queue\n");
        return 1;
    }

    if (!create_window())
    {
        al_destroy_timer(timer);
        al_destroy_event_queue(queue);
        return 1;
    }

    if (!load_resources())
    {
        free_resources();
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(queue);
        return 1;
    }

    // o Top 10 e lido uma vez, antes do primeiro desenho do menu, do
    // nanobo_ranking.txt na pasta do jogo, junto dos assets (#77, #78; dono, 15/11)
    load_hi_score();

    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    double tick_start = 0.0;

    ALLEGRO_EVENT event;

    // semente do sorteio, uma vez por execucao: o relogio em segundos se repete
    // entre duas aberturas no mesmo segundo, entao o relogio do allegro entra junto
    srand((unsigned int)time(NULL) * 1000u + (unsigned int)(al_get_time() * 1000.0));

    // o menu mostra ameaças e agentes da fase: a Fase 1 nasce antes do loop (#M4)
    reset_match();

    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                tick_start = al_get_time();

                // a partida so atualiza no estado GAME
                // hit-stop: o mundo segura uns quadros depois de uma eliminacao;
                // a entrada continua chegando (#221)
                if (game_get_scene() == GAME_SCENE_GAME && feel_hit_stop > 0.0)
                {
                    feel_hit_stop = feel_hit_stop - deltaTime;
                }
                else if (game_get_scene() == GAME_SCENE_GAME)
                {
                    update_match(deltaTime);

                    if (player.health <= 0.0f || patient_vitals <= 0.0f)
                    {
                        // a tela diz qual barra zerou (#234)
                        game_over_patient = patient_vitals <= 0.0f;

                        // game over nao grava recorde: so a vitoria entra (#34/#82)
                        game_set_scene(GAME_SCENE_GAME_OVER);
                    }
                    // fase cumprida entra em Resultados sozinha (#70)
                    else if (phase_completed())
                    {
                        game_set_scene(GAME_SCENE_RESULTS);
                    }
                }
                // a caixa do diario revela as letras mesmo com update_match parado;
                // na pausa por perda de foco ela fica na mesma pagina e letra
                sync_card_dialog();

                if (game_get_scene() == GAME_SCENE_GAME && card_dialog_entry != CARD_NONE)
                {
                    dialog_tick(&card_dialog, (float)deltaTime);
                }

                if (game_get_scene() == GAME_SCENE_BRIEFING)
                {
                    dialog_tick(&opening_dialog, (float)deltaTime);
                }

                feel_trigger();

                // na pausa (e no como jogar) os efeitos ficam parados junto com o jogo
                if (game_get_scene() != GAME_SCENE_PAUSE && game_get_scene() != GAME_SCENE_HOWTO)
                {
                    feel_update((float)deltaTime);
                }
                play_pending_sfx();
                sync_music();

                perf_update_time = perf_update_time + (al_get_time() - tick_start);
                perf_sample(al_get_time());
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                handle_key_down(event.keyboard.keycode);
                break;

            case ALLEGRO_EVENT_KEY_UP:
                handle_key_up(event.keyboard.keycode);
                break;

            // texto da entrada de nome, com a repeticao do teclado (#81)
            case ALLEGRO_EVENT_KEY_CHAR:
                handle_key_char(event.keyboard.keycode, event.keyboard.unichar);
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                handle_mouse_click(event.mouse.button, event.mouse.x, event.mouse.y);
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                // hover: guarda o pixel da janela, quem desenha converte
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;

                // roda do mouse troca o modulo dentro da fase (#198)
                if (event.mouse.dz != 0 && game_get_scene() == GAME_SCENE_GAME && pending_card() == CARD_NONE)
                {
                    toggle_module();
                }
                break;

            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                al_acknowledge_resize(event.display.source);
                refresh_mouse();
                redraw = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                // alt-tab nao pode matar o jogador: a partida pausa ao perder o foco (#19)
                if (game_get_scene() == GAME_SCENE_GAME)
                {
                    game_set_scene(GAME_SCENE_PAUSE);
                }
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            render();
            redraw = false;
        }
    }

    free_resources();

    if (canvas)
    {
        al_destroy_bitmap(canvas);
        canvas = NULL;
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}