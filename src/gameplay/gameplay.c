#include "gameplay.h"

#include "../entities/entities.h"

// tempo da partida em segundos, so conta dentro do estado GAME
double match_time = 0;

// teclas seguradas agora, quem escreve nelas e o handle_key_down/handle_key_up
bool key_up = false;
bool key_down = false;
bool key_left = false;
bool key_right = false;

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

// a partida so anda no estado GAME, o tamanho da tela vem do main
void update_match(double dt, float screen_w, float screen_h)
{
    float step = (float)(player_speed * dt);

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

    if (player_x > screen_w - 16)
    {
        player_x = screen_w - 16;
    }

    if (player_y < 16)
    {
        player_y = 16;
    }

    if (player_y > screen_h - 16)
    {
        player_y = screen_h - 16;
    }
}
