#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
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

#define BUTTON_X 240
#define BUTTON_Y 266
#define BUTTON_W 160
#define BUTTON_H 28

// medidas do HUD: tudo derivado do painel, nunca coordenada magica
#define HUD_MARGIN 10
#define HUD_LINES 5
#define HUD_LINE_H ((PANEL_H - 2 * HUD_MARGIN) / HUD_LINES)

// a fonte embutida tem 8 px: menos de 12 por linha encosta uma na outra
_Static_assert(HUD_LINE_H >= 12, "linha do HUD apertada demais para a fonte");
_Static_assert(HUD_LINES * HUD_LINE_H + 2 * HUD_MARGIN <= PANEL_H, "HUD nao cabe no painel");

// medidas do kit de interface, todas derivadas do painel e da fonte
#define UI_PAD 8
#define UI_GAP 12
#define UI_BAR_SEGMENTS 4
#define UI_SEG_W 8
#define UI_SEG_H 6
#define UI_SEG_GAP 2
#define UI_BAR_W (UI_BAR_SEGMENTS * UI_SEG_W + (UI_BAR_SEGMENTS - 1) * UI_SEG_GAP)

// barra dos sinais vitais no HUD e a faixa em que ela fica vermelha (#218)
#define VITALS_BAR_W 48
#define VITALS_LOW 30.0f
#define TITLE_SCALE 3

// cartoes do briefing de entrada (#103)
#define BRIEFING_CARDS 3

// linhas por cartao: o terceiro cartao usa quatro
#define BRIEFING_LINES 4

// paleta da interface: os mesmos hex do catalogo de recursos
#define UI_INK       al_map_rgb(18, 26, 36)    // #121A24
#define UI_PANEL     al_map_rgb(26, 38, 52)    // #1A2634
#define UI_TEXT      al_map_rgb(242, 245, 247) // #F2F5F7
#define UI_DIM       al_map_rgb(168, 184, 200) // #A8B8C8
#define UI_AMBER     al_map_rgb(255, 210, 63)  // #FFD23F
#define UI_PURPLE    al_map_rgb(190, 110, 240) // #BE6EF0
#define UI_RED       al_map_rgb(240, 90, 90)   // #F05A5A
#define UI_PINK      al_map_rgb(240, 130, 165) // Gram-: rosa da safranina (#214)

// mapa recuado e silhueta neutra: os agentes precisam se destacar do vermelho (#220)
#define MAP_TINT        al_map_rgb_f(0.68f, 0.62f, 0.66f)
#define MAP_VEIL        al_map_rgba_f(0.05f * 0.28f, 0.07f * 0.28f, 0.11f * 0.28f, 0.28f)
#define SILHOUETTE_FILL al_map_rgb(86, 104, 132)
#define SILHOUETTE_EDGE al_map_rgb(170, 190, 215)
#define CONTACT_SHADOW  al_map_rgba_f(0.0f, 0.0f, 0.0f, 0.35f)

// tingimento do sprite cinza da bacteria depois da camada 2 (#214)
#define GRAM_POS_TINT   al_map_rgb_f(0.85f, 0.55f, 1.0f)
#define GRAM_NEG_TINT   al_map_rgb_f(1.0f, 0.62f, 0.74f)
#define UI_CYAN      al_map_rgb(125, 249, 255) // #7DF9FF
#define UI_GREEN     al_map_rgb(110, 220, 120) // #6EDC78
#define UI_WARM      al_map_rgb(255, 232, 148) // #FFE894
#define UI_SLOT      al_map_rgb(52, 64, 78)    // slot vazio, sem brilho
#define UI_HOVER     al_map_rgb(56, 46, 16)    // fundo do botao sob o cursor
#define UI_LINE      al_map_rgb(48, 60, 74)    // separador e molduras discretas
#define UI_FRAME     al_map_rgb(76, 92, 108)   // moldura de painel

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

// coracao do HUD: icone dos sinais vitais (#19, #218)
ALLEGRO_BITMAP* heart_icon = NULL;

// icones da interface: scanner, as duas armas e a defesa
ALLEGRO_BITMAP* scanner_icon = NULL;
ALLEGRO_BITMAP* ruptura_icon = NULL;
ALLEGRO_BITMAP* dissolucao_icon = NULL;
ALLEGRO_BITMAP* defense_icon = NULL;

// titulo do menu montado a partir da fonte, desenhado ampliado
ALLEGRO_BITMAP* title_bitmap = NULL;

// a arte de cada especie da Fase 1
ALLEGRO_BITMAP* gram_positive = NULL;
ALLEGRO_BITMAP* gram_negative = NULL;
ALLEGRO_BITMAP* cell_sprite = NULL;
ALLEGRO_BITMAP* virus_sprite = NULL;

// segunda variante de cada especie (#211)
ALLEGRO_BITMAP* gram_positive_b = NULL;
ALLEGRO_BITMAP* gram_negative_b = NULL;
ALLEGRO_BITMAP* cell_sprite_b = NULL;
ALLEGRO_BITMAP* virus_sprite_b = NULL;

// bacterias em cinza, uma por forma e variante: a cor de Gram vem do tingimento (#214)
// o coco sai do cacho e da cadeia, o bacilo do bastao e do par de bastoes
ALLEGRO_BITMAP* coccus_grey[SPRITE_VARIANTS] = { NULL };
ALLEGRO_BITMAP* bacillus_grey[SPRITE_VARIANTS] = { NULL };

// mapa do mundo inteiro da Fase 1 (1920x512), sempre atras do resto
ALLEGRO_BITMAP* world_map = NULL;

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
int briefing_card = 0;

// overlay de depuracao da interface: F1 liga e desliga, so existe no Debug
bool show_debug = false;
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

