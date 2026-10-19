#include "gameplay.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../entities/entities.h"

// a invulnerabilidade precisa existir para o dano nao virar por quadro
_Static_assert(INVULNERABILITY_TIME > 0.0, "invulnerabilidade precisa ser positiva");

// tempo da partida em segundos, so conta dentro do estado GAME
double match_time = 0;

// cooldown de invulnerabilidade
static double invulnerability_timer = 0.0;

// janela ativa da defesa e a recarga dela (#17)
static double defense_timer = 0.0;
static double defense_cooldown_timer = 0.0;

// intervalo entre disparos (#64)
static double attack_cooldown_timer = 0.0;

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
const char* layer_one_text(Species species)
{
    if (species == SPECIES_CELL)
    {
        return "celula do organismo detectada";
    }

    return "bacteria detectada";
}

// segunda camada, o detalhe que o jogador interpreta
const char* layer_two_text(Species species)
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

// raio que neutraliza cada especie: roxo para Gram+, vermelho para Gram- (#62)
static int laser_for_species(Species species)
{
    if (species == SPECIES_GRAM_POSITIVE)
    {
        return LASER_GRAM_POSITIVE;
    }

    return LASER_GRAM_NEGATIVE;
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

// dano no player: 1 de vida, respeitando a invulnerabilidade da #11
static void damage_player()
{
    if (player.health <= 0)
    {
        return;
    }

    if (invulnerability_timer > 0.0)
    {
        return;
    }

    player.health--;

    if (player.health < 0)
    {
        player.health = 0;
    }

    invulnerability_timer = INVULNERABILITY_TIME;
}

// um projetil contra os agentes: o primeiro que cruzar leva o acerto e o projetil some (#49)
static void hit_projectiles()
{
    int i;
    int j;
    float reach = (float)(PROJECTILE_RADIUS + AGENT_RADIUS);

    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].active)
        {
            continue;
        }

        for (j = 0; j < NUM_AGENTES; j++)
        {
            if (!agents[j].active)
            {
                continue;
            }

            if (squared_distance(projectiles[i].x, projectiles[i].y, agents[j].x, agents[j].y) > reach * reach)
            {
                continue;
            }

            // atacar a celula saudavel custa 1 de vida ao player (#14)
            if (agents[j].species == SPECIES_CELL)
            {
                damage_player();
            }
            else if (projectiles[i].laser == laser_for_species(agents[j].species))
            {
                hit_agent(j);
            }

            projectiles[i].active = false;
            break;
        }
    }
}

// projetis andam em linha reta e somem ao sair da area jogavel (#63)
static void update_projectiles(double dt, float screen_w)
{
    int i;
    float floor_y = (float)SCAN_PANEL_Y;

    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].active)
        {
            continue;
        }

        projectiles[i].x += (float)(projectiles[i].dx * PROJECTILE_SPEED * dt);
        projectiles[i].y += (float)(projectiles[i].dy * PROJECTILE_SPEED * dt);

        if (projectiles[i].x < 0 || projectiles[i].x > screen_w
            || projectiles[i].y < 0 || projectiles[i].y > floor_y)
        {
            projectiles[i].active = false;
        }
    }

    hit_projectiles();
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

// aperta a defesa: so ativa quando ela esta pronta (#17)
void activate_defense()
{
    if (player.health <= 0)
    {
        return;
    }

    // durante a janela ativa ou a recarga o aperto nao faz nada
    if (defense_timer > 0.0 || defense_cooldown_timer > 0.0)
    {
        return;
    }

    defense_timer = DEFENSE_DURATION;
}

// 1 = defesa ativa, 2 = recarregando, 0 = pronta; o main desenha o anel com isso
int defense_state()
{
    if (defense_timer > 0.0)
    {
        return 1;
    }

    if (defense_cooldown_timer > 0.0)
    {
        return 2;
    }

    return 0;
}

