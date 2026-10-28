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

#include "core/game.h"
#include "entities/entities.h"
#include "gameplay/gameplay.h"

#define BUTTON_X 220
#define BUTTON_Y 200
#define BUTTON_W 200
#define BUTTON_H 50

// medidas do HUD: tudo derivado do painel, nunca coordenada magica
#define HUD_MARGIN 10
#define HUD_LINES 5
#define HUD_LINE_H ((PANEL_H - 2 * HUD_MARGIN) / HUD_LINES)

// a fonte embutida tem 8 px: menos de 12 por linha encosta uma na outra
_Static_assert(HUD_LINE_H >= 12, "linha do HUD apertada demais para a fonte");
_Static_assert(HUD_LINES * HUD_LINE_H + 2 * HUD_MARGIN <= PANEL_H, "HUD nao cabe no painel");

// y de cada linha do HUD, contando de cima para baixo dentro do painel
int hud_y(int line)
{
    return SCAN_PANEL_Y + HUD_MARGIN + line * HUD_LINE_H;
}

// recursos do jogo, carregados uma vez antes do loop
ALLEGRO_FONT* font = NULL;
ALLEGRO_BITMAP* nanobo = NULL;

// coracao do HUD de vida (#19)
ALLEGRO_BITMAP* heart_icon = NULL;

// a arte de cada especie da Fase 1
ALLEGRO_BITMAP* gram_positive = NULL;
ALLEGRO_BITMAP* gram_negative = NULL;
ALLEGRO_BITMAP* cell_sprite = NULL;

// janela, canvas logico e a escala inteira aplicada na saida
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_BITMAP* canvas = NULL;
float screen_scale = 1.0f;
float screen_off_x = 0.0f;
float screen_off_y = 0.0f;

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

    // escala quebrada so quando a janela e menor que a tela logica
    if (fit >= 1.0f)
    {
        fit = floorf(fit);
    }

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

    nanobo = al_load_bitmap("assets/sprites/nanobo.png");
    if (!nanobo)
    {
        printf("couldn't load nanobo\n");
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

    heart_icon = al_load_bitmap("assets/sprites/icone_coracao.png");
    if (!heart_icon)
    {
        printf("couldn't load heart icon\n");
        return false;
    }

    return true;
}

// libera o que foi carregado, na ordem inversa
void free_resources()
{
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

    if (heart_icon)
    {
        al_destroy_bitmap(heart_icon);
        heart_icon = NULL;
    }

    if (nanobo)
    {
        al_destroy_bitmap(nanobo);
        nanobo = NULL;
    }

    if (font)
    {
        al_destroy_font(font);
        font = NULL;
    }
}

// leitura do ultimo alvo escaneado; apertar space de novo rele o alvo
void draw_scan_info()
{
    int target = last_scan_target();
    char line[128];

    if (target == -1)
    {
        al_draw_text(font, al_map_rgb(130, 130, 130), HUD_MARGIN, hud_y(3), 0,
            "alvo: nenhum (chegue perto e aperte space)");
        return;
    }

    if (!agents[target].active)
    {
        snprintf(line, sizeof(line), "agente %d: neutralizado", agents[target].id);
        al_draw_text(font, al_map_rgb(130, 130, 130), HUD_MARGIN, hud_y(3), 0, line);
        return;
    }

    snprintf(line, sizeof(line), "agente %d: %s", agents[target].id,
        layer_one_text(agents[target].species));
    al_draw_text(font, al_map_rgb(170, 170, 170), HUD_MARGIN, hud_y(3), 0, line);

    if (agents[target].layers >= MAX_LAYERS)
    {
        // a camada 2 e a linha mais longa do painel: o clip do draw_hud corta o excedente
        snprintf(line, sizeof(line), "  %s", layer_two_text(agents[target].species));
        al_draw_text(font, al_map_rgb(220, 220, 220), HUD_MARGIN, hud_y(4), 0, line);
    }
}

