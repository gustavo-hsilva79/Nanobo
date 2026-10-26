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

// pontos da partida atual: so a neutralizacao de uma ameaca soma (#66)
int score = 0;

// cooldown de invulnerabilidade
static double invulnerability_timer = 0.0;

// janela ativa da defesa e a recarga dela (#17)
static double defense_timer = 0.0;
static double defense_cooldown_timer = 0.0;

// intervalo entre disparos (#64)
static double attack_cooldown_timer = 0.0;

// ultimo alvo lido pelo scanner: o painel do HUD mostra so ele
static int scan_target = -1;

// fase concluida: vira true uma vez e so o reset_match desliga (#26)
static bool phase_finished = false;

// bonus de identificacao pago na conclusao, lido pela tela de resultados (#67)
static bool bonus_awarded = false;

// mensagem educacional da ultima decisao e o tempo que ela ainda fica na tela (#178)
static const char* feedback = NULL;
static double feedback_timer = 0.0;

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

// segunda camada, a pista do envelope que o jogador interpreta (#3)
// o texto descreve a parede e a membrana sem nomear Gram+: a classificacao e do jogador
const char* layer_two_text(Species species)
{
    if (species == SPECIES_GRAM_POSITIVE)
    {
        return "parede celular espessa e clara, sem membrana externa";
    }

    if (species == SPECIES_GRAM_NEGATIVE)
    {
        return "parede fina de peptidoglicano sob membrana externa";
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

        // o acerto que neutralizou e o unico evento que pontua (#66)
        score = score + SCORE_PER_THREAT;

        return true;
    }

    return false;
}

// dano no player: 1 de vida, respeitando a invulnerabilidade da #11; devolve se o dano aconteceu
static bool damage_player()
{
    if (player.health <= 0)
    {
        return false;
    }

    if (invulnerability_timer > 0.0)
    {
        return false;
    }

    player.health--;

    if (player.health < 0)
    {
        player.health = 0;
    }

    invulnerability_timer = INVULNERABILITY_TIME;

    return true;
}

// guarda a mensagem da decisao que acabou de acontecer, sem mexer em nenhum estado (#178)
static void set_feedback(const char* text)
{
    feedback = text;
    feedback_timer = FEEDBACK_TIME;
}

// mensagem atual, NULL quando o tempo dela ja passou (#178)
const char* feedback_text()
{
    return feedback;
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

            // atacar a celula saudavel custa 1 de vida ao player (#14, #178)
            if (agents[j].species == SPECIES_CELL)
            {
                // a mensagem so promete a vida perdida quando o dano aconteceu (#178)
                if (damage_player())
                {
                    set_feedback("celula do organismo: atacar custa 1 de vida");
                }
            }
            else if (projectiles[i].laser == laser_for_species(agents[j].species))
            {
                if (hit_agent(j))
                {
                    // sem identificacao a ameaca sai da cena e o bonus de 9/9 fica impossivel
                    if (agents[j].identified)
                    {
                        set_feedback("ameaca neutralizada: +10 pontos");
                    }
                    else
                    {
                        set_feedback("neutralizada sem identificar: o bonus de 9/9 se perdeu");
                    }
                }
                else
                {
                    set_feedback("acerto valido: a bacteria ainda resiste");
                }
            }
            else
            {
                // raio errado: a mensagem diz qual parede barrou o disparo (#178)
                if (agents[j].species == SPECIES_GRAM_POSITIVE)
                {
                    set_feedback("parede Gram+ espessa: o raio vermelho nao penetra");
                }
                else
                {
                    set_feedback("membrana externa da Gram-: o raio roxo nao penetra");
                }
            }

            projectiles[i].active = false;
            break;
        }
    }
}

// projetis andam em linha reta e somem ao sair do mundo (#63)
static void update_projectiles(double dt)
{
    int i;

    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].active)
        {
            continue;
        }

        projectiles[i].x += (float)(projectiles[i].dx * PROJECTILE_SPEED * dt);
        projectiles[i].y += (float)(projectiles[i].dy * PROJECTILE_SPEED * dt);

        if (projectiles[i].x < 0 || projectiles[i].x > (float)WORLD_W
            || projectiles[i].y < 0 || projectiles[i].y > (float)WORLD_H)
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

    // o painel do HUD relembra o ultimo alvo, mesmo depois de andar
    scan_target = target;

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

