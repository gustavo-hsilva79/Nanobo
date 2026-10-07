#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "core/game.h"

#define WIDTH 640
#define HEIGHT 480

#define NUM_AGENTES 3
#define SCAN_RANGE 130
#define MAX_LAYERS 2

#define BUTTON_X 220
#define BUTTON_Y 300
#define BUTTON_W 200
#define BUTTON_H 50

// recursos do jogo, carregados uma vez antes do loop
ALLEGRO_FONT* font = NULL;
ALLEGRO_BITMAP* nanobo = NULL;

// agente do encontro
typedef struct
{
    int id;
    float x;
    float y;
    int kind;      // 0 = gram positiva, 1 = gram negativa, 2 = celula propria
    int layers;    // camadas que o scanner ja revelou
    bool identified;
} Agent;

Agent agents[NUM_AGENTES] =
{
    { 1, 140, 130, 0, 0, false },
    { 2, 490, 150, 1, 0, false },
    { 3, 320, 280, 2, 0, false }
};

int identified_count = 0;
double match_time = 0;

// movimentacao do nanobo
float mouse_x = 0;
float mouse_y = 0;

// onde o nanobo esta na partida
float player_x = 320;
float player_y = 260;

// quantos pixels ele anda por segundo
float player_speed = 200;

// teclas seguradas agora, o update le isso pra mover
bool key_up = false;
bool key_down = false;
bool key_left = false;
bool key_right = false;

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

    return true;
}

// libera o que foi carregado, na ordem inversa
void free_resources()
{
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

// primeira camada, so diz o que apareceu
const char* layer_one_text(int kind)
{
    if (kind == 2)
    {
        return "celula do organismo detectada";
    }

    return "bacteria detectada";
}

// segunda camada, o detalhe que o jogador interpreta
const char* layer_two_text(int kind)
{
    if (kind == 0)
    {
        return "Gram+: parede celular espessa, rica em peptidoglicano";
    }

    if (kind == 1)
    {
        return "Gram-: parede fina de peptidoglicano e membrana externa";
    }

    return "celula saudavel do proprio organismo: preserve-a";
}

// agente valido mais perto, -1 quando ninguem esta no alcance
int pick_target(float x, float y)
{
    int target = -1;
    int i;
    float best = 0;

    for (i = 0; i < NUM_AGENTES; i++)
    {
        float dx = agents[i].x - x;
        float dy = agents[i].y - y;
        float dist = dx * dx + dy * dy;

        if (dist > SCAN_RANGE * SCAN_RANGE)
        {
            continue;
        }

        if (target == -1 || dist < best)
        {
            best = dist;
            target = i;
        }
    }

    return target;
}

// um pulso do scanner, revela a proxima camada do alvo
void use_scanner()
{
    int target = pick_target(player_x, player_y);

    if (target == -1)
    {
        return;
    }

    if (agents[target].layers < MAX_LAYERS)
    {
        agents[target].layers = agents[target].layers + 1;
    }

    if (agents[target].layers == MAX_LAYERS && agents[target].identified == false)
    {
        agents[target].identified = true;
        identified_count = identified_count + 1;
    }
}

// comeca partida nova, a identificacao do encontro e zerada
void reset_match()
{
    int i;

    for (i = 0; i < NUM_AGENTES; i++)
    {
        agents[i].layers = 0;
        agents[i].identified = false;
    }

    player_x = 320;
    player_y = 260;

    key_up = false;
    key_down = false;
    key_left = false;
    key_right = false;

    identified_count = 0;
    match_time = 0;
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
            al_draw_textf(font, al_map_rgb(170, 170, 170), 10, 370 + line * 14, 0, "agente %d: %s", agents[i].id, layer_one_text(agents[i].kind));
            line = line + 1;
        }

        if (agents[i].layers >= 2)
        {
            al_draw_textf(font, al_map_rgb(220, 220, 220), 10, 370 + line * 14, 0, "  %s", layer_two_text(agents[i].kind));
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

void draw_game()
{
    int i;
    ALLEGRO_COLOR tint;

    for (i = 0; i < NUM_AGENTES; i++)
    {
        if (agents[i].kind == 0)
        {
            tint = al_map_rgb(190, 110, 240);
        }
        else if (agents[i].kind == 1)
        {
            tint = al_map_rgb(240, 90, 90);
        }
        else
        {
            tint = al_map_rgb(110, 220, 120);
        }

        al_draw_tinted_bitmap(nanobo, tint, agents[i].x - 16, agents[i].y - 16, 0);
    }

    // o nanobo e desenhado onde o teclado deixou ele
    al_draw_bitmap(nanobo, player_x - 16, player_y - 16, 0);

    al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "identificados: %d/%d", identified_count, NUM_AGENTES);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 26, 0, "tempo: %.1f", match_time);
    al_draw_text(font, al_map_rgb(130, 130, 130), 10, 42, 0, "setas ou wasd para andar    space = scanner    esc = pausa");

    draw_scan_info();
}

void draw_message(const char* title, const char* hint)
{
    al_draw_text(font, al_map_rgb(255, 255, 255), 320, HEIGHT / 2 - 20, ALLEGRO_ALIGN_CENTRE, title);
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
void handle_key_up(int key)
{
    GameScene scene = game_get_scene();

    if (scene != GAME_SCENE_GAME)
    {
        return;
    }

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

// a partida so anda no estado GAME
void update_match(double dt)
{
    float step = player_speed * dt;

    match_time = match_time + dt;

    if (key_up)
    {
        player_y = player_y - step;
    }

    if (key_down)
    {
        player_y = player_y + step;
    }

    if (key_left)
    {
        player_x = player_x - step;
    }

    if (key_right)
    {
        player_x = player_x + step;
    }

    // o nanobo nao sai da tela
    if (player_x < 16)
    {
        player_x = 16;
    }

    if (player_x > WIDTH - 16)
    {
        player_x = WIDTH - 16;
    }

    if (player_y < 16)
    {
        player_y = 16;
    }

    if (player_y > HEIGHT - 16)
    {
        player_y = HEIGHT - 16;
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

    //ALLEGRO_MOUSE mouse;
    bool done = false;
    bool redraw = true;

    ALLEGRO_EVENT event;

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
                }
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                handle_key_down(event.keyboard.keycode);
                break;

            case ALLEGRO_EVENT_KEY_UP:
                handle_key_up(event.keyboard.keycode);
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
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