void draw_menu()
{
    char objective[96];

    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 56, ALLEGRO_ALIGN_CENTRE, "NANOBO");
    al_draw_text(font, al_map_rgb(170, 170, 170), SCREEN_W / 2, 82, ALLEGRO_ALIGN_CENTRE, "defenda o organismo");

    al_draw_rectangle(BUTTON_X, BUTTON_Y, BUTTON_X + BUTTON_W, BUTTON_Y + BUTTON_H, al_map_rgb(255, 255, 255), 1);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, BUTTON_Y + 20, ALLEGRO_ALIGN_CENTRE, "INICIAR");

    al_draw_text(font, al_map_rgb(130, 130, 130), SCREEN_W / 2, BUTTON_Y + 64, ALLEGRO_ALIGN_CENTRE,
        "clique no botao ou aperte enter");

    // instrucao minima do MVP: objetivo, controles e a regra que liga pista a laser (#58)
    snprintf(objective, sizeof(objective), "objetivo: neutralize as %d bacterias e alcance o destino", threats_total());
    al_draw_text(font, al_map_rgb(240, 190, 70), SCREEN_W / 2, 286, ALLEGRO_ALIGN_CENTRE, objective);
    al_draw_text(font, al_map_rgb(170, 170, 170), SCREEN_W / 2, 304, ALLEGRO_ALIGN_CENTRE,
        "space le o scanner   j atira   1/2 escolhe o raio   x defende");
    al_draw_text(font, al_map_rgb(170, 170, 170), SCREEN_W / 2, 320, ALLEGRO_ALIGN_CENTRE,
        "parede espessa, sem membrana externa = Gram+ = raio roxo");
    al_draw_text(font, al_map_rgb(170, 170, 170), SCREEN_W / 2, 334, ALLEGRO_ALIGN_CENTRE,
        "parede fina sob membrana externa = Gram- = raio vermelho");
}

// sprite de cada especie, alinhado com as constantes do entities.h
ALLEGRO_BITMAP* species_sprite(Species species)
{
    if (species == SPECIES_GRAM_POSITIVE)
    {
        return gram_positive;
    }

    if (species == SPECIES_GRAM_NEGATIVE)
    {
        return gram_negative;
    }

    return cell_sprite;
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
    float margin_x = 34.0f;
    float margin_y = 18.0f;
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

    al_draw_filled_circle(cx, cy, 6.0f, al_map_rgb(240, 190, 70));
    al_draw_text(font, al_map_rgb(240, 190, 70), cx, cy + 10.0f, ALLEGRO_ALIGN_CENTRE, "destino");
}

