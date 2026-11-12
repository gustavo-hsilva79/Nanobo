#include "gameplay.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../entities/entities.h"

// arquivo do recorde, ao lado do executavel, sem pasta nova (#M4)
#define HI_SCORE_FILE "nanobo_hiscore.txt"

// a invulnerabilidade precisa existir para o dano nao virar por quadro
_Static_assert(INVULNERABILITY_TIME > 0.0, "invulnerabilidade precisa ser positiva");

// o texto do feedback diz "falta 1 acerto": a bacteria tem 2 de resistencia (#105)
_Static_assert(BACTERIA_RESISTANCE == 2, "o feedback assume um acerto restante");

// tempo da partida em segundos, so conta dentro do estado GAME
double match_time = 0;

// tempo somado da run, preservado entre fases (#M4)
double run_time = 0;

// pontos da partida atual: so a neutralizacao de uma ameaca soma (#66)
int score = 0;

// quantos agentes a fase atual colocou no mundo (#M4)
static int agents_count = 0;

// indice da fase corrente: 0 = Fase 1 (#M4)
static int phase_index = 0;

// melhor pontuacao entre execucoes, lida e gravada em arquivo (#M4)
static int best_score = 0;

// contadores do resumo educativo: zerados no reset_match, so memoria (#200)
MatchProgress progress = { 0 };

// totais da run inteira: pontuacao, tempo e estes contadores atravessam fases (#M4)
RunProgress run_progress = { 0 };

// cooldown de invulnerabilidade
static double invulnerability_timer = 0.0;

// janela ativa da defesa e a recarga dela (#17)
static double defense_timer = 0.0;
static double defense_cooldown_timer = 0.0;

// intervalo entre disparos (#64)
static double attack_cooldown_timer = 0.0;

// trava do disparo depois de arma errada (RF-06)
static double fire_lock_timer = 0.0;

// ultimo evento de audio e quantos ja sairam: o main toca um som por id (#31)
static unsigned int sfx_event_count = 0;
static SfxEvent sfx_event_last = SFX_EVENT_NONE;

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

// aproxima value de target sem passar dele, pelo passo maximo (#179)
static float approach(float value, float target, float step)
{
    if (value < target)
    {
        value = value + step;

        if (value > target)
        {
            value = target;
        }
    }
    else if (value > target)
    {
        value = value - step;

        if (value < target)
        {
            value = target;
        }
    }

    return value;
}

// distancia ao quadrado entre dois pontos, evita a raiz quadrada
static float squared_distance(float ax, float ay, float bx, float by)
{
    float dx = ax - bx;
    float dy = ay - by;

    return dx * dx + dy * dy;
}

// registra o evento de audio do quadro; o main toca e zera a pendencia (#31)
static void emit_sfx(SfxEvent kind)
{
    sfx_event_count = sfx_event_count + 1;
    sfx_event_last = kind;
}

unsigned int sfx_event_id(void)
{
    return sfx_event_count;
}

SfxEvent sfx_event_kind(void)
{
    return sfx_event_last;
}

// camada 1: coloracao (roxo/vermelho) e nome do agente (#104)
const char* layer_one_text(Species species, const char* name)
{
    static char text[96];
    const char* color;

    if (species == SPECIES_CELL)
    {
        snprintf(text, sizeof(text), "célula do organismo · %s", name);
        return text;
    }

    // o virus nao tem coloracao de Gram: a camada 1 diz o que ele e (#M4)
    if (species == SPECIES_VIRUS)
    {
        snprintf(text, sizeof(text), "vírus · %s", name);
        return text;
    }

    color = species == SPECIES_GRAM_POSITIVE ? "roxo" : "vermelho";
    snprintf(text, sizeof(text), "%s · %s", color, name);
    return text;
}


