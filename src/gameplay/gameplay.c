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

// o tratamento e de duas doses: o feedback fala em "primeira dose" (#215)
_Static_assert(BACTERIA_RESISTANCE == 2, "o tratamento assume duas doses");

// tempo da partida em segundos, so conta dentro do estado GAME
double match_time = 0;

// tempo somado da run, preservado entre fases (#M4)
double run_time = 0;

// pontos da partida atual: so a neutralizacao de uma ameaca soma (#66)
int score = 0;

// vitais do paciente: so a run nova enche; entre as fases ha o piso de 60 (#234)
float patient_vitals = PATIENT_VITALS_MAX;

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

// cargas de Reserva que restam na fase atual (#216)
static int reserve_left = RESERVE_CHARGES;

// ultimo evento de audio e quantos ja sairam: o main toca um som por id (#31)
static unsigned int sfx_event_count = 0;
static SfxEvent sfx_event_last = SFX_EVENT_NONE;

// lugar do ultimo evento no mundo, para a resposta visual (#221)
static float sfx_event_x = 0.0f;
static float sfx_event_y = 0.0f;

// ultimo alvo lido pelo scanner: o painel do HUD mostra so ele
static int scan_target = -1;

// agente com a dica "botao direito: escanear"; -1 com a dica escondida (#219)
static int hint_target = -1;

// bacteria do tutorial: espera parada ate o primeiro scan; -1 fora da Fase 1 (#219)
static int tutorial_index = -1;

// fase concluida: vira true uma vez e so o reset_match desliga (#26)
static bool phase_finished = false;

// bonus de identificacao pago na conclusao, lido pela tela de resultados (#67)
static bool bonus_awarded = false;

// pontos dos vitais pagos na conclusao, lidos pela tela de resultados (#218)
static int vitals_bonus = 0;

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

// raio de colisao de cada especie, em escala biologica: virus < bacteria < celula (#235)
float agent_radius(int index)
{
    if (agents[index].species == SPECIES_VIRUS)
    {
        return (float)VIRUS_RADIUS;
    }

    if (agents[index].species == SPECIES_CELL)
    {
        return (float)CELL_RADIUS;
    }

    return (float)BACTERIA_RADIUS;
}

// registra o evento de audio do quadro; o main toca e zera a pendencia (#31)
static void emit_sfx(SfxEvent kind)
{
    sfx_event_count = sfx_event_count + 1;
    sfx_event_last = kind;
    sfx_event_x = player.x;
    sfx_event_y = player.y;
}

// evento num agente: a resposta visual aparece em cima dele (#221)
static void emit_sfx_at(SfxEvent kind, float x, float y)
{
    emit_sfx(kind);
    sfx_event_x = x;
    sfx_event_y = y;
}

void sfx_event_position(float* x, float* y)
{
    *x = sfx_event_x;
    *y = sfx_event_y;
}

float invulnerability_left(void)
{
    return (float)invulnerability_timer;
}

unsigned int sfx_event_id(void)
{
    return sfx_event_count;
}

SfxEvent sfx_event_kind(void)
{
    return sfx_event_last;
}

// camada 1: so a forma da bacteria, sem cor nem nome que entreguem o grupo (#214)
const char* layer_one_text(int index)
{
    static char text[96];
    Species species = agents[index].species;

    if (species == SPECIES_CELL)
    {
        return "célula do paciente · eucarionte";
    }

    // o virus nao tem coloracao de Gram: a camada 1 diz o que ele e (#M4)
    if (species == SPECIES_VIRUS)
    {
        snprintf(text, sizeof(text), "vírus · %s", agents[index].name);
        return text;
    }

    if (agents[index].shape == SHAPE_BACILLUS)
    {
        return "forma: bacilo (bastão)";
    }

    return "forma: coco (esférica)";
}

// camada 2: coloracao de Gram, estrutura do envoltorio e o nome (#214)
// a interpretacao e do jogador: o scanner nao recomenda arma
const char* layer_two_text(int index)
{
    static char text[96];
    Species species = agents[index].species;

    if (species == SPECIES_GRAM_POSITIVE)
    {
        snprintf(text, sizeof(text), "Gram+ · parede grossa, sem membrana externa · %s",
            agents[index].name);
        return text;
    }

    if (species == SPECIES_GRAM_NEGATIVE)
    {
        snprintf(text, sizeof(text), "Gram- · parede fina sob membrana externa · %s",
            agents[index].name);
        return text;
    }

    if (species == SPECIES_VIRUS)
    {
        return "Vírus precisa de uma célula hospedeira para se multiplicar";
    }

    return "núcleo guarda o DNA · mitocôndrias · não atacar";
}