// status e leitura do scanner no rodape: a faixa do mundo fica limpa
void draw_hud()
{
    int i;
    int contact;
    int hearts_x;
    char status[96];
    char laser_text[64];
    char contact_text[64];

    al_draw_line(0, SCAN_PANEL_Y - 1, SCREEN_W, SCAN_PANEL_Y - 1, al_map_rgb(60, 60, 60), 1);

    // nada do HUD pode vazar para a faixa do mundo; o clip vale so no painel
    al_set_clipping_rectangle(0, SCAN_PANEL_Y, SCREEN_W, PANEL_H);

    // o status e medido na hora: os coracoes entram logo depois dele
    // temporaria: pontos e ameacas ate a #19/#172 cuidarem do HUD
    snprintf(status, sizeof(status), "identificados: %d/%d    restam: %d/%d    pontos: %d    tempo: %.1f",
        identified_count, NUM_AGENTES, threats_left(), threats_total(), score, match_time);
    al_draw_text(font, al_map_rgb(255, 255, 255), HUD_MARGIN, hud_y(0), 0, status);

    hearts_x = HUD_MARGIN + al_get_text_width(font, status) + 24;

    // vida em coracoes: tamanho e espacamento vem do bitmap, nao de literal (#19)
    for (i = 0; i < PLAYER_MAX_HEALTH; i++)
    {
        int hx = hearts_x + i * (al_get_bitmap_width(heart_icon) + 2);

        if (i < player.health)
        {
            al_draw_bitmap(heart_icon, hx, hud_y(0), 0);
        }
        else
        {
            al_draw_tinted_bitmap(heart_icon, al_map_rgba_f(0.25f, 0.25f, 0.25f, 1.0f),
                hx, hud_y(0), 0);
        }
    }

    if (threats_left() > 0)
    {
        al_draw_text(font, al_map_rgb(240, 190, 70), HUD_MARGIN, hud_y(1), 0,
            "objetivo: neutralize as bacterias e chegue ao destino");
    }
    else
    {
        al_draw_text(font, al_map_rgb(240, 190, 70), HUD_MARGIN, hud_y(1), 0,
            "objetivo: va ao destino");
    }

    snprintf(laser_text, sizeof(laser_text), "raio: %s (%s)%s",
        player.laser == LASER_GRAM_POSITIVE ? "roxo" : "vermelho",
        player.laser == LASER_GRAM_POSITIVE ? "Gram+" : "Gram-",
        attack_ready() ? "  [pronto]" : "  [recarga]");
    al_draw_text(font,
        player.laser == LASER_GRAM_POSITIVE ? al_map_rgb(190, 90, 255) : al_map_rgb(255, 70, 70),
        HUD_MARGIN, hud_y(2), 0, laser_text);

    // linha temporaria: prova visual do contato enquanto a #11 nao mostra o dano
    contact = touching_agent();

    if (contact != -1)
    {
        int w;

        snprintf(contact_text, sizeof(contact_text), "encostado no agente %d", agents[contact].id);
        w = al_get_text_width(font, contact_text);
        al_draw_text(font, al_map_rgb(240, 200, 90), SCREEN_W - HUD_MARGIN - w, hud_y(2), 0,
            contact_text);
    }

    draw_scan_info();

    // o quadro seguinte desenha o mundo no mesmo canvas: sem isso ele sai cortado
    al_reset_clipping_rectangle();
}

void draw_game()
{
    int i;
    const char* message;

    // o mundo para na faixa jogavel: sprite nenhum entra no painel
    al_set_clipping_rectangle(0, 0, SCREEN_W, PLAY_H);

    // destino da fase: anel e rotulo visiveis desde o inicio (#26)
    al_draw_filled_circle(view_x(DESTINATION_X), view_y(DESTINATION_Y), DESTINATION_RADIUS,
        al_map_rgb(40, 34, 18));
    al_draw_circle(view_x(DESTINATION_X), view_y(DESTINATION_Y), DESTINATION_RADIUS,
        al_map_rgb(240, 190, 70), 3);
    al_draw_text(font, al_map_rgb(240, 190, 70), view_x(DESTINATION_X),
        view_y(DESTINATION_Y) - 40, ALLEGRO_ALIGN_CENTRE, "destino");

    for (i = 0; i < NUM_AGENTES; i++)
    {
        // agente neutralizado sai do desenho
        if (!agents[i].active)
        {
            continue;
        }

        al_draw_bitmap(species_sprite(agents[i].species),
            view_x(agents[i].x) - AGENT_RADIUS,
            view_y(agents[i].y) - AGENT_RADIUS, 0);
    }

    // o nanobo e desenhado onde o mundo deixou ele, ja descontada a camera
    al_draw_bitmap(nanobo, view_x(player.x) - PLAYER_RADIUS,
        view_y(player.y) - PLAYER_RADIUS, 0);

    // anel da defesa: ciano enquanto protege, cinza durante a recarga (#17)
    if (defense_state() == 1)
    {
        al_draw_circle(view_x(player.x), view_y(player.y), PLAYER_RADIUS + 2,
            al_map_rgb(0, 200, 255), 2);
    }
    else if (defense_state() == 2)
    {
        al_draw_circle(view_x(player.x), view_y(player.y), PLAYER_RADIUS + 2,
            al_map_rgb(90, 90, 90), 2);
    }

    // projetil na cor do raio escolhido no disparo (#63)
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].active)
        {
            continue;
        }

        if (projectiles[i].laser == LASER_GRAM_POSITIVE)
        {
            al_draw_filled_circle(view_x(projectiles[i].x), view_y(projectiles[i].y),
                PROJECTILE_RADIUS, al_map_rgb(190, 90, 255));
        }
        else
        {
            al_draw_filled_circle(view_x(projectiles[i].x), view_y(projectiles[i].y),
                PROJECTILE_RADIUS, al_map_rgb(255, 70, 70));
        }
    }

    // feedback educacional curto da ultima decisao (#178)
    message = feedback_text();

    if (message != NULL)
    {
        al_draw_text(font, al_map_rgb(255, 230, 140), SCREEN_W / 2, PLAY_H - 26,
            ALLEGRO_ALIGN_CENTRE, message);
    }

    // marca o destino quando ele esta fora da faixa visivel (#19)
    draw_destination_hint();

    // o clip do painel nao pode intersectar o clip do mundo, senao o HUD some
    al_reset_clipping_rectangle();

    draw_hud();
}