// segunda camada: grupo, pista do envelope e o porque; a interpretacao e do jogador (#104)
const char* layer_two_text(Species species)
{
    if (species == SPECIES_GRAM_POSITIVE)
    {
        return "Gram+ · parede espessa, rica em peptidoglicano";
    }

    if (species == SPECIES_GRAM_NEGATIVE)
    {
        return "Gram- · parede fina sob membrana externa";
    }

    if (species == SPECIES_VIRUS)
    {
        return "Vírus precisa de uma célula hospedeira para se multiplicar";
    }

    return "célula saudável do próprio organismo: preserve-a";
}

// o porque do grupo, na mesma linha de leitura da camada 2 (#104)
// a frase repete o que o conteudo biologico validado ja traz: nenhuma
// mecanica nova e nenhuma recomendacao de arma
const char* layer_two_reason(Species species)
{
    if (species == SPECIES_GRAM_POSITIVE)
    {
        return "Essa parede mais espessa distingue este grupo.";
    }

    if (species == SPECIES_GRAM_NEGATIVE)
    {
        return "A membrana externa adicional distingue este grupo.";
    }

    if (species == SPECIES_VIRUS)
    {
        return "a célula hospedeira é o que ele usa para se multiplicar";
    }

    return "preserve a célula: as duas armas ferem o próprio organismo";
}