// dispara um projetil na direcao do facing, respeitando defesa e cooldown (#63, #64)
void shoot()
{
    int i;
    int free_slot = -1;
    float offset = (float)(PLAYER_RADIUS + PROJECTILE_RADIUS);

    if (player.health <= 0)
    {
        return;
    }

    // ataque e defesa nao acontecem juntos (#17)
    if (defense_timer > 0.0)
    {
        return;
    }

    if (attack_cooldown_timer > 0.0)
    {
        return;
    }

    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].active)
        {
            free_slot = i;
            break;
        }
    }

    // sem espaco livre o disparo nao acontece, e o cooldown nao e gasto
    if (free_slot == -1)
    {
        return;
    }

    projectiles[free_slot].active = true;
    projectiles[free_slot].laser = player.laser;
    projectiles[free_slot].x = player.x;
    projectiles[free_slot].y = player.y;
    projectiles[free_slot].dx = 0.0f;
    projectiles[free_slot].dy = 0.0f;

    // nasce encostado no nanobo para nao parecer deslocado
    if (player.facing == PLAYER_FACING_UP)
    {
        projectiles[free_slot].y = player.y - offset;
        projectiles[free_slot].dy = -1.0f;
    }
    else if (player.facing == PLAYER_FACING_DOWN)
    {
        projectiles[free_slot].y = player.y + offset;
        projectiles[free_slot].dy = 1.0f;
    }
    else if (player.facing == PLAYER_FACING_LEFT)
    {
        projectiles[free_slot].x = player.x - offset;
        projectiles[free_slot].dx = -1.0f;
    }
    else
    {
        projectiles[free_slot].x = player.x + offset;
        projectiles[free_slot].dx = 1.0f;
    }

    attack_cooldown_timer = ATTACK_COOLDOWN;
}

// pontos validos do cenario, a regra de cada um esta em docs/game-design/gdd.md
// quantos pontos validos a tabela tem; o array usa esta constante
#define NUM_VALID_POINTS 12

static const float valid_points[][2] =
{
    {  90, 120 }, { 250, 110 }, { 430, 130 }, { 560, 200 },
    {  80, 250 }, { 200, 220 }, { 330, 160 }, { 470, 260 },
    { 110, 340 }, { 260, 340 }, { 400, 330 }, { 550, 350 }
};

// o array e sem dimensao de proposito: assim a contagem e a real e uma linha
// faltando (ou sobrando) quebra o build em vez de virar zero
_Static_assert(sizeof(valid_points) / sizeof(valid_points[0]) == NUM_VALID_POINTS,
    "valid_points precisa ter NUM_VALID_POINTS linhas");

// cada agente precisa de um ponto de nascimento proprio
_Static_assert(NUM_AGENTES <= NUM_VALID_POINTS, "faltam pontos para todos os agentes");

// sorteia um ponto valido e diferente para cada agente da fase
static void place_agents()
{
    int taken[NUM_VALID_POINTS] = { 0 };
    int i;

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

// defesa ativa empurra a bacteria que encosta: direcao do nanobo para o agente (#17)
static void repel_touching_agents()
{
    int i;
    float reach = (float)(PLAYER_RADIUS + AGENT_RADIUS);

    if (defense_timer <= 0.0)
    {
        return;
    }

    for (i = 0; i < NUM_AGENTES; i++)
    {
        float dx;
        float dy;
        float dist;

        if (!agents[i].active)
        {
            continue;
        }

        // a celula propria nao e empurrada: o comportamento dela e da #14
        if (agents[i].species == SPECIES_CELL)
        {
            continue;
        }

        dx = agents[i].x - player.x;
        dy = agents[i].y - player.y;
        dist = sqrtf(dx * dx + dy * dy);

        if (dist > reach)
        {
            continue;
        }

        // centro exato do nanobo: sorteia uma direcao para nao dividir por zero
        if (dist < 0.001f)
        {
            pick_new_direction(i);
        }
        else
        {
            agents[i].dx = dx / dist;
            agents[i].dy = dy / dist;
        }

        agents[i].impulse_timer = (float)DEFENSE_PUSH_TIME;
    }
}

// resistencia de combate que cada especie precisa para ser neutralizada (#49)
static int species_resistance(Species species)
{
    if (species == SPECIES_CELL)
    {
        return CELL_RESISTANCE;
    }

    return BACTERIA_RESISTANCE;
}

// velocidade de cada especie: a celula saudavel e mais lenta que a bacteria (#14)
static int species_speed(Species species)
{
    if (species == SPECIES_CELL)
    {
        return CELL_SPEED;
    }

    return AGENT_SPEED;
}

// devolve o agente ao estado de partida nova: resistencia da especie, ativo de
// novo e direcao propria; a posicao de nascimento quem sorteia e o place_agents
static void reset_agent(int index)
{
    agents[index].resistance = species_resistance(agents[index].species);
    agents[index].speed = species_speed(agents[index].species);
    agents[index].active = true;
    agents[index].layers = 0;
    agents[index].identified = false;
    agents[index].impulse_timer = 0.0f;
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
    defense_timer = 0.0;
    defense_cooldown_timer = 0.0;
    attack_cooldown_timer = 0.0;

    // partida nova comeca sem projetil na tela
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        projectiles[i].active = false;
    }
}