// segunda linha da camada 2: o que a celula e (#214)
// procarionte x eucarionte e o conteudo da linha; nenhuma recomendacao de arma
const char* layer_two_reason(int index)
{
    Species species = agents[index].species;

    if (species == SPECIES_GRAM_POSITIVE || species == SPECIES_GRAM_NEGATIVE)
    {
        return "procarionte · sem núcleo: DNA solto no nucleoide";
    }

    if (species == SPECIES_VIRUS)
    {
        return "a célula hospedeira é o que ele usa para se multiplicar";
    }

    return "eucarionte: tem núcleo, como as células do corpo";
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
    float reach;
    int i;

    for (i = 0; i < agents_count; i++)
    {
        // agente neutralizado nao encosta mais no nanobo; a celula do paciente
        // nao fere o nanobo, entao nao conta como contato (#235)
        if (!agents[i].active || agents[i].species == SPECIES_CELL)
        {
            continue;
        }

        reach = (float)PLAYER_RADIUS + agent_radius(i);

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
        agents[index].dose_timer = 0.0f;

        // o acerto que neutralizou e o unico evento que pontua (#66)
        score = score + SCORE_PER_THREAT;
        run_progress.neutralized = run_progress.neutralized + 1;

        return true;
    }

    // primeira dose numa bacteria: liga o prazo da segunda (#215)
    if (agents[index].species == SPECIES_GRAM_POSITIVE
        || agents[index].species == SPECIES_GRAM_NEGATIVE)
    {
        agents[index].dose_timer = (float)INCOMPLETE_TIME;
    }

    return false;
}