// agente valido mais perto, -1 quando ninguem esta no alcance
int pick_target(float x, float y)
{
    int target = -1;
    int i;
    float best = 0;

    for (i = 0; i < agents_count; i++)
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

    for (i = 0; i < agents_count; i++)
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

// a arma atinge a especie? o virus da Fase 2 minima aceita as duas (#M4)
static bool weapon_affects(Laser laser, Species species)
{
    if (species == SPECIES_VIRUS)
    {
        return true;
    }

    return (int)laser == laser_for_species(species);
}

// um acerto valido no agente tira 1 de resistencia e neutraliza quando ela zera
// (#49); devolve true so no acerto que neutralizou, o evento unico que a #66 pontua
bool hit_agent(int index)
{
    if (index < 0 || index >= MAX_AGENTS)
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
        run_progress.neutralized = run_progress.neutralized + 1;

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
    emit_sfx(SFX_EVENT_DAMAGE);

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

        for (j = 0; j < agents_count; j++)
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
                progress.cell_attacks = progress.cell_attacks + 1;

                // a mensagem so promete a vida perdida quando o dano aconteceu (#178)
                if (damage_player())
                {
                    if (projectiles[i].laser == LASER_GRAM_POSITIVE)
                    {
                        set_feedback("no jogo, a Ruptura fere a célula própria: -1 vida");
                    }
                    else
                    {
                        set_feedback("no jogo, a Dissolução fere a célula própria: -1 vida");
                    }
                }
            }
            else if (weapon_affects(projectiles[i].laser, agents[j].species))
            {
                progress.correct_decisions = progress.correct_decisions + 1;

                if (hit_agent(j))
                {
                    emit_sfx(SFX_EVENT_NEUTRALIZED);

                    // sem identificacao a ameaca sai da cena e o bonus de 9/9 fica impossivel
                    if (agents[j].identified)
                    {
                        set_feedback("ameaça neutralizada: +10 pontos");
                    }
                    else
                    {
                        set_feedback("neutralizada sem identificar: o bônus de 9/9 se perdeu");
                    }
                }
                else if (agents[j].species == SPECIES_GRAM_POSITIVE)
                {
                    emit_sfx(SFX_EVENT_HIT);
                    set_feedback("Ruptura atingiu a parede: falta 1 acerto");
                }
                else
                {
                    emit_sfx(SFX_EVENT_HIT);
                    set_feedback("Dissolução atingiu a camada extra: falta 1 acerto");
                }
            }
            else
            {
                progress.wrong_decisions = progress.wrong_decisions + 1;

                // arma errada custa caro: trava o disparo e poe so esta bacteria atras do nanobo (RF-06)
                agents[j].alert = true;
                fire_lock_timer = FIRE_LOCK_TIME;
                emit_sfx(SFX_EVENT_WRONG);

                // dica escalonada: pergunta no primeiro erro, mecanismo do segundo em diante (#105)
                if (progress.wrong_decisions == 1)
                {
                    set_feedback("que estrutura o scan mostrou? qual arma tem esse alvo?");
                }
                else if (agents[j].species == SPECIES_GRAM_POSITIVE)
                {
                    set_feedback("no jogo, a Gram+ não tem a camada extra atingida pela Dissolução");
                }
                else
                {
                    set_feedback("no jogo, a camada extra das Gram- dificulta a ação da Ruptura");
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

// revela a proxima camada do alvo e identifica quando as duas apareceram (#60, #61)
static void reveal_layer(int index)
{
    // o painel do HUD relembra o ultimo alvo, mesmo depois de andar
    scan_target = index;

    if (agents[index].layers < MAX_LAYERS)
    {
        agents[index].layers = agents[index].layers + 1;
        emit_sfx(SFX_EVENT_SCAN);
    }

    if (agents[index].layers == MAX_LAYERS && agents[index].identified == false)
    {
        agents[index].identified = true;
        identified_count = identified_count + 1;
        run_progress.identified = run_progress.identified + 1;

        // o resumo conta leitura de agente Gram+ e Gram-, uma vez por agente (#200)
        if (agents[index].species == SPECIES_GRAM_POSITIVE)
        {
            progress.scans_gram_positive = progress.scans_gram_positive + 1;
        }
        else if (agents[index].species == SPECIES_GRAM_NEGATIVE)
        {
            progress.scans_gram_negative = progress.scans_gram_negative + 1;
        }
    }
}

// um pulso do scanner a partir do nanobo, revela a proxima camada do alvo
void use_scanner()
{
    int target = pick_target(player.x, player.y);

    if (target == -1)
    {
        return;
    }

    reveal_layer(target);
}

// agente sob o cursor: tolerancia de 24 px do clique e alcance de 130 px do nanobo (#198)
int pick_target_at(float x, float y)
{
    int target = -1;
    int i;
    float best = 0;

    for (i = 0; i < agents_count; i++)
    {
        float dist_cursor;
        float dist_player;

        // agente neutralizado nao e candidato do scanner
        if (!agents[i].active)
        {
            continue;
        }

        dist_cursor = squared_distance(agents[i].x, agents[i].y, x, y);

        if (dist_cursor > CURSOR_SCAN_TOLERANCE * CURSOR_SCAN_TOLERANCE)
        {
            continue;
        }

        // o alcance continua sendo medido do nanobo, nao do cursor
        dist_player = squared_distance(agents[i].x, agents[i].y, player.x, player.y);

        if (dist_player > SCAN_RANGE * SCAN_RANGE)
        {
            continue;
        }

        if (target == -1 || dist_cursor < best)
        {
            best = dist_cursor;
            target = i;
        }
    }

    return target;
}

// escaneia o agente sob o ponto do mundo, nao o mais perto do nanobo (#198)
void scan_at(float x, float y)
{
    int target = pick_target_at(x, y);

    if (target == -1)
    {
        return;
    }

    reveal_layer(target);
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
    emit_sfx(SFX_EVENT_DEFENSE);
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

// dispara um projetil na direcao (dx, dy) ja normalizada, respeitando defesa e cooldown (#63, #64)
static void fire(float dx, float dy)
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

    // arma errada trava o disparo, mesmo com a recarga pronta (RF-06)
    if (fire_lock_timer > 0.0)
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

    // nasce encostado no nanobo para nao parecer deslocado
    projectiles[free_slot].x = player.x + dx * offset;
    projectiles[free_slot].y = player.y + dy * offset;
    projectiles[free_slot].dx = dx;
    projectiles[free_slot].dy = dy;

    attack_cooldown_timer = ATTACK_COOLDOWN;
    emit_sfx(SFX_EVENT_SHOT);
}

// dispara na direcao do facing, o caminho que o teclado usava (#63, #64)
void shoot()
{
    if (player.facing == PLAYER_FACING_UP)
    {
        fire(0.0f, -1.0f);
    }
    else if (player.facing == PLAYER_FACING_DOWN)
    {
        fire(0.0f, 1.0f);
    }
    else if (player.facing == PLAYER_FACING_LEFT)
    {
        fire(-1.0f, 0.0f);
    }
    else
    {
        fire(1.0f, 0.0f);
    }
}

// mira livre: dispara na direcao do ponto do mundo escolhido pelo cursor (#198)
void shoot_toward(float x, float y)
{
    float dx = x - player.x;
    float dy = y - player.y;
    float dist = sqrtf(dx * dx + dy * dy);

    // cursor colado no nanobo: usa a direcao em que ele olha
    if (dist < AIM_DEAD_ZONE)
    {
        shoot();
        return;
    }

    fire(dx / dist, dy / dist);
}

// roda do mouse alterna os modulos A e B (#198)
void toggle_module()
{
    if (player.laser == LASER_GRAM_POSITIVE)
    {
        player.laser = LASER_GRAM_NEGATIVE;
    }
    else
    {
        player.laser = LASER_GRAM_POSITIVE;
    }
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
_Static_assert(MAX_AGENTS <= NUM_VALID_POINTS, "faltam pontos para todos os agentes");

// sorteia um ponto valido e diferente para cada agente da fase
static void place_agents()
{
    int taken[NUM_VALID_POINTS] = { 0 };
    int i;

    for (i = 0; i < agents_count; i++)
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

// aponta a bacteria alertada para o nanobo; no centro exato, sorteia uma direcao (RF-06)
static void chase_player(int index)
{
    float dx = player.x - agents[index].x;
    float dy = player.y - agents[index].y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 0.001f)
    {
        pick_new_direction(index);
        return;
    }

    agents[index].dx = dx / dist;
    agents[index].dy = dy / dist;
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

    for (i = 0; i < agents_count; i++)
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

    if (species == SPECIES_VIRUS)
    {
        return VIRUS_RESISTANCE;
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
    agents[index].alert = false;

    // virus nasce lisogenico com o tempo cheio; quem nao e virus ignora (#84)
    agents[index].virus_state = VIRUS_LYSOGENIC;
    agents[index].lysogenic_timer = LYSOGENIC_TIME;
    agents[index].generation = 0;

    pick_new_direction(index);
}

// virus lisogenico: parado, piscando e com tempo correndo (#84)
bool virus_is_lysogenic(int index)
{
    if (index < 0 || index >= agents_count)
    {
        return false;
    }

    return agents[index].species == SPECIES_VIRUS
        && agents[index].virus_state == VIRUS_LYSOGENIC;
}

// segundos que faltam do lisogenico; 0 quando nao e virus lisogenico (#84)
float virus_lysogenic_left(int index)
{
    if (!virus_is_lysogenic(index))
    {
        return 0.0f;
    }

    return agents[index].lysogenic_timer;
}

// conta o tempo do virus lisogenico e marca o fim uma vez so (#84)
// a multiplicacao e o eclipse ficam para a #85/#86
static void update_virus(int index, double dt)
{
    if (agents[index].species != SPECIES_VIRUS)
    {
        return;
    }

    if (agents[index].virus_state != VIRUS_LYSOGENIC)
    {
        return;
    }

    agents[index].lysogenic_timer -= (float)dt;

    if (agents[index].lysogenic_timer <= 0.0f)
    {
        agents[index].lysogenic_timer = 0.0f;
        agents[index].virus_state = VIRUS_LYTIC_READY;
    }
}

// quantas ameacas ainda estao ativas: celula saudavel nao conta (#26)
int threats_left()
{
    int i;
    int total = 0;

    for (i = 0; i < agents_count; i++)
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

    for (i = 0; i < agents_count; i++)
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

    if (fire_lock_timer > 0.0)
    {
        return false;
    }

    if (defense_timer > 0.0)
    {
        return false;
    }

    return attack_cooldown_timer <= 0.0;
}

// fracao restante da recarga do ataque: 1 logo depois do disparo, 0 quando pronto
// (a defesa ativa tambem bloqueia o disparo, mas nao gasta a recarga)
float attack_cooldown_left()
{
    if (attack_cooldown_timer <= 0.0)
    {
        return 0.0f;
    }

    return (float)(attack_cooldown_timer / ATTACK_COOLDOWN);
}

// true enquanto a trava da arma errada segura o disparo (RF-06)
bool fire_locked()
{
    return fire_lock_timer > 0.0;
}

// fracao restante da trava: 1 logo depois do erro, 0 quando o disparo volta
float fire_lock_left()
{
    if (fire_lock_timer <= 0.0)
    {
        return 0.0f;
    }

    return (float)(fire_lock_timer / FIRE_LOCK_TIME);
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
    emit_sfx(SFX_EVENT_PHASE_DONE);

    // bonus de identificacao completa, so na conclusao e so uma vez (#67)
    if (identified_count == agents_count)
    {
        score = score + IDENTIFICATION_BONUS;
        bonus_awarded = true;
    }
}

// comeca partida nova: o encontro e o nanobo voltam ao estado inicial
// zera o estado transitorio de uma fase e coloca os agentes dela no mundo (#M4)
static void start_phase(int index)
{
    int i;

    phase_index = index;
    agents_count = phases[index].count;

    for (i = 0; i < MAX_AGENTS; i++)
    {
        if (i < agents_count)
        {
            agents[i].name = phases[index].members[i].name;
            agents[i].id = phases[index].members[i].id;
            agents[i].species = phases[index].members[i].species;
            reset_agent(i);
        }
        else
        {
            agents[i].active = false;
        }
    }

    // liga cada virus a celula hospedeira pelo id da composicao (#89)
    for (i = 0; i < agents_count; i++)
    {
        int j;

        agents[i].host_cell = -1;

        for (j = 0; j < agents_count; j++)
        {
            if (phases[index].members[i].host != 0
                && agents[j].species == SPECIES_CELL
                && agents[j].id == phases[index].members[i].host)
            {
                agents[i].host_cell = j;
            }
        }
    }

    // a run soma o que cada fase acrescenta (#M4)
    run_progress.threats = run_progress.threats + threats_total();
    run_progress.agents = run_progress.agents + agents_count;

    player.x = (float)WORLD_W / 2.0f;
    player.y = (float)WORLD_H / 2.0f;
    player.health = PLAYER_MAX_HEALTH;
    player.facing = PLAYER_FACING_DOWN;
    player.vx = 0.0f;
    player.vy = 0.0f;

    place_agents();

    key_up = false;
    key_down = false;
    key_left = false;
    key_right = false;

    identified_count = 0;

    // contadores do resumo educativo voltam a zero a cada fase (#200)
    progress.scans_gram_positive = 0;
    progress.scans_gram_negative = 0;
    progress.correct_decisions = 0;
    progress.wrong_decisions = 0;
    progress.cell_attacks = 0;

    phase_finished = false;
    bonus_awarded = false;
    feedback = NULL;
    feedback_timer = 0.0;
    match_time = 0;
    invulnerability_timer = 0.0;
    defense_timer = 0.0;
    defense_cooldown_timer = 0.0;
    attack_cooldown_timer = 0.0;
    fire_lock_timer = 0.0;

    // fase nova comeca sem projetil na tela
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        projectiles[i].active = false;
    }

    scan_target = -1;

    // a camera ja nasce no lugar certo do mundo novo
    update_camera();
}

// comeca uma run nova na Fase 1: pontuacao e tempo da run zerados (#M4)
void reset_match()
{
    score = 0;
    run_time = 0;

    run_progress.threats = 0;
    run_progress.neutralized = 0;
    run_progress.agents = 0;
    run_progress.identified = 0;

    start_phase(0);
}

// quantas fases a partida tem e em qual delas o jogo esta
int phase_count()
{
    return PHASE_COUNT;
}

int current_phase()
{
    return phase_index + 1;
}

// agentes que a fase atual colocou no mundo: 9 na Fase 1, 6 na Fase 2 (#M4)
int agents_total()
{
    return agents_count;
}

// Verifica se a fase atual possui pelo menos uma bactéria.
bool phase_has_bacteria()
{
    int i;

    for (i = 0; i < agents_count; i++)
    {
        if (agents[i].species == SPECIES_GRAM_POSITIVE ||
            agents[i].species == SPECIES_GRAM_NEGATIVE)
        {
            return true;
        }
    }

    return false;
}

// true quando os Resultados devem carregar outra fase (#M4)
bool has_next_phase()
{
    return phase_index + 1 < PHASE_COUNT;
}

// Resultados -> proxima fase: a pontuacao da run continua (#M4)
void start_next_phase()
{
    if (has_next_phase())
    {
        start_phase(phase_index + 1);
    }
}

// melhor pontuacao ja feita, lida do arquivo no inicio do jogo (#M4)
int hi_score()
{
    return best_score;
}

// le o recorde do disco; arquivo ausente ou invalido vale zero
void load_hi_score()
{
    FILE* file = NULL;
    int value = 0;

    if (fopen_s(&file, HI_SCORE_FILE, "r") == 0 && file != NULL)
    {
        if (fscanf_s(file, "%d", &value) != 1 || value < 0)
        {
            value = 0;
        }

        fclose(file);
    }

    best_score = value;
}

// grava a melhor pontuacao quando a run termina em vitoria; sem arquivo, so memoria
void finish_run()
{
    FILE* file = NULL;

    if (score <= best_score)
    {
        return;
    }

    best_score = score;

    if (fopen_s(&file, HI_SCORE_FILE, "w") != 0 || file == NULL)
    {
        return;
    }

    fprintf(file, "%d\n", best_score);
    fclose(file);
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

// separa agentes sobrepostos entre si e do nanobo
static void collide_agents()
{
    int i;
    int j;
    float min_dist = (float)(AGENT_RADIUS * 2);
    float player_reach = (float)(PLAYER_RADIUS + AGENT_RADIUS);

    for (i = 0; i < agents_count; i++)
    {
        float dx;
        float dy;
        float dist;
        float nx;
        float ny;

        if (!agents[i].active)
        {
            continue;
        }

        // agente x nanobo: o nanobo e fixo, o agente recua a sobreposicao inteira
        dx = agents[i].x - player.x;
        dy = agents[i].y - player.y;

        if (dx * dx + dy * dy < player_reach * player_reach)
        {
            dist = sqrtf(dx * dx + dy * dy);

            if (dist < 0.001f)
            {
                nx = 1.0f;
                ny = 0.0f;
            }
            else
            {
                nx = dx / dist;
                ny = dy / dist;
            }

            agents[i].x += nx * (player_reach - dist);
            agents[i].y += ny * (player_reach - dist);

            // quem persegue ou esta no empurrao da defesa mantem a direcao (RF-06, #17)
            if (!agents[i].alert && agents[i].impulse_timer <= 0.0f)
            {
                agents[i].dx = nx;
                agents[i].dy = ny;
            }

            bounce_agent_off_bounds(i);
        }

        // agente x agente
        for (j = i + 1; j < agents_count; j++)
        {
            float push;

            if (!agents[j].active)
            {
                continue;
            }

            dx = agents[j].x - agents[i].x;
            dy = agents[j].y - agents[i].y;

            if (dx * dx + dy * dy >= min_dist * min_dist)
            {
                continue;
            }

            dist = sqrtf(dx * dx + dy * dy);

            if (dist < 0.001f)
            {
                nx = 1.0f;
                ny = 0.0f;
            }
            else
            {
                nx = dx / dist;
                ny = dy / dist;
            }

            push = (min_dist - dist) / 2.0f;

            agents[i].x -= nx * push;
            agents[i].y -= ny * push;
            agents[j].x += nx * push;
            agents[j].y += ny * push;

            if (!agents[i].alert && agents[i].impulse_timer <= 0.0f)
            {
                agents[i].dx = -nx;
                agents[i].dy = -ny;
            }

            if (!agents[j].alert && agents[j].impulse_timer <= 0.0f)
            {
                agents[j].dx = nx;
                agents[j].dy = ny;
            }

            bounce_agent_off_bounds(i);
            bounce_agent_off_bounds(j);
        }
    }
}

// a partida so anda no estado GAME; o mundo vem das constantes
void update_match(double dt)
{
    float dir_x = 0.0f;
    float dir_y = 0.0f;

    for (int i = 0; i < agents_count; i++)
    {
        float speed_factor = 1.0f;
        int move_speed = agents[i].speed;

        if (!agents[i].active)
            continue;

        update_virus(i, dt);

        // virus lisogenico ou pronto para o litico fica parado; so o
        // empurrao da defesa o move (#84); andar e regra do litico (#85)
        if (agents[i].species == SPECIES_VIRUS && agents[i].impulse_timer <= 0.0f)
        {
            continue;
        }

        // durante o empurrao da defesa a direcao e a velocidade vem do impulso
        if (agents[i].impulse_timer > 0.0f)
        {
            agents[i].impulse_timer -= (float)dt;
            speed_factor = DEFENSE_PUSH_MULTIPLIER;

            if (agents[i].impulse_timer <= 0.0f)
            {
                agents[i].impulse_timer = 0.0f;

                // o fim do empurrao nao pode apagar a perseguicao (RF-06)
                if (!agents[i].alert)
                {
                    pick_new_direction(i);
                }
            }
        }
        else if (agents[i].alert)
        {
            // so a bacteria que levou arma errada persegue, e ate ser neutralizada (RF-06)
            chase_player(i);
            move_speed = CHASE_SPEED;
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

        agents[i].x += (float)(agents[i].dx * move_speed * speed_factor * dt);
        agents[i].y += (float)(agents[i].dy * move_speed * speed_factor * dt);

        bounce_agent_off_bounds(i);
    }

    match_time = match_time + dt;
    run_time = run_time + dt;

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

    // trava do disparo depois de arma errada (RF-06)
    if (fire_lock_timer > 0.0)
    {
        fire_lock_timer -= dt;

        if (fire_lock_timer < 0.0)
        {
            fire_lock_timer = 0.0;
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

    // direcao pedida pelas teclas: a diagonal soma os dois eixos, como antes (#179)
    if (key_up)
    {
        dir_y -= 1.0f;
        player.facing = PLAYER_FACING_UP;
    }

    if (key_down)
    {
        dir_y += 1.0f;
        player.facing = PLAYER_FACING_DOWN;
    }

    if (key_left)
    {
        dir_x -= 1.0f;
        player.facing = PLAYER_FACING_LEFT;
    }

    if (key_right)
    {
        dir_x += 1.0f;
        player.facing = PLAYER_FACING_RIGHT;
    }

    // acelera enquanto a tecla esta apertada e freia quando ela solta (#179)
    player.vx = approach(player.vx, dir_x * player.speed,
        (dir_x != 0.0f ? PLAYER_ACCEL : PLAYER_DECEL) * (float)dt);
    player.vy = approach(player.vy, dir_y * player.speed,
        (dir_y != 0.0f ? PLAYER_ACCEL : PLAYER_DECEL) * (float)dt);

    player.x = player.x + player.vx * (float)dt;
    player.y = player.y + player.vy * (float)dt;

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
            set_feedback("contato com bactéria: -1 de vida");
        }
    }

    collide_agents();

    conclude_phase_if_ready();
}