// copia em tons de cinza de um sprite, para ser tingida na hora de desenhar (#214)
// o PNG carregado ja tem alfa pre-multiplicado, e a copia mantem isso
ALLEGRO_BITMAP* make_grey_copy(ALLEGRO_BITMAP* source)
{
    ALLEGRO_BITMAP* copy;
    ALLEGRO_BITMAP* old_target = al_get_target_bitmap();
    int w = al_get_bitmap_width(source);
    int h = al_get_bitmap_height(source);
    int x;
    int y;

    copy = al_create_bitmap(w, h);
    if (!copy)
    {
        return NULL;
    }

    al_set_target_bitmap(copy);
    al_lock_bitmap(source, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
    al_lock_bitmap(copy, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);

    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            float r;
            float g;
            float b;
            float a;
            float grey;

            al_unmap_rgba_f(al_get_pixel(source, x, y), &r, &g, &b, &a);

            // luminancia com um pouco mais de brilho, para o tingimento aparecer
            grey = (0.299f * r + 0.587f * g + 0.114f * b) * 1.35f;
            if (grey > a)
            {
                grey = a;
            }

            al_put_pixel(x, y, al_map_rgba_f(grey, grey, grey, a));
        }
    }

    al_unlock_bitmap(copy);
    al_unlock_bitmap(source);
    al_set_target_bitmap(old_target);
    return copy;
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

    gram_positive = al_load_bitmap("assets/sprites/bacteria_gram_positiva.png");
    if (!gram_positive)
    {
        printf("couldn't load gram positive\n");
        return false;
    }

    gram_negative = al_load_bitmap("assets/sprites/bacteria_gram_negativa.png");
    if (!gram_negative)
    {
        printf("couldn't load gram negative\n");
        return false;
    }

    cell_sprite = al_load_bitmap("assets/sprites/celula_propria.png");
    if (!cell_sprite)
    {
        printf("couldn't load cell\n");
        return false;
    }

    virus_sprite = al_load_bitmap("assets/sprites/virus_fase2.png");
    if (!virus_sprite)
    {
        printf("couldn't load virus\n");
        return false;
    }

    gram_positive_b = al_load_bitmap("assets/sprites/bacteria_gram_positiva_b.png");
    if (!gram_positive_b)
    {
        printf("couldn't load gram positive variant\n");
        return false;
    }

    gram_negative_b = al_load_bitmap("assets/sprites/bacteria_gram_negativa_b.png");
    if (!gram_negative_b)
    {
        printf("couldn't load gram negative variant\n");
        return false;
    }

    cell_sprite_b = al_load_bitmap("assets/sprites/celula_propria_b.png");
    if (!cell_sprite_b)
    {
        printf("couldn't load cell variant\n");
        return false;
    }

    virus_sprite_b = al_load_bitmap("assets/sprites/virus_fase2_b.png");
    if (!virus_sprite_b)
    {
        printf("couldn't load virus variant\n");
        return false;
    }

    // a camada 1 mostra a forma sem a cor: copias cinza das bacterias (#214)
    coccus_grey[0] = make_grey_copy(gram_positive);
    coccus_grey[1] = make_grey_copy(gram_positive_b);
    bacillus_grey[0] = make_grey_copy(gram_negative);
    bacillus_grey[1] = make_grey_copy(gram_negative_b);
    if (!coccus_grey[0] || !coccus_grey[1] || !bacillus_grey[0] || !bacillus_grey[1])
    {
        printf("couldn't create grey bacteria\n");
        return false;
    }

    heart_icon = al_load_bitmap("assets/sprites/icone_coracao.png");
    if (!heart_icon)
    {
        printf("couldn't load heart icon\n");
        return false;
    }

    scanner_icon = al_load_bitmap("assets/sprites/icone_scanner.png");
    if (!scanner_icon)
    {
        printf("couldn't load scanner icon\n");
        return false;
    }

    ruptura_icon = al_load_bitmap("assets/sprites/icone_ruptura.png");
    if (!ruptura_icon)
    {
        printf("couldn't load ruptura icon\n");
        return false;
    }

    dissolucao_icon = al_load_bitmap("assets/sprites/icone_dissolucao.png");
    if (!dissolucao_icon)
    {
        printf("couldn't load dissolucao icon\n");
        return false;
    }

    defense_icon = al_load_bitmap("assets/sprites/icone_defesa.png");
    if (!defense_icon)
    {
        printf("couldn't load defense icon\n");
        return false;
    }

    title_bitmap = make_text_bitmap("NANOBÔ", al_map_rgb(255, 255, 255));
    if (!title_bitmap)
    {
        printf("couldn't build title\n");
        return false;
    }

    world_map = al_load_bitmap("assets/backgrounds/mapa_fase1.png");
    if (!world_map)
    {
        printf("couldn't load world map\n");
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
    }

    if (gram_positive)
    {
        al_destroy_bitmap(gram_positive);
        gram_positive = NULL;
    }

    if (gram_negative)
    {
        al_destroy_bitmap(gram_negative);
        gram_negative = NULL;
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

    if (gram_positive_b)
    {
        al_destroy_bitmap(gram_positive_b);
        gram_positive_b = NULL;
    }

    if (gram_negative_b)
    {
        al_destroy_bitmap(gram_negative_b);
        gram_negative_b = NULL;
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

    if (heart_icon)
    {
        al_destroy_bitmap(heart_icon);
        heart_icon = NULL;
    }

    if (defense_icon)
    {
        al_destroy_bitmap(defense_icon);
        defense_icon = NULL;
    }

    if (dissolucao_icon)
    {
        al_destroy_bitmap(dissolucao_icon);
        dissolucao_icon = NULL;
    }

    if (ruptura_icon)
    {
        al_destroy_bitmap(ruptura_icon);
        ruptura_icon = NULL;
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
        { (float)BUTTON_X, (float)BUTTON_Y, (float)BUTTON_W, (float)BUTTON_H, "INICIAR" }
    };
    static const UiButton pause_items[] = {
        { 240.0f, 170.0f, 160.0f, 28.0f, "CONTINUAR" }
    };
    static const UiButton results_items[] = {
        { 240.0f, 256.0f, 160.0f, 28.0f, "CONTINUAR" }
    };
    static const UiButton briefing_items[] = {
        { 180.0f, 198.0f, 140.0f, 28.0f, "SEGUIR" },
        { 330.0f, 198.0f, 140.0f, 28.0f, "PULAR" }
    };
    static const UiButton victory_items[] = {
        { 240.0f, 212.0f, 160.0f, 28.0f, "NOVA PARTIDA" }
    };
    static const UiButton over_items[] = {
        { 240.0f, 204.0f, 160.0f, 28.0f, "NOVA PARTIDA" }
    };

    *count = 1;

    switch (game_get_scene())
    {
        case GAME_SCENE_MENU:
            return menu_items;

        case GAME_SCENE_BRIEFING:
            *count = 2;
            return briefing_items;

        case GAME_SCENE_PAUSE:
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
            reset_match();
            briefing_card = 0;
            game_set_scene(GAME_SCENE_BRIEFING);
            break;

        case GAME_SCENE_BRIEFING:
            // indice 0 avanca um cartao e entra na fase no ultimo; 1 pula o resto
            if (index == 1 || briefing_card + 1 == BRIEFING_CARDS)
            {
                game_set_scene(GAME_SCENE_GAME);
            }
            else
            {
                briefing_card = briefing_card + 1;
                ui_focus = 0;
            }
            break;

        case GAME_SCENE_PAUSE:
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
                finish_run();
                game_set_scene(GAME_SCENE_VICTORY);
            }
            break;

        case GAME_SCENE_VICTORY:
        case GAME_SCENE_GAME_OVER:
            // briefing antes de toda partida, pulavel com ESC
            reset_match();
            briefing_card = 0;
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
    if (species == SPECIES_CELL || species == SPECIES_VIRUS)
    {
        return;
    }

    al_draw_filled_rectangle(x, y + 1.0f, x + 8.0f, y + 9.0f,
        species == SPECIES_GRAM_POSITIVE ? UI_PURPLE : UI_PINK);
}

void draw_scan_panel()
{
    int target;
    int hover;
    int panel_y = hud_y(2) - 6;
    int text_x = 36;
    int row_state = hud_y(2);
    int row_info = hud_y(3);
    int row_action = hud_y(4);
    const char* state;
    const char* category;
    const char* source;
    ALLEGRO_COLOR source_color;
    float lx;
    float ly;
    float world_x;
    float world_y;

    draw_panel(8.0f, (float)panel_y, (float)SCREEN_W - 16.0f,
        (float)(3 * HUD_LINE_H + 12), UI_PANEL, UI_FRAME);

    al_draw_bitmap(scanner_icon, 14.0f, (float)(row_state - 4), 0);

    // o painel segue o agente sob o cursor; sem alvo no cursor, fica a ultima leitura
    lx = to_logical_x(mouse_x);
    ly = to_logical_y(mouse_y);
    hover = -1;

    // cursor sobre o painel do HUD nao tem alvo: o clique ali nao escaneia
    if (lx >= 0.0f && lx < (float)SCREEN_W && ly >= 0.0f && ly < (float)PLAY_H)
    {
        world_x = lx + camera.x;
        world_y = ly + camera.y;
        hover = pick_target_at(world_x, world_y);
    }

    target = hover != -1 ? hover : last_scan_target();

    if (target == -1)
    {
        al_draw_text(font, UI_AMBER, (float)text_x, (float)row_state, 0, "Nenhum alvo");
        al_draw_text(font, UI_TEXT, (float)text_x, (float)row_info, 0,
            "Aproxime-se de um agente para escanear.");
        al_draw_text(font, UI_DIM, (float)text_x, (float)row_action, 0,
            "botão direito sobre o agente");
        return;
    }

    // de onde vem a leitura: o cursor manda, a ultima leitura e o retorno (#199)
    if (hover != -1)
    {
        source = " · alvo sob o cursor";
        source_color = UI_CYAN;
    }
    else
    {
        source = " · última leitura";
        source_color = UI_DIM;
    }

    if (!agents[target].active)
    {
        al_draw_text(font, UI_DIM, (float)text_x, (float)row_state, 0, "Alvo neutralizado");
        al_draw_text(font, source_color,
            (float)(text_x + al_get_text_width(font, "Alvo neutralizado")),
            (float)row_state, 0, source);
        al_draw_text(font, UI_TEXT, (float)text_x, (float)row_info, 0,
            "Chegue perto de outro agente.");
        al_draw_text(font, UI_DIM, (float)text_x, (float)row_action, 0,
            "botão direito sobre o agente");
        return;
    }

    if (agents[target].layers == 0)
    {
        state = "Alvo detectado";
    }
    else if (agents[target].layers < MAX_LAYERS)
    {
        state = "Leitura 1 de 2";
    }
    else
    {
        state = "Identificação concluída";
    }

    if (agents[target].species == SPECIES_CELL)
    {
        category = "célula";
    }
    else if (agents[target].species == SPECIES_VIRUS)
    {
        category = "vírus";
    }
    else
    {
        category = "bactéria";
    }

    al_draw_text(font, UI_AMBER, (float)text_x, (float)row_state, 0, state);
    al_draw_text(font, source_color, (float)(text_x + al_get_text_width(font, state)),
        (float)row_state, 0, source);
    al_draw_text(font, UI_TEXT, (float)(SCREEN_W - 16), (float)row_state,
        ALLEGRO_ALIGN_RIGHT, category);

    if (agents[target].layers == 0)
    {
        al_draw_text(font, UI_TEXT, (float)text_x, (float)row_info, 0,
            "Clique com o botão direito para escanear.");
        al_draw_text(font, UI_DIM, (float)text_x, (float)row_action, 0,
            "o scan revela os dados do agente");
        return;
    }

    // camada 1: so a forma, sem amostra de cor que entregue o grupo (#214)
    if (agents[target].layers < MAX_LAYERS)
    {
        al_draw_text(font, UI_TEXT, (float)(text_x + 12), (float)row_info, 0,
            layer_one_text(target));
        al_draw_text(font, UI_DIM, (float)text_x, (float)row_action, 0,
            agents[target].species == SPECIES_GRAM_POSITIVE
                || agents[target].species == SPECIES_GRAM_NEGATIVE
                ? "clique de novo para revelar a coloração"
                : "clique de novo para concluir a leitura");
        return;
    }

    // camada 2: coloracao, estrutura e o que a celula e, sem indicar a arma (#214)
    draw_scan_swatch(agents[target].species, (float)text_x, (float)row_info);
    al_draw_text(font, UI_WARM, (float)(text_x + 12), (float)row_info, 0,
        layer_two_text(target));
    al_draw_text(font, UI_WARM, (float)text_x, (float)row_action, 0,
        layer_two_reason(target));
}

// ---------------------------------------------------------------
// menu inicial
// ---------------------------------------------------------------


// textos do briefing aprovados pela dupla em 04/11; nada alem da spec (#103)
static const char* briefing_titles[BRIEFING_CARDS] = {
    "A coloração de Gram",
    "Duas camadas externas",
    "As armas e a célula própria"
};

static const char* briefing_lines[BRIEFING_CARDS][4] = {
    { "A coloração separa as bactérias em dois grupos.",
      "O resultado é roxo (Gram+) ou vermelho (Gram-).",
      "A cor indica o grupo, não se a bactéria causa doença.",
      NULL },
    { "Gram+: parede espessa, rica em peptidoglicano.",
      "Gram-: parede fina e uma membrana externa.",
      "O peptidoglicano faz parte da parede da bactéria.",
      NULL },
    { "Ruptura age na parede; Dissolução, na camada extra.",
      "As duas armas ferem a célula própria: mire longe dela.",
      "Nem toda bactéria causa doença; neste cenário,",
      "os agentes são tratados como ameaças do jogo." }
};

// briefing: tres cartoes pulaveis antes da fase (#103, #106)
void draw_briefing()
{
    char step[32];
    int count;
    int i;
    const UiButton* buttons = screen_buttons(&count);

    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 26, ALLEGRO_ALIGN_CENTRE,
        "Antes de entrar na fase");
    draw_rule((float)SCREEN_W / 2, 40.0f, 200.0f, UI_FRAME);

    snprintf(step, sizeof(step), "cartão %d de %d", briefing_card + 1, BRIEFING_CARDS);
    al_draw_text(font, UI_DIM, SCREEN_W / 2, 48, ALLEGRO_ALIGN_CENTRE, step);

    draw_panel(40.0f, 70.0f, (float)SCREEN_W - 80.0f, 116.0f, UI_INK, UI_FRAME);

    al_draw_text(font, UI_AMBER, 52.0f, 82.0f, 0, briefing_titles[briefing_card]);

    for (i = 0; i < BRIEFING_LINES; i++)
    {
        // cartao com menos linhas termina no NULL
        if (briefing_lines[briefing_card][i] == NULL)
        {
            continue;
        }

        al_draw_text(font, UI_TEXT, 52.0f, 106.0f + (float)i * 18.0f, 0,
            briefing_lines[briefing_card][i]);
    }

    draw_buttons(buttons, count);

    al_draw_text(font, UI_DIM, SCREEN_W / 2, 236, ALLEGRO_ALIGN_CENTRE,
        "enter ou clique avança · esc pula");
}