// mantem um valor dentro da tela, respeitando o raio do nanobo
static float clamp_to_screen(float value, float limit)
{
    return fminf(fmaxf(value, (float)PLAYER_RADIUS), limit - (float)PLAYER_RADIUS);
}

// mantem o agente dentro da area jogavel e inverte o sentido quando ele bate
static void bounce_agent_off_bounds(int index, float screen_w)
{
    float min_x = (float)AGENT_RADIUS;
    float max_x = screen_w - (float)AGENT_RADIUS;
    float min_y = (float)AGENT_RADIUS;
    float max_y = (float)(SCAN_PANEL_Y - AGENT_RADIUS);

    if (agents[index].x < min_x)
    {
        agents[index].x = min_x;
        agents[index].dx = -agents[index].dx;
    }
    else if (agents[index].x > max_x)
    {
        agents[index].x = max_x;
        agents[index].dx = -agents[index].dx;
    }

    if (agents[index].y < min_y)
    {
        agents[index].y = min_y;
        agents[index].dy = -agents[index].dy;
    }
    else if (agents[index].y > max_y)
    {
        agents[index].y = max_y;
        agents[index].dy = -agents[index].dy;
    }
}

// a partida so anda no estado GAME, o tamanho da tela vem do main
void update_match(double dt, float screen_w, float screen_h)
{
    float step = (float)(player.speed * dt);

    for (int i = 0; i < NUM_AGENTES; i++)
    {
        float speed_factor = 1.0f;

        if (!agents[i].active)
            continue;

        // durante o empurrao da defesa a direcao e a velocidade vem do impulso
        if (agents[i].impulse_timer > 0.0f)
        {
            agents[i].impulse_timer -= (float)dt;
            speed_factor = DEFENSE_PUSH_MULTIPLIER;

            if (agents[i].impulse_timer <= 0.0f)
            {
                agents[i].impulse_timer = 0.0f;
                pick_new_direction(i);
            }
        }
        else
        {
            agents[i].direction_timer -= (float)dt;

            // a direcao so muda quando o tempo do agente acaba, nunca a cada quadro
            if (agents[i].direction_timer <= 0)
            {
                pick_new_direction(i);
            }
        }

        agents[i].x += (float)(agents[i].dx * agents[i].speed * speed_factor * dt);
        agents[i].y += (float)(agents[i].dy * agents[i].speed * speed_factor * dt);

        bounce_agent_off_bounds(i, screen_w);
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

    // fim da janela de defesa vira o inicio da recarga (#17)
    if (defense_timer > 0.0)
    {
        defense_timer -= dt;

        if (defense_timer <= 0.0)
        {
            defense_timer = 0.0;
            defense_cooldown_timer = DEFENSE_COOLDOWN;
        }
    }
    else if (defense_cooldown_timer > 0.0)
    {
        defense_cooldown_timer -= dt;

        if (defense_cooldown_timer < 0.0)
        {
            defense_cooldown_timer = 0.0;
        }
    }

    // cooldown do ataque (#64)
    if (attack_cooldown_timer > 0.0)
    {
        attack_cooldown_timer -= dt;

        if (attack_cooldown_timer < 0.0)
        {
            attack_cooldown_timer = 0.0;
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

    repel_touching_agents();
    update_projectiles(dt, screen_w);

    int touched = touching_agent();

    if (touched != -1
        && agents[touched].species != SPECIES_CELL
        && defense_timer <= 0.0)
    {
        damage_player();
    }
}
