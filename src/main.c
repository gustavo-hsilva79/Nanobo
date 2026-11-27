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
#include "ui/ui.h"

// volume dos efeitos e da musica, de 0 a 1 (#31, #32)
#define SFX_GAIN 0.55f
#define MUSIC_GAIN 0.30f

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

// como jogar: pagina atual (0 a 3), zerada a cada entrada na cena
int howto_page = 0;

// Top 10 na interface (#81, #197): entrada de nome na Vitoria, painel do
// ranking no menu e destaque do registro novo; nenhuma cena nova
bool name_entry_active = false;
char name_entry[RANKING_NAME_BYTES] = "";
int name_entry_chars = 0;
int ranking_highlight = -1;
bool ranking_save_failed = false;
bool ranking_panel_open = false;

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
    w = ui_text_width(line);
    al_draw_filled_rectangle((float)(SCREEN_W - w - 12), 2.0f, (float)(SCREEN_W - 2), 14.0f, UI_INK);
    ui_text(UI_DIM, (float)(SCREEN_W - 7), 4.0f, ALLEGRO_ALIGN_RIGHT, line);
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

    wx = lx / WORLD_SCALE + camera.x;
    wy = ly / WORLD_SCALE + camera.y;

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