// ultimo alvo lido pelo scanner, -1 antes da primeira leitura
int last_scan_target()
{
    return scan_target;
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
    {  160,  96 }, {  480, 140 }, {  800,  96 }, { 1120, 140 },
    { 1440,  96 }, { 1760, 140 }, {  160, 420 }, {  480, 380 },
    {  800, 420 }, { 1120, 380 }, { 1440, 420 }, { 1760, 380 }
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

// a camera segue o nanobo e para nas bordas do mundo
static void update_camera()
{
    float max_x = (float)(WORLD_W - SCREEN_W);
    float max_y = (float)(WORLD_H - PLAY_H);

    camera.x = player.x - (float)SCREEN_W / 2.0f;
    camera.y = player.y - (float)PLAY_H / 2.0f;

    if (camera.x < 0.0f)
    {
        camera.x = 0.0f;
    }

    if (camera.y < 0.0f)
    {
        camera.y = 0.0f;
    }

    if (camera.x > max_x)
    {
        camera.x = max_x;
    }

    if (camera.y > max_y)
    {
        camera.y = max_y;
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

// quantas ameacas ainda estao ativas: celula saudavel nao conta (#26)
int threats_left()
{
    int i;
    int total = 0;

    for (i = 0; i < NUM_AGENTES; i++)
    {
        if (agents[i].active && agents[i].species != SPECIES_CELL)
        {
            total = total + 1;
        }
    }

    return total;
}

// total de ameacas da fase: celula saudavel nao conta (#19)
int threats_total()
{
    int i;
    int total = 0;

    for (i = 0; i < NUM_AGENTES; i++)
    {
        if (agents[i].species != SPECIES_CELL)
        {
            total = total + 1;
        }
    }

    return total;
}

// o bonus de +15 foi pago nesta conclusao? a tela de resultados le isso (#67)
bool identification_bonus_awarded()
{
    return bonus_awarded;
}

// o disparo esta liberado agora? o HUD usa para o indicador do ataque (#19)
bool attack_ready()
{
    if (player.health <= 0)
    {
        return false;
    }

    if (defense_timer > 0.0)
    {
        return false;
    }

    return attack_cooldown_timer <= 0.0;
}

// a fase ja foi concluida? a #70 le isso para entrar em Resultados (#26)
bool phase_completed()
{
    return phase_finished;
}

// conclusao: sem ameacas e com o nanobo no destino, uma vez por partida (#26)
static void conclude_phase_if_ready()
{
    float reach = (float)(PLAYER_RADIUS + DESTINATION_RADIUS);

    if (phase_finished || player.health <= 0)
    {
        return;
    }

    if (threats_left() > 0)
    {
        return;
    }

    if (squared_distance(player.x, player.y, DESTINATION_X, DESTINATION_Y) > reach * reach)
    {
        return;
    }

    phase_finished = true;

    // bonus de identificacao completa, so na conclusao e so uma vez (#67)
    if (identified_count == NUM_AGENTES)
    {
        score = score + IDENTIFICATION_BONUS;
        bonus_awarded = true;
    }
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

    player.x = (float)WORLD_W / 2.0f;
    player.y = (float)WORLD_H / 2.0f;
    player.health = PLAYER_MAX_HEALTH;
    player.facing = PLAYER_FACING_DOWN;

    place_agents();

    key_up = false;
    key_down = false;
    key_left = false;
    key_right = false;

    identified_count = 0;
    score = 0;
    phase_finished = false;
    bonus_awarded = false;
    feedback = NULL;
    feedback_timer = 0.0;
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

    scan_target = -1;

    // a camera ja nasce no lugar certo do mundo novo
    update_camera();
}

// mantem um valor dentro do mundo, respeitando o raio do nanobo
static float clamp_to_world(float value, float limit)
{
    return fminf(fmaxf(value, (float)PLAYER_RADIUS), limit - (float)PLAYER_RADIUS);
}

// mantem o agente dentro do mundo e inverte o sentido quando ele bate
static void bounce_agent_off_bounds(int index)
{
    float min_x = (float)AGENT_RADIUS;
    float max_x = (float)(WORLD_W - AGENT_RADIUS);
    float min_y = (float)AGENT_RADIUS;
    float max_y = (float)(WORLD_H - AGENT_RADIUS);

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

// a partida so anda no estado GAME; o mundo vem das constantes
void update_match(double dt)
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

        bounce_agent_off_bounds(i);
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

    // a mensagem educacional some sozinha e nunca bloqueia o jogo (#178)
    if (feedback_timer > 0.0)
    {
        feedback_timer -= dt;

        if (feedback_timer <= 0.0)
        {
            feedback_timer = 0.0;
            feedback = NULL;
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

    // o nanobo nao sai do mundo
    player.x = clamp_to_world(player.x, (float)WORLD_W);
    player.y = clamp_to_world(player.y, (float)WORLD_H);

    repel_touching_agents();
    update_projectiles(dt);
    update_camera();

    int touched = touching_agent();

    if (touched != -1
        && agents[touched].species != SPECIES_CELL
        && defense_timer <= 0.0)
    {
        // contato a cada quadro nao pode reescrever a mensagem sem dano novo (#178)
        if (damage_player())
        {
            set_feedback("contato com bacteria: -1 de vida");
        }
    }

    conclude_phase_if_ready();
}