void draw_menu()
{
    char objective[96];
    char bonus[96];
    char record[32];
    int count;

    const UiButton* buttons = screen_buttons(&count);

    float panel_x = 40.0f;
    float panel_w = (float)SCREEN_W - 80.0f;
    float col_x = panel_x + 12.0f;

    /*
     Menu organizado para acomodar:
     - movimentacao;
     - combate;
     - equipamentos;
     - objetivo dinamico.
    */
    float controls_y = 66.0f;
    float controls_h = 136.0f;

    float goal_y = 208.0f;
    float goal_h = 48.0f;

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
       CONTROLES
       ========================================= */

    draw_panel(
        panel_x,
        controls_y,
        panel_w,
        controls_h,
        UI_INK,
        UI_FRAME
    );

    al_draw_text(
        font,
        UI_AMBER,
        col_x,
        controls_y + 8.0f,
        0,
        "Como jogar"
    );

    /* Movimentacao */

    al_draw_text(
        font,
        UI_AMBER,
        col_x,
        controls_y + 22.0f,
        0,
        "Movimentação"
    );

    al_draw_text(
        font,
        UI_TEXT,
        col_x + 120.0f,
        controls_y + 22.0f,
        0,
        "setas ou WASD mover · cursor mirar"
    );

    /* Combate */

    al_draw_text(
        font,
        UI_AMBER,
        col_x,
        controls_y + 38.0f,
        0,
        "Combate"
    );

    al_draw_text(
        font,
        UI_TEXT,
        col_x + 120.0f,
        controls_y + 38.0f,
        0,
        "botão esquerdo atirar · botão direito escanear"
    );

    /* Equipamentos */

    al_draw_text(
        font,
        UI_AMBER,
        col_x,
        controls_y + 54.0f,
        0,
        "Equipamentos"
    );

    al_draw_text(
        font,
        UI_TEXT,
        col_x + 120.0f,
        controls_y + 70.0f,
        0,
        "1: Ruptura - age na parede celular"
    );

    al_draw_text(
        font,
        UI_TEXT,
        col_x + 120.0f,
        controls_y + 86.0f,
        0,
        "2: Dissolução - age na camada externa"
    );

    al_draw_text(
        font,
        UI_DIM,
        col_x,
        controls_y + 102.0f,
        0,
        "1, 2, 3 ou roda: trocar de arma"
    );

    al_draw_text(
        font,
        UI_DIM,
        col_x,
        controls_y + 118.0f,
        0,
        "interprete as informações do scanner antes de atacar"
    );

    /* =========================================
       OBJETIVO DA MISSAO
       ========================================= */

    draw_panel(
        panel_x,
        goal_y,
        panel_w,
        goal_h,
        UI_INK,
        UI_FRAME
    );

    al_draw_text(
        font,
        UI_AMBER,
        col_x,
        goal_y + 8.0f,
        0,
        "Objetivo da missão"
    );

    snprintf(
        objective,
        sizeof(objective),
        "neutralizar as %d bactérias e chegar ao destino",
        threats_total()
    );

    al_draw_text(
        font,
        UI_TEXT,
        col_x,
        goal_y + 22.0f,
        0,
        objective
    );

    snprintf(
        bonus,
        sizeof(bonus),
        "identificar os %d agentes vale +%d pontos",
        agents_initial(),
        IDENTIFICATION_BONUS
    );

    al_draw_text(
        font,
        UI_DIM,
        col_x,
        goal_y + 34.0f,
        0,
        bonus
    );

    /* =========================================
       BOTAO E INSTRUCOES
       ========================================= */

    draw_buttons(buttons, count);

    al_draw_text(
        font,
        UI_DIM,
        SCREEN_W / 2,
        BUTTON_Y + 36,
        ALLEGRO_ALIGN_CENTRE,
        "enter ou clique para começar"
    );

    // recorde lido do arquivo no inicio do jogo (#M4)
    snprintf(record, sizeof(record), "recorde %d", hi_score());
    al_draw_text(font, UI_DIM, SCREEN_W / 2, BUTTON_Y + 52, ALLEGRO_ALIGN_CENTRE, record);

    al_draw_text(
        font,
        UI_DIM,
        SCREEN_W - HUD_MARGIN,
        336,
        ALLEGRO_ALIGN_RIGHT,
        "F11 tela cheia"
    );
}


