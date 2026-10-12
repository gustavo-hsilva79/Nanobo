#include "gameplay.h"

#include <stdlib.h>

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

// agente que o nanobo esta encostando, -1 quando nenhum
// contato e sobreposicao de circulos: a distancia entre os centros e menor
// que a soma dos raios. e uma consulta pura -- nao aplica dano, nao guarda
// estado e nao dispara evento, entao contato que dura varios frames devolve
// sempre o mesmo resultado; quem trata o tempo e a vida (#11)
int touching_agent()
{
    float alcance = (float)(PLAYER_RADIUS + AGENT_RADIUS);
    int i;

    for (i = 0; i < NUM_AGENTES; i++)
    {
        float dx = agents[i].x - player_x;
        float dy = agents[i].y - player_y;

        if (dx * dx + dy * dy <= alcance * alcance)
        {
            return i;
        }
    }

    return -1;
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

// pontos validos do cenario: dentro da area jogavel e fora do painel do scanner
static const float pontos_validos[12][2] =
{
    {  90, 120 }, { 250, 110 }, { 430, 130 }, { 560, 200 },
    {  80, 250 }, { 200, 220 }, { 330, 160 }, { 470, 260 },
    { 110, 340 }, { 260, 340 }, { 400, 330 }, { 550, 350 }
};

// sorteia um ponto valido e diferente para cada agente da fase
void place_agents()
{
    int usados[12] = { 0 };
    int i;

    for (i = 0; i < NUM_AGENTES; i++)
    {
        int p;

        do
        {
            p = rand() % 12;
        }
        while (usados[p] == 1);

        usados[p] = 1;
        agents[i].x = pontos_validos[p][0];
        agents[i].y = pontos_validos[p][1];
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
    player_facing = PLAYER_FACING_DOWN;

    place_agents();

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
        player_facing = PLAYER_FACING_UP;
    }

    if (key_down)
    {
        player_y = player_y + step;
        player_facing = PLAYER_FACING_DOWN;
    }

    if (key_left)
    {
        player_x = player_x - step;
        player_facing = PLAYER_FACING_LEFT;
    }

    if (key_right)
    {
        player_x = player_x + step;
        player_facing = PLAYER_FACING_RIGHT;
    }

    // o nanobo nao sai da tela
    if (player_x < PLAYER_RADIUS)
    {
        player_x = PLAYER_RADIUS;
    }

    if (player_x > screen_w - PLAYER_RADIUS)
    {
        player_x = screen_w - PLAYER_RADIUS;
    }

    if (player_y < PLAYER_RADIUS)
    {
        player_y = PLAYER_RADIUS;
    }

    if (player_y > screen_h - PLAYER_RADIUS)
    {
        player_y = screen_h - PLAYER_RADIUS;
    }
}
