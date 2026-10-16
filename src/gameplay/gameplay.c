#include "gameplay.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../entities/entities.h"

// tempo da partida em segundos, so conta dentro do estado GAME
double match_time = 0;

// cooldown de invulnerabilidade
static double invulnerability_timer = 0.0;

// teclas seguradas agora, quem escreve nelas e o handle_key_down/handle_key_up
bool key_up = false;
bool key_down = false;
bool key_left = false;
bool key_right = false;

// distancia ao quadrado entre dois pontos, evita a raiz quadrada
static float squared_distance(float ax, float ay, float bx, float by)
{
    float dx = ax - bx;
    float dy = ay - by;

    return dx * dx + dy * dy;
}

// primeira camada, so diz o que apareceu
const char* layer_one_text(int species)
{
    if (species == SPECIES_CELL)
    {
        return "celula do organismo detectada";
    }

    return "bacteria detectada";
}

// segunda camada, o detalhe que o jogador interpreta
const char* layer_two_text(int species)
{
    if (species == SPECIES_GRAM_POSITIVE)
    {
        return "Gram+: parede celular espessa, rica em peptidoglicano";
    }

    if (species == SPECIES_GRAM_NEGATIVE)
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
        float dist;

        // agente neutralizado nao e candidato do scanner
        if (!agents[i].active)
        {
            continue;
        }

        dist = squared_distance(agents[i].x, agents[i].y, x, y);

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
// contato e sobreposicao de circulos; a consulta e pura, quem trata o tempo e a #11
int touching_agent()
{
    float reach = (float)(PLAYER_RADIUS + AGENT_RADIUS);
    int i;

    for (i = 0; i < NUM_AGENTES; i++)
    {
        // agente neutralizado nao encosta mais no nanobo
        if (!agents[i].active)
        {
            continue;
        }

        if (squared_distance(agents[i].x, agents[i].y, player.x, player.y) <= reach * reach)
        {
            return i;
        }
    }

    return -1;
}

// um acerto valido no agente tira 1 de resistencia e neutraliza quando ela zera
// (#49); devolve true so no acerto que neutralizou, o evento unico que a #66 pontua
bool hit_agent(int index)
{
    if (index < 0 || index >= NUM_AGENTES)
    {
        return false;
    }

    // agente ja neutralizado nao leva mais acerto
    if (!agents[index].active)
    {
        return false;
    }

    // celula saudavel nao tem resistencia de combate: um acerto nunca a remove (#14)
    if (agents[index].species == SPECIES_CELL)
    {
        return false;
    }

    agents[index].resistance = agents[index].resistance - 1;

    if (agents[index].resistance <= 0)
    {
        agents[index].active = false;
        return true;
    }

    return false;
}

// um pulso do scanner, revela a proxima camada do alvo
void use_scanner()
{
    int target = pick_target(player.x, player.y);

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

// pontos validos do cenario, a regra de cada um esta em docs/game-design/gdd.md
static const float valid_points[12][2] =
{
    {  90, 120 }, { 250, 110 }, { 430, 130 }, { 560, 200 },
    {  80, 250 }, { 200, 220 }, { 330, 160 }, { 470, 260 },
    { 110, 340 }, { 260, 340 }, { 400, 330 }, { 550, 350 }
};

// quantos pontos a tabela tem, sem repetir o numero
#define NUM_VALID_POINTS ((int)(sizeof(valid_points) / sizeof(valid_points[0])))

// sorteia um ponto valido e diferente para cada agente da fase
static void place_agents()
{
    int taken[NUM_VALID_POINTS] = { 0 };
    int i;

    // sem pontos sobrando dois agentes nasceriam no mesmo lugar (e o laco travaria)
    if (NUM_AGENTES > NUM_VALID_POINTS)
    {
        printf("poucos pontos validos para %d agentes\n", NUM_AGENTES);
        return;
    }

    for (i = 0; i < NUM_AGENTES; i++)
    {
        int p = rand() % NUM_VALID_POINTS;

        while (taken[p] == 1)
        {
            p = (p + 1) % NUM_VALID_POINTS;
        }

        taken[p] = 1;
        agents[i].x = valid_points[p][0];
        agents[i].y = valid_points[p][1];
    }
}

// sorteia uma direcao unitaria nova e conta o tempo ate a proxima troca do agente
static void pick_new_direction(int index)
{
    float angle = ((float)rand() / ((float)RAND_MAX + 1.0f)) * 2.0f * (float)M_PI;

    agents[index].dx = cosf(angle);
    agents[index].dy = sinf(angle);
    agents[index].direction_timer = (float)(rand() % 5 + 1);
}

// resistencia de combate que cada especie precisa para ser neutralizada (#49)
static int species_resistance(int species)
{
    if (species == SPECIES_CELL)
    {
        return CELL_RESISTANCE;
    }

    return BACTERIA_RESISTANCE;
}

// devolve o agente ao estado de partida nova: resistencia da especie, ativo de
// novo e direcao propria; a posicao de nascimento quem sorteia e o place_agents
static void reset_agent(int index)
{
    agents[index].resistance = species_resistance(agents[index].species);
    agents[index].active = true;
    agents[index].layers = 0;
    agents[index].identified = false;
    pick_new_direction(index);
}

// comeca partida nova: o encontro e o nanobo voltam ao estado inicial
void reset_match()
{
    int i;

    // cada agente volta ao estado inicial, inclusive resistencia e direcao
    for (i = 0; i < NUM_AGENTES; i++)
    {
        reset_agent(i);
    }

    player.x = 320;
    player.y = 260;
    player.health = PLAYER_MAX_HEALTH;
    player.facing = PLAYER_FACING_DOWN;

    place_agents();

    key_up = false;
    key_down = false;
    key_left = false;
    key_right = false;

    identified_count = 0;
    match_time = 0;
    invulnerability_timer = 0.0;
}

// mantem um valor dentro da tela, respeitando o raio do nanobo
static float clamp_to_screen(float value, float limit)
{
    if (value < PLAYER_RADIUS)
    {
        return PLAYER_RADIUS;
    }

    if (value > limit - PLAYER_RADIUS)
    {
        return limit - PLAYER_RADIUS;
    }

    return value;
}

// a partida so anda no estado GAME, o tamanho da tela vem do main
void update_match(double dt, float screen_w, float screen_h)
{
    float step = (float)(player.speed * dt);

    for (int i = 0; i < NUM_AGENTES; i++)
    {
        if (!agents[i].active)
            continue;

        agents[i].direction_timer -= (float)dt;

        // a direcao so muda quando o tempo do agente acaba, nunca a cada quadro
        if (agents[i].direction_timer <= 0)
        {
            pick_new_direction(i);
        }

        agents[i].x += (float)(agents[i].dx * agents[i].speed * dt);
        agents[i].y += (float)(agents[i].dy * agents[i].speed * dt);

        // borda: o sentido vira e a posicao volta pra dentro da area jogavel
        if (agents[i].x - AGENT_RADIUS < 0)
        {
            agents[i].x = (float)AGENT_RADIUS;
            agents[i].dx = -agents[i].dx;
        }
        else if (agents[i].x + AGENT_RADIUS > screen_w)
        {
            agents[i].x = screen_w - (float)AGENT_RADIUS;
            agents[i].dx = -agents[i].dx;
        }

        // o painel do scanner ocupa a faixa de baixo, o agente para antes dele
        if (agents[i].y - AGENT_RADIUS < 0)
        {
            agents[i].y = (float)AGENT_RADIUS;
            agents[i].dy = -agents[i].dy;
        }
        else if (agents[i].y + AGENT_RADIUS > SCAN_PANEL_Y)
        {
            agents[i].y = (float)(SCAN_PANEL_Y - AGENT_RADIUS);
            agents[i].dy = -agents[i].dy;
        }
    }

    match_time = match_time + dt;

    if (invulnerability_timer > 0.0)
    {
        invulnerability_timer -= dt;

        if (invulnerability_timer < 0.0)
        {
            invulnerability_timer = 0.0;
        }
    }

    if (key_up)
    {
        player.y = player.y - step;
        player.facing = PLAYER_FACING_UP;
    }

    if (key_down)
    {
        player.y = player.y + step;
        player.facing = PLAYER_FACING_DOWN;
    }

    if (key_left)
    {
        player.x = player.x - step;
        player.facing = PLAYER_FACING_LEFT;
    }

    if (key_right)
    {
        player.x = player.x + step;
        player.facing = PLAYER_FACING_RIGHT;
    }

    // o nanobo nao sai da tela
    player.x = clamp_to_screen(player.x, screen_w);
    player.y = clamp_to_screen(player.y, screen_h);

    int touched = touching_agent();

    if (player.health > 0 
        && touched != -1 
        && agents[touched].species != SPECIES_CELL 
        && invulnerability_timer <= 0.0)
    {
        player.health--;

        if (player.health < 0)
            player.health = 0;

        invulnerability_timer = 1.0;
    }
}