// sprite de cada especie e variante, alinhado com as constantes do entities.h
// variante 1 usa o segundo desenho da especie; qualquer outra, o primeiro (#211)
ALLEGRO_BITMAP* species_sprite(Species species, int variant)
{
    if (species == SPECIES_GRAM_POSITIVE)
    {
        return variant == 1 ? gram_positive_b : gram_positive;
    }

    if (species == SPECIES_GRAM_NEGATIVE)
    {
        return variant == 1 ? gram_negative_b : gram_negative;
    }

    if (species == SPECIES_VIRUS)
    {
        return variant == 1 ? virus_sprite_b : virus_sprite;
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

// quando o destino esta fora da faixa visivel, uma marca na borda mostra o caminho (#19)
void draw_destination_hint()
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

    if (cy < margin_y)
    {
        cy = margin_y;
    }

    if (cy > (float)PLAY_H - margin_y)
    {
        cy = (float)PLAY_H - margin_y;
    }

    al_draw_filled_circle(cx, cy, 6.0f, UI_AMBER);

    // na metade de baixo o rotulo sobe, para nao encostar na linha do painel
    if (cy > (float)PLAY_H / 2.0f)
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
#define FEEL_IMPACT_TIME 0.12f
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
            al_map_rgba_f(0.49f * a, 0.98f * a, 1.0f * a, a), 2.0f);
    }

    if (feel_impact_time > 0.0f)
    {
        float a = feel_impact_time / FEEL_IMPACT_TIME;
        float r = (float)AGENT_RADIUS + 2.0f + (1.0f - a) * 8.0f;
        ALLEGRO_COLOR c = feel_impact_dull
            ? al_map_rgba_f(0.45f * a, 0.5f * a, 0.55f * a, a)
            : al_map_rgba_f(a, a, a, a);

        al_draw_circle(view_x(feel_impact_x), view_y(feel_impact_y), r, c, 2.0f);
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
            al_map_rgba_f(1.0f * a, 0.91f * a, 0.58f * a, a));
    }

    if (feel_pop_time > 0.0f)
    {
        float a = feel_pop_time / FEEL_POP_TIME;
        float rise = (1.0f - a) * 24.0f;

        snprintf(pop, sizeof(pop), "+%d", SCORE_PER_THREAT);
        al_draw_text(font, al_map_rgba_f(0.43f * a, 0.86f * a, 0.47f * a, a),
            view_x(feel_pop_x), view_y(feel_pop_y) - (float)AGENT_RADIUS - 12.0f - rise,
            ALLEGRO_ALIGN_CENTRE, pop);
    }
}

// icone provisorio da Reserva no HUD: capsula de 16x16 em duas metades (#216)
void draw_reserve_icon(float x, float y)
{
    al_draw_filled_rounded_rectangle(x + 1.0f, y + 5.0f, x + 15.0f, y + 11.0f,
        3.0f, 3.0f, UI_AMBER);
    al_draw_filled_rectangle(x + 8.0f, y + 5.0f, x + 12.0f, y + 11.0f, UI_TEXT);
    al_draw_filled_rounded_rectangle(x + 8.0f, y + 5.0f, x + 15.0f, y + 11.0f,
        3.0f, 3.0f, UI_TEXT);
    al_draw_rounded_rectangle(x + 1.5f, y + 5.5f, x + 14.5f, y + 10.5f,
        3.0f, 3.0f, UI_INK, 1.0f);
}

