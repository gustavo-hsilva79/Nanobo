#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "core/game.h"
#include "entities/entities.h"
#include "gameplay/gameplay.h"

#define WIDTH 640
#define HEIGHT 480

#define BUTTON_X 220
#define BUTTON_Y 300
#define BUTTON_W 200
#define BUTTON_H 50

// recursos do jogo, carregados uma vez antes do loop
ALLEGRO_FONT* font = NULL;
ALLEGRO_BITMAP* nanobo = NULL;

// coracao do HUD de vida (#19)
ALLEGRO_BITMAP* heart_icon = NULL;

// a arte de cada especie da Fase 1
ALLEGRO_BITMAP* gram_positive = NULL;
ALLEGRO_BITMAP* gram_negative = NULL;
ALLEGRO_BITMAP* cell_sprite = NULL;

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

// o que ja foi revelado continua na tela durante o encontro
void draw_scan_info()
{
    int i;
    int line = 0;

    for (i = 0; i < NUM_AGENTES; i++)
    {
        if (agents[i].layers >= 1)
        {
            al_draw_textf(font, al_map_rgb(170, 170, 170), 10, SCAN_PANEL_Y + line * 14, 0, "agente %d: %s", agents[i].id, layer_one_text(agents[i].species));
            line = line + 1;
        }

        if (agents[i].layers >= 2)
        {
            al_draw_textf(font, al_map_rgb(220, 220, 220), 10, SCAN_PANEL_Y + line * 14, 0, "  %s", layer_two_text(agents[i].species));
            line = line + 1;
        }
    }
}

void draw_menu()
{
    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, 120, ALLEGRO_ALIGN_CENTRE, "NANOBO");
    al_draw_text(font, al_map_rgb(170, 170, 170), WIDTH / 2, 150, ALLEGRO_ALIGN_CENTRE, "defenda o organismo");

    al_draw_rectangle(BUTTON_X, BUTTON_Y, BUTTON_X + BUTTON_W, BUTTON_Y + BUTTON_H, al_map_rgb(255, 255, 255), 1);
    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, BUTTON_Y + 20, ALLEGRO_ALIGN_CENTRE, "INICIAR");

    al_draw_text(font, al_map_rgb(130, 130, 130), WIDTH / 2, BUTTON_Y + 80, ALLEGRO_ALIGN_CENTRE, "clique no botao ou aperte enter");
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

void draw_game()
{
    int i;
    int contact;

    for (i = 0; i < NUM_AGENTES; i++)
    {
        // agente neutralizado sai do desenho
        if (!agents[i].active)
        {
            continue;
        }

        al_draw_bitmap(species_sprite(agents[i].species), agents[i].x - AGENT_RADIUS, agents[i].y - AGENT_RADIUS, 0);
    }

    // o nanobo e desenhado onde o teclado deixou ele
    al_draw_bitmap(nanobo, player.x - PLAYER_RADIUS, player.y - PLAYER_RADIUS, 0);

    // anel da defesa: ciano enquanto protege, cinza durante a recarga (#17)
    if (defense_state() == 1)
    {
        al_draw_circle(player.x, player.y, PLAYER_RADIUS + 2, al_map_rgb(0, 200, 255), 2);
    }
    else if (defense_state() == 2)
    {
        al_draw_circle(player.x, player.y, PLAYER_RADIUS + 2, al_map_rgb(90, 90, 90), 2);
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
            al_draw_filled_circle(projectiles[i].x, projectiles[i].y, PROJECTILE_RADIUS, al_map_rgb(190, 90, 255));
        }
        else
        {
            al_draw_filled_circle(projectiles[i].x, projectiles[i].y, PROJECTILE_RADIUS, al_map_rgb(255, 70, 70));
        }
    }

    al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "identificados: %d/%d", identified_count, NUM_AGENTES);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 26, 0, "tempo: %.1f", match_time);
    al_draw_text(font, al_map_rgb(130, 130, 130), 10, 42, 0, "wasd para andar    space = scanner    esc = pausa");
    // a linha da direcao saiu com o #63: o que importa agora e o raio escolhido
    al_draw_text(font, al_map_rgb(130, 130, 130), 10, 58, 0, "j = atacar    1/2 = raio    x = defesa");
    // vida em coracoes de 16x16: os perdidos ficam escurecidos (#19)
    for (i = 0; i < PLAYER_MAX_HEALTH; i++)
    {
        if (i < player.health)
        {
            al_draw_bitmap(heart_icon, 10 + i * 18, 74, 0);
        }
        else
        {
            al_draw_tinted_bitmap(heart_icon, al_map_rgba_f(0.25f, 0.25f, 0.25f, 1.0f), 10 + i * 18, 74, 0);
        }
    }

    // linha temporaria: prova visual do contato enquanto a #11 nao mostra o dano
    contact = touching_agent();
    if (contact != -1)
    {
        al_draw_textf(font, al_map_rgb(240, 200, 90), 10, 90, 0, "encostado no agente %d", agents[contact].id);
    }

    // raio escolhido: roxo para Gram+, vermelho para Gram- (#62)
    if (player.laser == LASER_GRAM_POSITIVE)
    {
        al_draw_text(font, al_map_rgb(190, 90, 255), 10, 106, 0, "raio: roxo (Gram+)");
    }
    else
    {
        al_draw_text(font, al_map_rgb(255, 70, 70), 10, 106, 0, "raio: vermelho (Gram-)");
    }

    draw_scan_info();
}

void draw_message(const char* title, const char* hint)
{
    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 - 20, ALLEGRO_ALIGN_CENTRE, title);
    al_draw_text(font, al_map_rgb(160, 160, 160), WIDTH / 2, HEIGHT / 2 + 10, ALLEGRO_ALIGN_CENTRE, hint);
}

// cada estado desenha so o que e dele
void render()
{
    GameScene scene = game_get_scene();

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
        draw_message("FASE CONCLUIDA", "enter = proxima fase    v = vitoria");
    }
    else if (scene == GAME_SCENE_VICTORY)
    {
        draw_message("VITORIA", "enter = nova partida");
    }
    else if (scene == GAME_SCENE_GAME_OVER)
    {
        draw_message("GAME OVER", "enter = nova partida");
    }

    al_flip_display();
}

// teclado, cada estado so olha as teclas que sao dele
void handle_key_down(int key)
{
    GameScene scene = game_get_scene();

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
        // teclas de teste do fluxo, saem quando as condicoes de verdade existirem
        else if (key == ALLEGRO_KEY_F)
        {
            game_set_scene(GAME_SCENE_RESULTS);
        }
        else if (key == ALLEGRO_KEY_G)
        {
            game_set_scene(GAME_SCENE_GAME_OVER);
        }
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
            game_set_scene(GAME_SCENE_GAME);
        }
        else if (key == ALLEGRO_KEY_V)
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

// clique do mouse, so a interface do menu tem alvo por enquanto
void handle_mouse_click(float x, float y)
{
    GameScene scene = game_get_scene();

    if (scene == GAME_SCENE_MENU)
    {
        if (x >= BUTTON_X && x <= BUTTON_X + BUTTON_W && y >= BUTTON_Y && y <= BUTTON_Y + BUTTON_H)
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

    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    if (!display)
    {
        printf("couldn't initialize display\n");
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
                    update_match(deltaTime, WIDTH, HEIGHT);

                    if (player.health == 0)
                    {
                        game_set_scene(GAME_SCENE_GAME_OVER);
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
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}