void draw_message(const char* title, const char* hint)
{
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 - 20, ALLEGRO_ALIGN_CENTRE, title);
    al_draw_text(font, al_map_rgb(160, 160, 160), SCREEN_W / 2, SCREEN_H / 2 + 10, ALLEGRO_ALIGN_CENTRE, hint);
}

// resumo da fase concluida com os numeros reais da partida (#57, #171-176)
void draw_results()
{
    char line[96];
    int neutralized = threats_total() - threats_left();

    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 44, ALLEGRO_ALIGN_CENTRE,
        "FASE CONCLUIDA");
    al_draw_text(font, al_map_rgb(150, 150, 150), SCREEN_W / 2, 66, ALLEGRO_ALIGN_CENTRE,
        "resumo do encontro");

    snprintf(line, sizeof(line), "pontos: %d", score);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 106, ALLEGRO_ALIGN_CENTRE, line);

    snprintf(line, sizeof(line), "ameacas neutralizadas: %d/%d", neutralized, threats_total());
    al_draw_text(font, al_map_rgb(220, 220, 220), SCREEN_W / 2, 128, ALLEGRO_ALIGN_CENTRE, line);

    snprintf(line, sizeof(line), "agentes identificados: %d/%d", identified_count, NUM_AGENTES);
    al_draw_text(font, al_map_rgb(220, 220, 220), SCREEN_W / 2, 150, ALLEGRO_ALIGN_CENTRE, line);

    if (identification_bonus_awarded())
    {
        snprintf(line, sizeof(line), "bonus de identificacao: +%d", IDENTIFICATION_BONUS);
        al_draw_text(font, al_map_rgb(120, 220, 140), SCREEN_W / 2, 172, ALLEGRO_ALIGN_CENTRE, line);
    }
    else
    {
        snprintf(line, sizeof(line), "bonus de identificacao: nao obtido (%d/%d)",
            identified_count, NUM_AGENTES);
        al_draw_text(font, al_map_rgb(200, 170, 90), SCREEN_W / 2, 172, ALLEGRO_ALIGN_CENTRE, line);
    }

    al_draw_text(font, al_map_rgb(170, 170, 170), SCREEN_W / 2, 232, ALLEGRO_ALIGN_CENTRE,
        "enter = continuar");
}

// fim da execucao: pontuacao final e reinicio (#52)
void draw_victory()
{
    char line[64];

    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTRE, "VITORIA");

    snprintf(line, sizeof(line), "pontuacao final: %d", score);
    al_draw_text(font, al_map_rgb(240, 190, 70), SCREEN_W / 2, 140, ALLEGRO_ALIGN_CENTRE, line);

    al_draw_text(font, al_map_rgb(170, 170, 170), SCREEN_W / 2, 190, ALLEGRO_ALIGN_CENTRE,
        "enter = nova partida");
}