void draw_hud()
{
    int x;
    float bar_x;
    float fill;
    ALLEGRO_COLOR vitals_color;
    int heart_y;
    int total_seconds;
    int x_score;
    int x_time;
    int x_weapon;
    int x_icon;
    int x_bar;
    int x_word;
    char line[96];
    char score_text[32];
    char timer_text[16];
    const char* state_text;
    const char* module_name;
    char reserve_text[24];
    ALLEGRO_BITMAP* laser_icon;

    al_draw_filled_rectangle(0.0f, (float)SCAN_PANEL_Y, (float)SCREEN_W, (float)SCREEN_H, UI_INK);
    al_draw_line(0.0f, (float)SCAN_PANEL_Y - 0.5f, (float)SCREEN_W,
        (float)SCAN_PANEL_Y - 0.5f, UI_LINE, 1.0f);

    // nada do HUD pode vazar para a faixa do mundo; o clip vale so no painel
    al_set_clipping_rectangle(0, SCAN_PANEL_Y, SCREEN_W, PANEL_H);

    // coracao centrado na altura do texto, nao topo a topo
    heart_y = hud_y(0) + (al_get_font_line_height(font) - al_get_bitmap_height(heart_icon)) / 2;

    // sinais vitais: coracao, barra com a cor da faixa e o numero (#218)
    if (player.health >= VITALS_MIN_BETWEEN)
    {
        vitals_color = UI_GREEN;
    }
    else if (player.health >= VITALS_LOW)
    {
        vitals_color = UI_AMBER;
    }
    else
    {
        vitals_color = UI_RED;
    }

    al_draw_bitmap(heart_icon, (float)HUD_MARGIN, (float)heart_y, 0);
    bar_x = (float)(HUD_MARGIN + al_get_bitmap_width(heart_icon) + 4);
    fill = (float)VITALS_BAR_W * player.health / PLAYER_MAX_HEALTH;
    al_draw_filled_rectangle(bar_x, (float)(hud_y(0) + 1), bar_x + (float)VITALS_BAR_W,
        (float)(hud_y(0) + 1 + UI_SEG_H), UI_SLOT);
    al_draw_filled_rectangle(bar_x, (float)(hud_y(0) + 1), bar_x + fill,
        (float)(hud_y(0) + 1 + UI_SEG_H), vitals_color);

    // arredonda para cima: o numero so mostra 0 quando os vitais zeraram
    snprintf(line, sizeof(line), "%d", (int)ceilf(player.health));
    al_draw_text(font, vitals_color, bar_x + (float)VITALS_BAR_W + 4.0f, (float)hud_y(0), 0, line);

    x = (int)bar_x + VITALS_BAR_W + 4 + 3 * 8 + UI_GAP;

    // ameacas com contador: seis blocos de 6 px nao dizem quantas restam
    snprintf(line, sizeof(line), "ameaças %d/%d", threats_total() - threats_left(), threats_total());
    al_draw_text(font, UI_DIM, (float)x, (float)hud_y(0), 0, line);

    // lado direito: pontos, tempo e o raio com a recarga, tudo medido da borda
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

    laser_icon = player.laser == LASER_GRAM_POSITIVE ? ruptura_icon : dissolucao_icon;

    // o HUD diz qual arma esta ativa pelo nome, nao so a cor (#199)
    module_name = player.laser == LASER_GRAM_POSITIVE ? "Ruptura" : "Dissolução";

    // Reserva: nome com as cargas e a palavra de estado quando acabam (#216)
    if (player.laser == LASER_RESERVE)
    {
        snprintf(reserve_text, sizeof(reserve_text), "Reserva %d/%d",
            reserve_charges_left(), RESERVE_CHARGES);
        module_name = reserve_text;

        if (reserve_charges_left() <= 0 && defense_state() != 1)
        {
            state_text = "esgotada";
        }
    }

    total_seconds = (int)match_time;
    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", total_seconds / 60, total_seconds % 60);
    snprintf(score_text, sizeof(score_text), "pontos %d", score);

    x_word = SCREEN_W - HUD_MARGIN - al_get_text_width(font, state_text);
    x_bar = x_word - 6 - UI_BAR_W;
    x_icon = x_bar - 4 - al_get_bitmap_width(laser_icon);
    x_weapon = x_icon - 4 - al_get_text_width(font, module_name);
    x_time = x_weapon - UI_GAP - al_get_text_width(font, timer_text);
    x_score = x_time - UI_GAP - al_get_text_width(font, score_text);

    al_draw_text(font, UI_TEXT, (float)x_score, (float)hud_y(0), 0, score_text);
    al_draw_text(font, UI_DIM, (float)x_time, (float)hud_y(0), 0, timer_text);
    al_draw_text(font, UI_TEXT, (float)x_weapon, (float)hud_y(0), 0, module_name);

    // a Reserva ainda nao tem icone proprio (Etapa 3): uma capsula provisoria
    if (player.laser == LASER_RESERVE)
    {
        draw_reserve_icon((float)x_icon, (float)(hud_y(0) - 4));
    }
    else
    {
        al_draw_bitmap(laser_icon, (float)x_icon, (float)(hud_y(0) - 4), 0);
    }
    draw_cooldown_bar((float)x_bar, (float)(hud_y(0) + 1),
        fire_locked() ? fire_lock_left() : attack_cooldown_left());
    al_draw_text(font, UI_DIM, (float)x_word, (float)hud_y(0), 0, state_text);

    if (threats_left() > 0)
    {
        // a Fase 2 tem vírus, não bactéria: a frase acompanha a fase (#M4)
        if (current_phase() == 1)
        {
            al_draw_text(font, UI_AMBER, HUD_MARGIN, hud_y(1), 0,
                "objetivo: neutralizar as bactérias e chegar ao destino");
        }
        else
        {
            al_draw_text(font, UI_AMBER, HUD_MARGIN, hud_y(1), 0,
                "objetivo: neutralizar os vírus e chegar ao destino");
        }
    }
    else
    {
        al_draw_text(font, UI_AMBER, HUD_MARGIN, hud_y(1), 0, "objetivo: vá ao destino");
    }

    // identificados fica na linha do objetivo: e progresso do bonus, nao do combate
    snprintf(line, sizeof(line), "identificados %d/%d", identified_count, agents_initial());
    al_draw_text(font, UI_DIM, (float)(SCREEN_W - HUD_MARGIN), (float)hud_y(1),
        ALLEGRO_ALIGN_RIGHT, line);

    draw_scan_panel();

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

        al_draw_arc(x, y, (float)AGENT_RADIUS + 5.0f, -ALLEGRO_PI / 2.0f,
            left * 2.0f * ALLEGRO_PI, UI_AMBER, 2.0f);
    }

    if (agents[index].resistant)
    {
        al_draw_circle(x, y, (float)AGENT_RADIUS + 5.0f, UI_RED, 2.0f);
        al_draw_text(font, UI_RED, x, y - (float)AGENT_RADIUS - 16.0f,
            ALLEGRO_ALIGN_CENTRE, "R");
    }
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

    // o mundo para na faixa jogavel: sprite nenhum entra no painel
    al_set_clipping_rectangle(0, 0, SCREEN_W, PLAY_H);

    // tremor so no desenho do mundo: a camera e a simulacao nao mudam (#221)
    al_identity_transform(&shake);
    al_translate_transform(&shake, feel_offset_x(), feel_offset_y());
    al_use_transform(&shake);

    // mapa do mundo deslocado pela camera, atras de destino, agentes e nanobo
    // recuado: escurecido e com um veu azulado, sem mudar o PNG (#220)
    al_draw_tinted_bitmap(world_map, MAP_TINT, -floorf(camera.x), -floorf(camera.y), 0);
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)PLAY_H, MAP_VEIL);

    // destino da fase: anel e rotulo visiveis desde o inicio (#26)
    al_draw_filled_circle(view_x(DESTINATION_X), view_y(DESTINATION_Y), DESTINATION_RADIUS,
        al_map_rgb(40, 34, 18));
    al_draw_circle(view_x(DESTINATION_X), view_y(DESTINATION_Y), DESTINATION_RADIUS,
        UI_AMBER, 3);
    al_draw_text(font, UI_AMBER, view_x(DESTINATION_X),
        view_y(DESTINATION_Y) - 40, ALLEGRO_ALIGN_CENTRE, "destino");

    for (i = 0; i < agents_total(); i++)
    {
        // agente neutralizado sai do desenho
        if (!agents[i].active)
        {
            continue;
        }

        is_bacteria = agents[i].species == SPECIES_GRAM_POSITIVE
            || agents[i].species == SPECIES_GRAM_NEGATIVE;

        // sombra de contato: o agente deixa de parecer colado no fundo (#220)
        al_draw_filled_ellipse(view_x(agents[i].x), view_y(agents[i].y) + 12.0f,
            12.0f, 4.0f, CONTACT_SHADOW);

        // bacteria sem leitura: silhueta azulada, sem forma nem cor que entregue o grupo (#220)
        // o virus fica de fora: o estado dele e lido no mundo, sem scanner (#84)
        if (is_bacteria && agents[i].layers == 0)
        {
            al_draw_filled_circle(view_x(agents[i].x), view_y(agents[i].y),
                (float)AGENT_RADIUS - 3.0f, SILHOUETTE_FILL);
            al_draw_circle(view_x(agents[i].x), view_y(agents[i].y),
                (float)AGENT_RADIUS - 3.0f, SILHOUETTE_EDGE, 1.0f);
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

        // virus lisogenico pisca: opaco e semitransparente a cada LYSOGENIC_BLINK (#84)
        if (virus_is_lysogenic(i) && (int)(match_time / LYSOGENIC_BLINK) % 2 == 1)
        {
            al_draw_tinted_bitmap(species_sprite(agents[i].species, agents[i].variant),
                al_map_rgba_f(0.35f, 0.35f, 0.35f, 0.35f),
                view_x(agents[i].x) - AGENT_RADIUS,
                view_y(agents[i].y) - AGENT_RADIUS, 0);
            continue;
        }

        al_draw_bitmap(species_sprite(agents[i].species, agents[i].variant),
            view_x(agents[i].x) - AGENT_RADIUS,
            view_y(agents[i].y) - AGENT_RADIUS, 0);
    }

    // dica do tutorial presa ao agente: some no primeiro scan, volta no tiro sem leitura (#219)
    if (tutorial_hint_target() != -1)
    {
        const char* hint = "botão direito: escanear";
        int hint_w = al_get_text_width(font, hint);
        float hx = floorf(view_x(agents[tutorial_hint_target()].x) - (float)hint_w / 2.0f);
        float hy = view_y(agents[tutorial_hint_target()].y) - (float)AGENT_RADIUS - 18.0f;

        al_draw_filled_rectangle(hx - 4.0f, hy - 3.0f, hx + (float)hint_w + 4.0f, hy + 11.0f, UI_INK);
        al_draw_rectangle(hx - 3.5f, hy - 2.5f, hx + (float)hint_w + 3.5f, hy + 10.5f, UI_CYAN, 1.0f);
        al_draw_text(font, UI_CYAN, hx, hy, 0, hint);
    }

    // a sombra do nanobo segue a mesma regra dos agentes (#220)
    al_draw_filled_ellipse(view_x(player.x), view_y(player.y) + 13.0f,
        11.0f, 4.0f, CONTACT_SHADOW);

    // o nanobo e desenhado onde o mundo deixou ele, ja descontada a camera
    // a linha do spritesheet e a direcao quando a arte tem as quatro (#30)
    int nanobo_row = (al_get_bitmap_height(nanobo) >= 4 * 32) ? (int)player.facing : 0;

    // o nanobo pisca enquanto a invulnerabilidade dura (#221)
    if (invulnerability_left() > 0.0f && (int)(invulnerability_left() * 12.0f) % 2 == 1)
    {
        nanobo_tint = al_map_rgba_f(0.3f, 0.3f, 0.3f, 0.3f);
    }

    al_draw_tinted_bitmap_region(nanobo, nanobo_tint, (float)(player_frame() * 32),
        (float)(nanobo_row * 32), 32.0f, 32.0f,
        view_x(player.x) - PLAYER_RADIUS, view_y(player.y) - PLAYER_RADIUS, 0);

    // anel da defesa: ciano enquanto protege, apagado durante a recarga (#17)
    if (defense_state() == 1)
    {
        al_draw_circle(view_x(player.x), view_y(player.y), PLAYER_RADIUS + 2, UI_CYAN, 2);
    }
    else if (defense_state() == 2)
    {
        al_draw_circle(view_x(player.x), view_y(player.y), PLAYER_RADIUS + 2,
            UI_FRAME, 2);
    }

    // projetil de cor neutra: Ruptura e quadrado, Dissolucao e circulo (RF-04)
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].active)
        {
            continue;
        }

        px = view_x(projectiles[i].x);
        py = view_y(projectiles[i].y);

        if (projectiles[i].laser == LASER_GRAM_POSITIVE)
        {
            al_draw_filled_rectangle(px - (float)PROJECTILE_RADIUS,
                py - (float)PROJECTILE_RADIUS,
                px + (float)PROJECTILE_RADIUS,
                py + (float)PROJECTILE_RADIUS, UI_TEXT);
        }
        else if (projectiles[i].laser == LASER_RESERVE)
        {
            // Reserva: losango, a terceira forma neutra (#216)
            float r = (float)PROJECTILE_RADIUS + 1.0f;

            al_draw_filled_triangle(px - r, py, px, py - r, px + r, py, UI_TEXT);
            al_draw_filled_triangle(px - r, py, px, py + r, px + r, py, UI_TEXT);
        }
        else
        {
            al_draw_filled_circle(px, py, (float)PROJECTILE_RADIUS, UI_TEXT);
        }
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

    // feedback educacional curto da ultima decisao (#178), com placa de fundo
    message = feedback_text();

    if (message != NULL)
    {
        int text_w = al_get_text_width(font, message);
        float box_x = floorf(((float)SCREEN_W - (float)text_w) / 2.0f);
        float box_y = (float)(PLAY_H - 30);

        al_draw_filled_rectangle(box_x - UI_PAD, box_y - 4.0f, box_x + (float)text_w + UI_PAD,
            box_y + 12.0f, UI_INK);
        al_draw_rectangle(box_x - UI_PAD + 0.5f, box_y - 4.0f + 0.5f,
            box_x + (float)text_w + UI_PAD - 0.5f, box_y + 12.0f - 0.5f,
            al_map_rgb(120, 100, 60), 1.0f);
        al_draw_text(font, UI_WARM, box_x, box_y, 0, message);
    }

    // marca o destino quando ele esta fora da faixa visivel (#19)
    draw_destination_hint();

    // mira livre: o cursor do sistema some na fase e vira esta marca (#198)
    aim_x = to_logical_x(mouse_x);
    aim_y = to_logical_y(mouse_y);

    if (game_get_scene() == GAME_SCENE_GAME && aim_x >= 0.0f && aim_x < (float)SCREEN_W
        && aim_y >= 0.0f && aim_y < (float)PLAY_H)
    {
        al_draw_circle(aim_x, aim_y, 5.0f, UI_CYAN, 1.0f);
        al_draw_line(aim_x - 9.0f, aim_y + 0.5f, aim_x - 3.0f, aim_y + 0.5f, UI_CYAN, 1.0f);
        al_draw_line(aim_x + 3.0f, aim_y + 0.5f, aim_x + 9.0f, aim_y + 0.5f, UI_CYAN, 1.0f);
        al_draw_line(aim_x + 0.5f, aim_y - 9.0f, aim_x + 0.5f, aim_y - 3.0f, UI_CYAN, 1.0f);
        al_draw_line(aim_x + 0.5f, aim_y + 3.0f, aim_x + 0.5f, aim_y + 9.0f, UI_CYAN, 1.0f);
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
                agents[i].id, virus_is_lysogenic(i) ? "L" : "R",
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
    int count;
    const UiButton* buttons = screen_buttons(&count);

    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H,
        al_map_rgba(0, 0, 0, 180));
    draw_panel(190.0f, 130.0f, 260.0f, 100.0f, UI_INK, UI_FRAME);
    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 142, ALLEGRO_ALIGN_CENTRE, "PAUSE");
    draw_buttons(buttons, count);
    al_draw_text(font, UI_DIM, SCREEN_W / 2, 208, ALLEGRO_ALIGN_CENTRE, "esc para voltar");
}