// dano na integridade do nanobo, respeitando a invulnerabilidade da #11;
// devolve se o dano aconteceu (#218, #234)
static bool damage_player(float amount)
{
    if (player.health <= 0)
    {
        return false;
    }

    if (invulnerability_timer > 0.0)
    {
        return false;
    }

    player.health = player.health - amount;

    if (player.health < 0.0f)
    {
        player.health = 0.0f;
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

// cards da run: cada um aparece uma vez por partida, na ordem em que acontece (#222)
static bool card_shown[CARD_COUNT];
static bool card_waiting[CARD_COUNT];

static void queue_card(ConceptCard card)
{
    if (card_shown[card])
    {
        return;
    }

    card_shown[card] = true;
    card_waiting[card] = true;
}

ConceptCard pending_card(void)
{
    int i;

    for (i = 0; i < CARD_COUNT; i++)
    {
        if (card_waiting[i])
        {
            return (ConceptCard)i;
        }
    }

    return CARD_NONE;
}

void dismiss_card(void)
{
    ConceptCard card = pending_card();

    if (card != CARD_NONE)
    {
        card_waiting[card] = false;
    }
}

// um projetil contra os agentes: o primeiro que cruzar leva o acerto e o projetil some (#49)
static void hit_projectiles()
{
    int i;
    int j;
    float reach;

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

            reach = (float)PROJECTILE_RADIUS + agent_radius(j);

            if (squared_distance(projectiles[i].x, projectiles[i].y, agents[j].x, agents[j].y) > reach * reach)
            {
                continue;
            }

            // tiro num invasor sem leitura: a dica do scan volta, presa a ele (#219)
            if (agents[j].species != SPECIES_CELL && agents[j].layers == 0)
            {
                hint_target = j;
            }

            // levar tiro acorda o invasor que vagava (#235)
            if (agents[j].species != SPECIES_CELL && agents[j].ai_state == AI_WANDER)
            {
                agents[j].ai_state = AI_ALERT;
                agents[j].lost_timer = 0.0f;
            }

            // atacar a celula do paciente custa vitais do paciente, com qualquer
            // ferramenta; a integridade do nanobo nao muda (#14, #234)
            if (agents[j].species == SPECIES_CELL)
            {
                progress.cell_attacks = progress.cell_attacks + 1;
                patient_vitals = fmaxf(patient_vitals - CELL_SHOT_VITALS, 0.0f);
                emit_sfx_at(SFX_EVENT_WRONG, agents[j].x, agents[j].y);
                set_feedback("você feriu uma célula do paciente: ela tem núcleo, não é bactéria");
            }
            else if (projectiles[i].laser == LASER_RESERVE
                && (agents[j].species == SPECIES_GRAM_POSITIVE
                    || agents[j].species == SPECIES_GRAM_NEGATIVE))
            {
                bool was_resistant = agents[j].resistant;

                // o relatorio separa a Reserva bem usada da gasta numa comum (#223)
                if (was_resistant)
                {
                    progress.reserve_on_resistant = progress.reserve_on_resistant + 1;
                }
                else
                {
                    progress.reserve_on_common = progress.reserve_on_common + 1;
                }

                // Reserva: uma dose elimina qualquer bacteria, resistente ou nao;
                // gasta numa comum, o feedback lembra o uso racional (#216)
                progress.correct_decisions = progress.correct_decisions + 1;
                agents[j].resistance = 1;
                hit_agent(j);
                emit_sfx_at(SFX_EVENT_NEUTRALIZED, agents[j].x, agents[j].y);

                if (was_resistant)
                {
                    set_feedback("a Reserva eliminou a resistente: +10 pontos");
                }
                else
                {
                    set_feedback("Reserva numa bactéria comum: guarde para a resistente");
                }
            }
            else if (weapon_affects(projectiles[i].laser, agents[j].species)
                && agents[j].resistant)
            {
                // resistente: o antibiotico do tratamento incompleto nao age mais (#215)
                emit_sfx_at(SFX_EVENT_WRONG, agents[j].x, agents[j].y);

                if (agents[j].species == SPECIES_GRAM_POSITIVE)
                {
                    set_feedback("resistente à Ruptura: use a Reserva");
                }
                else
                {
                    set_feedback("resistente à Dissolução: use a Reserva");
                }
            }
            else if (weapon_affects(projectiles[i].laser, agents[j].species))
            {
                progress.correct_decisions = progress.correct_decisions + 1;

                if (hit_agent(j))
                {
                    emit_sfx_at(SFX_EVENT_NEUTRALIZED, agents[j].x, agents[j].y);

                    // duas doses do antibiotico certo: tratamento completo (#223)
                    if (agents[j].species != SPECIES_VIRUS)
                    {
                        progress.treatments_completed = progress.treatments_completed + 1;
                    }

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
                else
                {
                    // primeira dose: o anel do prazo comeca a esvaziar (#215)
                    emit_sfx_at(SFX_EVENT_HIT, agents[j].x, agents[j].y);
                    set_feedback("primeira dose: complete o tratamento");
                }
            }
            else
            {
                progress.wrong_decisions = progress.wrong_decisions + 1;

                // antibiotico errado nao muda a bacteria: nem dose, nem resistencia;
                // o custo e a trava do disparo (#215)
                fire_lock_timer = FIRE_LOCK_TIME;
                emit_sfx_at(SFX_EVENT_WRONG, agents[j].x, agents[j].y);

                // quem leu so a forma chutou o grupo: o feedback aponta a coloracao (#215)
                if (agents[j].layers == 1)
                {
                    progress.shape_guesses = progress.shape_guesses + 1;
                    set_feedback("a forma não define o grupo: veja a coloração");
                }
                else if (agents[j].species == SPECIES_GRAM_NEGATIVE)
                {
                    set_feedback("a membrana externa barrou a Ruptura");
                }
                else
                {
                    set_feedback("a Gram+ não tem membrana externa para a Dissolução");
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
    // com card na tela o scanner espera (#222)
    if (pending_card() != CARD_NONE)
    {
        return;
    }

    // o painel do HUD relembra o ultimo alvo, mesmo depois de andar
    scan_target = index;

    // quem escaneou ja sabe o gesto: a dica some (#219)
    hint_target = -1;

    if (agents[index].layers < MAX_LAYERS)
    {
        agents[index].layers = agents[index].layers + 1;
        emit_sfx_at(SFX_EVENT_SCAN, agents[index].x, agents[index].y);
    }

    if (agents[index].layers == MAX_LAYERS && agents[index].identified == false)
    {
        agents[index].identified = true;

        // a primeira leitura completa de bacteria abre o card da coloracao (#222)
        if (agents[index].species == SPECIES_GRAM_POSITIVE
            || agents[index].species == SPECIES_GRAM_NEGATIVE)
        {
            queue_card(CARD_GRAM);
        }

        // a filha da divisao fica identificada, mas so os agentes iniciais
        // contam para o bonus e para o resumo (#217)
        if (index >= agents_initial())
        {
            return;
        }

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
        float tolerance;

        // agente neutralizado nao e candidato do scanner
        if (!agents[i].active)
        {
            continue;
        }

        dist_cursor = squared_distance(agents[i].x, agents[i].y, x, y);

        // a celula grande aceita o clique em toda a area dela (#235)
        tolerance = fmaxf(CURSOR_SCAN_TOLERANCE, agent_radius(i) + 8.0f);

        if (dist_cursor > tolerance * tolerance)
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

    // com card na tela nao ha disparo (#222)
    if (pending_card() != CARD_NONE)
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

    // Reserva sem carga nao dispara: avisa e nao gasta a recarga (#216)
    if (player.laser == LASER_RESERVE && reserve_left <= 0)
    {
        set_feedback("Reserva esgotada");
        emit_sfx(SFX_EVENT_WRONG);
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

    // a carga e a dose: sai no disparo, acerte ou nao (#216)
    if (player.laser == LASER_RESERVE)
    {
        reserve_left = reserve_left - 1;
        run_progress.reserve_spent = run_progress.reserve_spent + 1;
    }
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

// roda do mouse passa para a proxima ferramenta da fase (#198, #216)
void toggle_module()
{
    if (player.laser == LASER_GRAM_POSITIVE)
    {
        player.laser = LASER_GRAM_NEGATIVE;
    }
    else if (player.laser == LASER_GRAM_NEGATIVE && reserve_available())
    {
        player.laser = LASER_RESERVE;
    }
    else
    {
        player.laser = LASER_GRAM_POSITIVE;
    }
}

// a Reserva e ferramenta da Fase 1: so aparece onde ha bacteria (#216)
bool reserve_available(void)
{
    return phase_has_bacteria();
}

int reserve_charges_left(void)
{
    return reserve_left;
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

// cada agente da composicao precisa de um ponto de nascimento proprio;
// as filhas da divisao nascem ao lado da mae, fora da tabela (#217)
_Static_assert(NUM_AGENTES <= NUM_VALID_POINTS, "faltam pontos para os agentes da fase");
_Static_assert(BACTERIA_CAP < MAX_AGENTS, "o teto de bacterias precisa caber no array");

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

    // tutorial: a primeira bacteria da Fase 1 nasce perto do nanobo, a vista no
    // primeiro quadro, para o jogador aprender o scan no proprio mundo (#219)
    if (phase_index == 0)
    {
        for (i = 0; i < agents_count; i++)
        {
            if (agents[i].species == SPECIES_GRAM_POSITIVE
                || agents[i].species == SPECIES_GRAM_NEGATIVE)
            {
                agents[i].x = player.x + TUTORIAL_OFFSET_X;
                agents[i].y = player.y + TUTORIAL_OFFSET_Y;
                hint_target = i;
                tutorial_index = i;
                agents[i].ai_state = AI_HOLD;
                break;
            }
        }
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
    float reach;

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

        reach = (float)PLAYER_RADIUS + agent_radius(i);
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

        // o empurrao arranca o coco da celula que ele colonizava (#235)
        agents[i].colonizing = false;
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

// tempo ate a proxima divisao: DIVISION_TIME com o sorteio de DIVISION_JITTER (#217)
static float random_division_time(void)
{
    float unit = (float)rand() / ((float)RAND_MAX + 1.0f);

    return DIVISION_TIME - DIVISION_JITTER + 2.0f * DIVISION_JITTER * unit;
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
    agents[index].dose_timer = 0.0f;
    agents[index].resistant = false;
    agents[index].division_timer = random_division_time();

    // IA comeca vagando; a primeira investida ou toxina vem depois do intervalo (#235)
    agents[index].ai_state = AI_WANDER;
    agents[index].lost_timer = 0.0f;
    agents[index].attack_timer = BACILLUS_LUNGE_INTERVAL;
    agents[index].windup = 0.0f;
    agents[index].lunge_timer = 0.0f;
    agents[index].target_cell = -1;
    agents[index].colonizing = false;
    agents[index].colonize_time = 0.0f;
    agents[index].cell_health = CELL_HEALTH;

    // virus nasce lisogenico com o tempo cheio; quem nao e virus ignora (#84)
    agents[index].virus_state = VIRUS_LYSOGENIC;
    agents[index].lysogenic_timer = LYSOGENIC_TIME;
    agents[index].generation = 0;

    // cada agente sorteia o sprite quando nasce; o desenho so mostra depois
    // da camada 1, antes disso a bacteria e o disco cinza (#211)
    agents[index].variant = rand() % SPRITE_VARIANTS;

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

// prazo da segunda dose: ao zerar, a bacteria fica resistente ao antibiotico
// que recebeu, e a dose dada se perde (#215); so a Reserva age nela (#216)
static void update_dose(int index, double dt)
{
    if (agents[index].dose_timer <= 0.0f)
    {
        return;
    }

    agents[index].dose_timer -= (float)dt;

    if (agents[index].dose_timer > 0.0f)
    {
        return;
    }

    agents[index].dose_timer = 0.0f;
    agents[index].resistant = true;
    agents[index].resistance = species_resistance(agents[index].species);

    // a primeira resistente abre o card da resistencia e da Reserva (#222)
    queue_card(CARD_RESISTANCE);
    progress.resistances = progress.resistances + 1;

    // resistencia criada custa pontos, sem deixar a pontuacao negativa (#215)
    score = score - RESISTANCE_PENALTY;

    if (score < 0)
    {
        score = 0;
    }

    emit_sfx_at(SFX_EVENT_WRONG, agents[index].x, agents[index].y);

    if (agents[index].species == SPECIES_GRAM_POSITIVE)
    {
        set_feedback("tratamento incompleto: resistente à Ruptura");
    }
    else
    {
        set_feedback("tratamento incompleto: resistente à Dissolução");
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

// pontos que os vitais renderam na conclusao (#218)
int vitals_bonus_awarded(void)
{
    return vitals_bonus;
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

    if (phase_finished || player.health <= 0 || patient_vitals <= 0.0f)
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

    // o paciente mais estavel rende mais: vitais x 0,5 no fim da fase (#218)
    vitals_bonus = (int)(patient_vitals * VITALS_BONUS_RATE);
    score = score + vitals_bonus;

    // bonus de identificacao completa, so na conclusao e so uma vez (#67)
    if (identified_count == agents_initial())
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
            agents[i].shape = phases[index].members[i].shape;
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
    player.facing = PLAYER_FACING_DOWN;
    player.vx = 0.0f;
    player.vy = 0.0f;

    // a integridade do nanobo enche em cada fase; os vitais do paciente nao (#234)
    player.health = PLAYER_MAX_HEALTH;

    // so a Fase 1 nasce com a dica; o place_agents liga ela na bacteria do tutorial
    hint_target = -1;
    tutorial_index = -1;

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
    progress.treatments_completed = 0;
    progress.resistances = 0;
    progress.reserve_on_resistant = 0;
    progress.reserve_on_common = 0;
    progress.shape_guesses = 0;

    phase_finished = false;
    bonus_awarded = false;
    vitals_bonus = 0;
    feedback = NULL;
    feedback_timer = 0.0;
    match_time = 0;
    invulnerability_timer = 0.0;
    defense_timer = 0.0;
    defense_cooldown_timer = 0.0;
    attack_cooldown_timer = 0.0;
    fire_lock_timer = 0.0;

    // as cargas da Reserva voltam a cada fase; fora da Fase 1 ela nao existe (#216)
    reserve_left = RESERVE_CHARGES;

    if (player.laser == LASER_RESERVE && !reserve_available())
    {
        player.laser = LASER_GRAM_POSITIVE;
    }

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
    run_progress.reserve_spent = 0;

    // os vitais do paciente so enchem numa run nova; entre as fases continuam (#218, #234)
    patient_vitals = PATIENT_VITALS_MAX;

    // cada card aparece uma vez por partida (#222)
    for (int c = 0; c < CARD_COUNT; c++)
    {
        card_shown[c] = false;
        card_waiting[c] = false;
    }

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

// a composicao da fase, sem as filhas da divisao (#217)
int agents_initial(void)
{
    return phases[phase_index].count;
}

// a dica some sozinha quando o agente dela sai de cena (#219)
int tutorial_hint_target(void)
{
    if (hint_target < 0 || hint_target >= agents_count || !agents[hint_target].active)
    {
        return -1;
    }

    return hint_target;
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
        // o interludio recupera o paciente ate no minimo 60 (#218)
        if (patient_vitals < VITALS_MIN_BETWEEN)
        {
            patient_vitals = VITALS_MIN_BETWEEN;
        }

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
    float r = agent_radius(index);
    float min_x = r;
    float max_x = (float)WORLD_W - r;
    float min_y = r;
    float max_y = (float)WORLD_H - r;

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
    float min_dist;
    float player_reach;

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

        // agente x nanobo: o nanobo e fixo, o agente recua a sobreposicao inteira;
        // a celula do paciente e fixa e quem recua e o nanobo (#235)
        player_reach = (float)PLAYER_RADIUS + agent_radius(i);
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

            if (agents[i].species == SPECIES_CELL)
            {
                player.x -= nx * (player_reach - dist);
                player.y -= ny * (player_reach - dist);
            }
            else
            {
                agents[i].x += nx * (player_reach - dist);
                agents[i].y += ny * (player_reach - dist);

                // quem esta no empurrao da defesa mantem a direcao (#17)
                if (agents[i].impulse_timer <= 0.0f)
                {
                    agents[i].dx = nx;
                    agents[i].dy = ny;
                }

                bounce_agent_off_bounds(i);
            }
        }

        // agente x agente
        for (j = i + 1; j < agents_count; j++)
        {
            float push;

            if (!agents[j].active)
            {
                continue;
            }

            min_dist = agent_radius(i) + agent_radius(j);
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

            push = min_dist - dist;

            // a celula do paciente e fixa: o outro recua a sobreposicao inteira (#235)
            if (agents[i].species == SPECIES_CELL && agents[j].species != SPECIES_CELL)
            {
                agents[j].x += nx * push;
                agents[j].y += ny * push;
            }
            else if (agents[j].species == SPECIES_CELL && agents[i].species != SPECIES_CELL)
            {
                agents[i].x -= nx * push;
                agents[i].y -= ny * push;
            }
            else
            {
                agents[i].x -= nx * push / 2.0f;
                agents[i].y -= ny * push / 2.0f;
                agents[j].x += nx * push / 2.0f;
                agents[j].y += ny * push / 2.0f;
            }

            if (agents[i].impulse_timer <= 0.0f)
            {
                agents[i].dx = -nx;
                agents[i].dy = -ny;
            }

            if (agents[j].impulse_timer <= 0.0f)
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
// celula do paciente perdida (colonizada ate o fim, ou rompida pelo virus):
// ela some e os vitais do paciente caem (#234)
void lose_patient_cell(int index)
{
    if (index < 0 || index >= agents_count || !agents[index].active
        || agents[index].species != SPECIES_CELL)
    {
        return;
    }

    agents[index].active = false;
    patient_vitals = fmaxf(patient_vitals - CELL_LOSS_VITALS, 0.0f);
    emit_sfx_at(SFX_EVENT_DAMAGE, agents[index].x, agents[index].y);
    set_feedback("uma célula do paciente foi destruída: -20 vitais");
}

// divisao binaria: a filha nasce ao lado da mae, numa vaga nova do array, com a
// resistencia e as camadas lidas dela, mas sem a dose que a mae recebeu (#217)
static void divide_bacteria(int index)
{
    int i;
    int alive = 0;
    int slot = agents_count;
    float angle;
    float offset = 2.0f * agent_radius(index);

    agents[index].division_timer = random_division_time();

    for (i = 0; i < agents_count; i++)
    {
        if (agents[i].active && (agents[i].species == SPECIES_GRAM_POSITIVE
            || agents[i].species == SPECIES_GRAM_NEGATIVE))
        {
            alive = alive + 1;
        }
    }

    // teto de bacterias vivas ou array cheio: a mae espera a proxima volta
    if (alive >= BACTERIA_CAP || slot >= MAX_AGENTS)
    {
        return;
    }

    agents[slot] = agents[index];
    agents[slot].resistance = species_resistance(agents[slot].species);
    agents[slot].dose_timer = 0.0f;
    agents[slot].impulse_timer = 0.0f;
    agents[slot].division_timer = random_division_time();
    agents[slot].host_cell = -1;

    // a filha nasce livre: vagando, sem celula e sem ataque em curso (#235)
    agents[slot].ai_state = AI_WANDER;
    agents[slot].target_cell = -1;
    agents[slot].colonizing = false;
    agents[slot].colonize_time = 0.0f;
    agents[slot].windup = 0.0f;
    agents[slot].lunge_timer = 0.0f;
    agents[slot].attack_timer = BACILLUS_LUNGE_INTERVAL;

    angle = ((float)rand() / ((float)RAND_MAX + 1.0f)) * 2.0f * (float)M_PI;
    agents[slot].x = clamp_to_world(agents[index].x + cosf(angle) * offset, (float)WORLD_W);
    agents[slot].y = clamp_to_world(agents[index].y + sinf(angle) * offset, (float)WORLD_H);
    pick_new_direction(slot);

    agents_count = agents_count + 1;

    // a filha e uma ameaca a mais na run (#M4)
    run_progress.threats = run_progress.threats + 1;

    set_feedback("uma bactéria se dividiu em duas");
}

// conta o tempo de divisao de cada bacteria viva (#217)
static void update_division(int index, double dt)
{
    if (agents[index].species != SPECIES_GRAM_POSITIVE
        && agents[index].species != SPECIES_GRAM_NEGATIVE)
    {
        return;
    }

    agents[index].division_timer -= (float)dt;

    if (agents[index].division_timer <= 0.0f)
    {
        divide_bacteria(index);
    }
}

// vagar: anda reto e troca de direcao quando o tempo do agente acaba
static void wander(int index, float dt)
{
    agents[index].direction_timer -= dt;

    if (agents[index].direction_timer <= 0.0f)
    {
        pick_new_direction(index);
    }
}

// celula do paciente ativa mais perto do agente; -1 quando nao sobrou nenhuma (#235)
static int nearest_cell(int index)
{
    int i;
    int best = -1;
    float best_d = 0.0f;

    for (i = 0; i < agents_count; i++)
    {
        float d;

        if (!agents[i].active || agents[i].species != SPECIES_CELL)
        {
            continue;
        }

        d = squared_distance(agents[i].x, agents[i].y, agents[index].x, agents[index].y);

        if (best == -1 || d < best_d)
        {
            best = i;
            best_d = d;
        }
    }

    return best;
}

// alerta: o nanobo a ALERT_RANGE acorda o invasor; longe de LOSE_RANGE por
// LOSE_TIME, ele volta a vagar (#235)
static void update_alert(int index, float dist, float dt)
{
    if (match_time < AI_GRACE_TIME)
    {
        return;
    }

    if (dist <= ALERT_RANGE)
    {
        agents[index].ai_state = AI_ALERT;
        agents[index].lost_timer = 0.0f;
        return;
    }

    if (agents[index].ai_state != AI_ALERT || dist <= LOSE_RANGE)
    {
        agents[index].lost_timer = 0.0f;
        return;
    }

    agents[index].lost_timer += dt;

    if (agents[index].lost_timer >= LOSE_TIME)
    {
        agents[index].ai_state = AI_WANDER;
        agents[index].lost_timer = 0.0f;
    }
}

// bacilo (tem flagelo): persegue o nanobo e da investidas, sempre com aviso;
// devolve a velocidade do quadro (#235)
static float bacillus_ai(int index, float dx, float dy, float dist, float dt)
{
    Agent* a = &agents[index];

    if (a->lunge_timer > 0.0f)
    {
        a->lunge_timer -= dt;
        return BACILLUS_LUNGE_SPEED;
    }

    // aviso: o bacilo para e encolhe antes da investida
    if (a->windup > 0.0f)
    {
        a->windup -= dt;

        if (a->windup > 0.0f)
        {
            return 0.0f;
        }

        a->windup = 0.0f;
        a->lunge_timer = BACILLUS_LUNGE_TIME;
        a->attack_timer = BACILLUS_LUNGE_INTERVAL;

        if (dist > 0.001f)
        {
            a->dx = dx / dist;
            a->dy = dy / dist;
        }

        return BACILLUS_LUNGE_SPEED;
    }

    if (a->ai_state != AI_ALERT)
    {
        wander(index, dt);
        return (float)a->speed;
    }

    if (dist > 0.001f)
    {
        a->dx = dx / dist;
        a->dy = dy / dist;
    }

    a->attack_timer -= dt;

    if (a->attack_timer <= 0.0f)
    {
        // a primeira investida abre o card do flagelo (#235)
        a->windup = WINDUP_TIME;
        queue_card(CARD_FLAGELLUM);
        return 0.0f;
    }

    return BACILLUS_CHASE_SPEED;
}

// coco (sem flagelo): deriva ate a celula do paciente mais perto, gruda nela
// e a destroi aos poucos; devolve a velocidade do quadro (#235)
static float coccus_ai(int index, float dt)
{
    Agent* a = &agents[index];
    int cell;
    float cx;
    float cy;
    float cd;

    if (a->colonizing)
    {
        cell = a->target_cell;

        if (cell >= 0 && agents[cell].active)
        {
            a->colonize_time += dt;
            agents[cell].cell_health -= COLONIZE_DAMAGE * dt;

            if (agents[cell].cell_health <= 0.0f)
            {
                agents[cell].cell_health = 0.0f;
                lose_patient_cell(cell);
                a->colonizing = false;
                a->target_cell = -1;
                a->colonize_time = 0.0f;
            }

            return 0.0f;
        }

        a->colonizing = false;
        a->target_cell = -1;
        a->colonize_time = 0.0f;
    }

    cell = nearest_cell(index);

    if (cell == -1)
    {
        wander(index, dt);
        return (float)a->speed;
    }

    a->target_cell = cell;
    cx = agents[cell].x - a->x;
    cy = agents[cell].y - a->y;
    cd = sqrtf(cx * cx + cy * cy);

    if (cd <= agent_radius(index) + agent_radius(cell) + COLONIZE_REACH)
    {
        a->colonizing = true;
        a->colonize_time = 0.0f;
        return 0.0f;
    }

    a->dx = cx / cd;
    a->dy = cy / cd;
    return COCCUS_DRIFT_SPEED;
}

// cada bacteria age pela forma: o bacilo ameaca o nanobo e o coco ameaca o
// paciente (#235)
static float update_bacteria_ai(int index, float dt)
{
    float dx = player.x - agents[index].x;
    float dy = player.y - agents[index].y;
    float dist = sqrtf(dx * dx + dy * dy);

    update_alert(index, dist, dt);

    if (agents[index].shape == SHAPE_COCCUS)
    {
        return coccus_ai(index, dt);
    }

    return bacillus_ai(index, dx, dy, dist, dt);
}

void update_match(double dt)
{
    float dir_x = 0.0f;
    float dir_y = 0.0f;

    // card de conceito na tela: a partida fica parada ate ele fechar (#222)
    if (pending_card() != CARD_NONE)
    {
        return;
    }

    for (int i = 0; i < agents_count; i++)
    {
        float speed_factor = 1.0f;
        float move_speed = 0.0f;

        if (!agents[i].active)
            continue;

        update_virus(i, dt);
        update_dose(i, dt);
        update_division(i, dt);

        // virus lisogenico ou pronto para o litico fica parado; so o
        // empurrao da defesa o move (#84); andar e regra do litico (#85)
        if (agents[i].species == SPECIES_VIRUS && agents[i].impulse_timer <= 0.0f)
        {
            continue;
        }

        // a celula do paciente e fixa: o endotelio forra o vaso (#235)
        if (agents[i].species == SPECIES_CELL)
        {
            continue;
        }

        // agente segurado: a bacteria do tutorial espera o primeiro scan;
        // o empurrao da defesa ainda move (#219, #235)
        if (agents[i].ai_state == AI_HOLD)
        {
            if (i == tutorial_index && agents[i].layers > 0)
            {
                agents[i].ai_state = AI_WANDER;
            }
            else if (agents[i].impulse_timer <= 0.0f)
            {
                continue;
            }
        }

        // durante o empurrao da defesa a direcao e a velocidade vem do impulso
        if (agents[i].impulse_timer > 0.0f)
        {
            agents[i].impulse_timer -= (float)dt;
            speed_factor = DEFENSE_PUSH_MULTIPLIER;
            move_speed = (float)agents[i].speed;

            if (agents[i].impulse_timer <= 0.0f)
            {
                agents[i].impulse_timer = 0.0f;
                pick_new_direction(i);
            }
        }
        else if (agents[i].species == SPECIES_GRAM_POSITIVE
            || agents[i].species == SPECIES_GRAM_NEGATIVE)
        {
            move_speed = update_bacteria_ai(i, (float)dt);
        }
        else
        {
            wander(i, (float)dt);
            move_speed = (float)agents[i].speed;
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
        if (damage_player(CONTACT_DAMAGE))
        {
            set_feedback("contato com invasor: -10 de integridade");
        }
    }

    collide_agents();

    conclude_phase_if_ready();
}