// cada estado desenha so o que e dele, tudo no canvas logico
void render()
{
    GameScene scene = game_get_scene();

    update_screen_fit();
    al_set_target_bitmap(canvas);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (scene == GAME_SCENE_MENU)
    {
        draw_menu();
    }
    else if (scene == GAME_SCENE_GAME)
    {
        draw_game();
    }
    else if (scene == GAME_SCENE_PAUSE)
    {
        draw_message("PAUSE", "esc para voltar");
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
        draw_message("GAME OVER", "enter = nova partida");
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

    if (scene == GAME_SCENE_MENU)
    {
        if (key == ALLEGRO_KEY_ENTER)
        {
            reset_match();
            game_set_scene(GAME_SCENE_GAME);
        }
    }
    else if (scene == GAME_SCENE_GAME)
    {
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
        else if (key == ALLEGRO_KEY_SPACE)
        {
            use_scanner();
        }
        else if (key == ALLEGRO_KEY_ESCAPE)
        {
            game_set_scene(GAME_SCENE_PAUSE);
        }
        else if (key == ALLEGRO_KEY_J)
        {
            shoot();
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
#ifdef _DEBUG
        // teclas de QA: so existem no Debug, nunca no Release (#76)
        // F forca a condicao real de conclusao para testar a transicao da #70
        else if (key == ALLEGRO_KEY_F)
        {
            int i;

            for (i = 0; i < NUM_AGENTES; i++)
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
        // H identifica os 9 e conclui, para testar o caminho do bonus (#67)
        else if (key == ALLEGRO_KEY_H)
        {
            int i;

            for (i = 0; i < NUM_AGENTES; i++)
            {
                agents[i].layers = MAX_LAYERS;
                agents[i].identified = true;
            }

            identified_count = NUM_AGENTES;

            for (i = 0; i < NUM_AGENTES; i++)
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
    else if (scene == GAME_SCENE_PAUSE)
    {
        if (key == ALLEGRO_KEY_ESCAPE)
        {
            game_set_scene(GAME_SCENE_GAME);
        }
    }
    else if (scene == GAME_SCENE_RESULTS)
    {
        if (key == ALLEGRO_KEY_ENTER)
        {
            game_set_scene(GAME_SCENE_VICTORY);
        }
    }
    else if (scene == GAME_SCENE_VICTORY)
    {
        if (key == ALLEGRO_KEY_ENTER)
        {
            reset_match();
            game_set_scene(GAME_SCENE_GAME);
        }
    }
    else if (scene == GAME_SCENE_GAME_OVER)
    {
        if (key == ALLEGRO_KEY_ENTER)
        {
            reset_match();
            game_set_scene(GAME_SCENE_GAME);
        }
    }
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

// clique do mouse vem em pixel da janela: converte para pixel da tela logica
void handle_mouse_click(float x, float y)
{
    GameScene scene = game_get_scene();
    float lx = (x - screen_off_x) / screen_scale;
    float ly = (y - screen_off_y) / screen_scale;

    if (scene == GAME_SCENE_MENU)
    {
        if (lx >= BUTTON_X && lx <= BUTTON_X + BUTTON_W
            && ly >= BUTTON_Y && ly <= BUTTON_Y + BUTTON_H)
        {
            reset_match();
            game_set_scene(GAME_SCENE_GAME);
        }
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

    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // a partida so atualiza no estado GAME
                if (game_get_scene() == GAME_SCENE_GAME)
                {
                    update_match(deltaTime);

                    if (player.health == 0)
                    {
                        game_set_scene(GAME_SCENE_GAME_OVER);
                    }
                    // fase cumprida entra em Resultados sozinha (#70)
                    else if (phase_completed())
                    {
                        game_set_scene(GAME_SCENE_RESULTS);
                    }
                }
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                handle_key_down(event.keyboard.keycode);
                break;

            case ALLEGRO_EVENT_KEY_UP:
                handle_key_up(event.keyboard.keycode);
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                handle_mouse_click(event.mouse.x, event.mouse.y);
                break;

            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                al_acknowledge_resize(event.display.source);
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