// ---------------------------------------------------------------
// fim de fase: resultados, vitoria e game over
// ---------------------------------------------------------------


void results_oa1_text(char* line, size_t size)
{
    // Verifica se o objetivo de identificação de Gram se aplica à fase atual.
    if (!phase_has_bacteria())
    {
        snprintf(line, size,
            "OA1 Gram: nao aplicavel nesta fase.");
        return;
    }

    if (progress.scans_gram_positive > 0 &&
        progress.scans_gram_negative > 0)
    {
        snprintf(line, size,
            "OA1 Gram: identificou os dois grupos.");
    }
    else if (progress.scans_gram_positive > 0)
    {
        snprintf(line, size,
            "OA1 Gram: identificou Gram+; faltou Gram-.");
    }
    else if (progress.scans_gram_negative > 0)
    {
        snprintf(line, size,
            "OA1 Gram: identificou Gram-; faltou Gram+.");
    }
    else
    {
        snprintf(line, size,
            "OA1 Gram: nenhuma bactéria identificada.");
    }
}

// Exibe o resultado da identificação dos grupos para o objetivo de comparação estrutural.
void results_oa2_text(char* line, size_t size)
{
    // Verifica se existem bactérias na fase atual.
    if (!phase_has_bacteria())
    {
        snprintf(line, size,
            "OA2 Estrutura: nao aplicavel nesta fase.");
        return;
    }

    if (progress.scans_gram_positive > 0 &&
        progress.scans_gram_negative > 0)
    {
        snprintf(line, size,
            "OA2: dados de Gram+ e Gram-.");
    }
    else if (progress.scans_gram_positive > 0)
    {
        snprintf(line, size,
            "OA2 Estrutura: dados parciais (Gram+).");
    }
    else if (progress.scans_gram_negative > 0)
    {
        snprintf(line, size,
            "OA2 Estrutura: dados parciais (Gram-).");
    }
    else
    {
        snprintf(line, size,
            "OA2 Estrutura: sem dados para comparação.");
    }
}

void results_oa3_text(char* line, size_t size)
{
    if (progress.wrong_decisions == 0 &&
        progress.correct_decisions > 0)
    {
        snprintf(line, size,
            "OA3 Armas: %d acertos, nenhum erro.",
            progress.correct_decisions);
    }
    else if (progress.wrong_decisions > 0)
    {
        snprintf(line, size,
            "OA3 Armas: %d acertos, %d erros.",
            progress.correct_decisions,
            progress.wrong_decisions);
    }
    else
    {
        snprintf(line, size,
            "OA3 Armas: sem acertos registrados.");
    }
}

void results_oa4_text(char* line, size_t size)
{
    if (progress.cell_attacks == 0)
    {
        snprintf(line, size,
            "OA4 Células: nenhum ataque registrado.");
    }
    else
    {
        snprintf(line, size,
            "OA4 Células: %d ataques; revise a proteção.",
            progress.cell_attacks);
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
    ALLEGRO_COLOR bonus_color;

    snprintf(line, sizeof(line), "Fase %d concluída", current_phase());
    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 24, ALLEGRO_ALIGN_CENTRE, line);
    draw_rule((float)SCREEN_W / 2, 38.0f, 200.0f, UI_FRAME);
    al_draw_text(font, UI_DIM, SCREEN_W / 2, 46, ALLEGRO_ALIGN_CENTRE, "resumo da fase");

    // resultado principal: pontos e ameacas, o que fecha a fase
    draw_panel(140.0f, 58.0f, 360.0f, 44.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d", score);
    draw_stat(left, 66.0f, right, "pontos", value, UI_AMBER);

    snprintf(value, sizeof(value), "%d/%d", neutralized, threats_total());
    draw_stat(left, 82.0f, right, "ameaças neutralizadas", value, UI_TEXT);

    // objetivos secundarios: identificacao e bonus
    draw_panel(140.0f, 106.0f, 360.0f, 52.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d/%d", identified_count, agents_initial());
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
    draw_stat(left, 146.0f, right, "bônus de vitais", value,
        vitals_bonus_awarded() > 0 ? UI_GREEN : UI_DIM);

    // resumo educativo: so contadores da partida atual (#174, #202)
    draw_panel(140.0f, 162.0f, 360.0f, 92.0f, UI_INK, UI_FRAME);

    al_draw_text(font, UI_AMBER, left, 170.0f, 0, "o que a partida mostrou");


    row = 186.0f;

    // OA1: identificacao dos grupos pela coloracao de Gram.
    results_oa1_text(line, sizeof(line));
    al_draw_text(font, UI_TEXT, left, row, 0, line);
    row = row + 16.0f;

    // OA2: dados dos grupos disponiveis para comparacao estrutural.
    results_oa2_text(line, sizeof(line));
    al_draw_text(font, UI_TEXT, left, row, 0, line);
    row = row + 16.0f;

    // OA3: registro de acertos e erros nas armas utilizadas.
    results_oa3_text(line, sizeof(line));
    al_draw_text(font, UI_TEXT, left, row, 0, line);
    row = row + 16.0f;

    // OA4: registro de ataques que atingiram celulas proprias.
    results_oa4_text(line, sizeof(line));
    al_draw_text(font, UI_TEXT, left, row, 0, line);


    draw_buttons(buttons, count);

    al_draw_text(font, UI_DIM, SCREEN_W / 2, 296, ALLEGRO_ALIGN_CENTRE,
        "enter também continua");

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

void draw_victory()
{
    char value[32];
    char timer_text[16];
    int total_seconds = (int)run_time;
    int count;
    const UiButton* buttons = screen_buttons(&count);
    float left = 152.0f;
    float right = (float)SCREEN_W - 152.0f;

    al_draw_text(font, UI_TEXT, SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTRE, "Vitória");
    draw_rule((float)SCREEN_W / 2, 44.0f, 200.0f, UI_FRAME);
    al_draw_text(font, UI_DIM, SCREEN_W / 2, 52, ALLEGRO_ALIGN_CENTRE,
        "você defendeu o organismo");

    draw_panel(140.0f, 72.0f, 360.0f, 116.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d", score);
    draw_stat(left, 82.0f, right, "pontuação final", value, UI_AMBER);

    snprintf(value, sizeof(value), "%d/%d", run_progress.neutralized, run_progress.threats);
    draw_stat(left, 102.0f, right, "ameaças neutralizadas", value, UI_TEXT);

    snprintf(value, sizeof(value), "%d/%d", run_progress.identified, run_progress.agents);
    draw_stat(left, 122.0f, right, "agentes identificados", value, UI_TEXT);

    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", total_seconds / 60,
        total_seconds % 60);
    draw_stat(left, 142.0f, right, "tempo total", timer_text, UI_TEXT);

    snprintf(value, sizeof(value), "%d", hi_score());
    draw_stat(left, 162.0f, right, "recorde", value, UI_AMBER);

    draw_buttons(buttons, count);

    al_draw_text(font, UI_DIM, SCREEN_W / 2, 256, ALLEGRO_ALIGN_CENTRE,
        "enter também começa de novo");
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

    al_draw_text(font, UI_RED, SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    draw_rule((float)SCREEN_W / 2, 44.0f, 200.0f, al_map_rgb(150, 60, 60));
    al_draw_text(font, UI_DIM, SCREEN_W / 2, 52, ALLEGRO_ALIGN_CENTRE,
        "os sinais vitais do paciente zeraram");

    draw_panel(140.0f, 72.0f, 360.0f, 96.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d", score);
    draw_stat(left, 82.0f, right, "pontos", value, UI_TEXT);

    snprintf(value, sizeof(value), "%d/%d", run_progress.neutralized, run_progress.threats);
    draw_stat(left, 102.0f, right, "ameaças neutralizadas", value, UI_TEXT);

    snprintf(value, sizeof(value), "%d/%d", run_progress.identified, run_progress.agents);
    draw_stat(left, 122.0f, right, "agentes identificados", value, UI_TEXT);

    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", total_seconds / 60,
        total_seconds % 60);
    draw_stat(left, 142.0f, right, "tempo total", timer_text, UI_TEXT);

    draw_buttons(buttons, count);

    al_draw_text(font, UI_DIM, SCREEN_W / 2, 248, ALLEGRO_ALIGN_CENTRE,
        "enter também reinicia");
}

// cada estado desenha so o que e dele, tudo no canvas logico
void render()
{
    GameScene scene = game_get_scene();

    // trocou de tela, o foco volta para o primeiro botao
    if (scene != ui_last_scene)
    {
        ui_last_scene = scene;
        ui_focus = 0;

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

    // o canvas vai para a janela com escala inteira e barras pretas
    al_set_target_backbuffer(display);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(canvas, 0, 0, SCREEN_W, SCREEN_H,
        screen_off_x, screen_off_y,
        (float)SCREEN_W * screen_scale, (float)SCREEN_H * screen_scale, 0);

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

        // no briefing a seta avanca mesmo com o foco no PULAR (#103)
        if (scene == GAME_SCENE_BRIEFING && key == ALLEGRO_KEY_RIGHT)
        {
            activate_button(0);
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
    else if (key == ALLEGRO_KEY_1)
    {
        player.laser = LASER_GRAM_POSITIVE;
    }
    else if (key == ALLEGRO_KEY_2)
    {
        player.laser = LASER_GRAM_NEGATIVE;
    }
    else if (key == ALLEGRO_KEY_3 && reserve_available())
    {
        player.laser = LASER_RESERVE;
    }
#ifdef _DEBUG
    // teclas de QA: so existem no Debug, nunca no Release (#76)
    // F forca a condicao real de conclusao para testar a transicao da #70
    else if (key == ALLEGRO_KEY_F)
    {
        int i;

        for (i = 0; i < agents_total(); i++)
        {
            if (agents[i].species != SPECIES_CELL)
            {
                // dois acertos validos, o mesmo caminho da regra de combate (#49)
                hit_agent(i);
                hit_agent(i);
            }
        }

        player.x = DESTINATION_X;
        player.y = DESTINATION_Y;
    }
    // G forca Game Over; vida zerada continua sendo o caminho real
    else if (key == ALLEGRO_KEY_G)
    {
        game_set_scene(GAME_SCENE_GAME_OVER);
    }
    // L encurta o lisogenico de todo virus para 1 s, para testar o fim (#84)
    else if (key == ALLEGRO_KEY_L)
    {
        int i;

        for (i = 0; i < agents_total(); i++)
        {
            if (virus_is_lysogenic(i) && agents[i].lysogenic_timer > 1.0f)
            {
                agents[i].lysogenic_timer = 1.0f;
            }
        }
    }
    // J avanca uma camada do scan em todo agente, para conferir o desenho (#214)
    else if (key == ALLEGRO_KEY_J)
    {
        int i;

        for (i = 0; i < agents_total(); i++)
        {
            if (agents[i].active && agents[i].layers < MAX_LAYERS)
            {
                agents[i].layers = agents[i].layers + 1;

                if (agents[i].layers == MAX_LAYERS && !agents[i].identified)
                {
                    agents[i].identified = true;

                    // so os agentes iniciais contam, como no scan real (#217)
                    if (i < agents_initial())
                    {
                        identified_count = identified_count + 1;
                    }
                }
            }
        }
    }
    // H identifica os 9 e conclui, para testar o caminho do bonus (#67)
    else if (key == ALLEGRO_KEY_H)
    {
        int i;

        for (i = 0; i < agents_total(); i++)
        {
            agents[i].layers = MAX_LAYERS;
            agents[i].identified = true;
        }

        identified_count = agents_initial();

        for (i = 0; i < agents_total(); i++)
        {
            if (agents[i].species != SPECIES_CELL)
            {
                hit_agent(i);
                hit_agent(i);
            }
        }

        player.x = DESTINATION_X;
        player.y = DESTINATION_Y;
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
    bool want = audio_ready && music != NULL && game_get_scene() != GAME_SCENE_PAUSE;

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

    // o recorde é lido uma vez, antes do primeiro desenho do menu (#M4)
    load_hi_score();

    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;

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

                    if (player.health <= 0.0f)
                    {
                        // game over nao grava recorde: so a vitoria entra (#34/#82)
                        game_set_scene(GAME_SCENE_GAME_OVER);
                    }
                    // fase cumprida entra em Resultados sozinha (#70)
                    else if (phase_completed())
                    {
                        game_set_scene(GAME_SCENE_RESULTS);
                    }
                }
                feel_trigger();

                // na pausa os efeitos ficam parados junto com o jogo
                if (game_get_scene() != GAME_SCENE_PAUSE)
                {
                    feel_update((float)deltaTime);
                }
                play_pending_sfx();
                sync_music();

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                handle_key_down(event.keyboard.keycode);
                break;

            case ALLEGRO_EVENT_KEY_UP:
                handle_key_up(event.keyboard.keycode);
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                handle_mouse_click(event.mouse.button, event.mouse.x, event.mouse.y);
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                // hover: guarda o pixel da janela, quem desenha converte
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;

                // roda do mouse troca o modulo dentro da fase (#198)
                if (event.mouse.dz != 0 && game_get_scene() == GAME_SCENE_GAME)
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