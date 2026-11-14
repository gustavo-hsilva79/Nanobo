#ifndef NANOBO_GAMEPLAY_H
#define NANOBO_GAMEPLAY_H

#include <stdbool.h>

// as regras falam das especies e direcoes do jogo
#include "../entities/entities.h"

// o projeto compila com /std:c11; sem isso o arquivo nao compila de proposito
_Static_assert(__STDC_VERSION__ >= 201112L, "compilar com /std:c11 (ver Nanobo.vcxproj)");

// regras centrais de gameplay ficam declaradas aqui
// (alvo do scanner, camadas, vida, dano, cooldown, pontuacao e fim de fase)

// alcance do scanner em pixels
#define SCAN_RANGE 130

// tolerancia do scan por clique: distancia do cursor ao centro do agente (#198)
#define CURSOR_SCAN_TOLERANCE 24.0f

// tela logica: o jogo desenha num bitmap deste tamanho e a janela amplia
#define SCREEN_W 640
#define SCREEN_H 360

// faixa visivel do mundo; multipla de 32, o lado de um tile do mapa futuro
#define PLAY_H 256

// painel do scanner no rodape da tela logica
#define PANEL_H 104

// o painel comeca onde a faixa visivel do mundo acaba
#define SCAN_PANEL_Y PLAY_H

// a tela e a faixa mais o painel, e tudo fecha com o tile de 32 px
_Static_assert(PANEL_H + PLAY_H == SCREEN_H, "tela = faixa do mundo + painel");
_Static_assert(SCREEN_W % 32 == 0, "largura da tela precisa ser multipla de 32");
_Static_assert(PLAY_H % 32 == 0, "faixa do mundo precisa ser multipla de 32");
_Static_assert(WORLD_W % SCREEN_W == 0, "mundo precisa ter um numero inteiro de telas");
_Static_assert(WORLD_H % PLAY_H == 0, "mundo precisa ter um numero inteiro de faixas");
_Static_assert(WORLD_W % 32 == 0 && WORLD_H % 32 == 0, "mundo precisa fechar no tile de 32");

// camadas que o scanner revela antes de identificar o agente
#define MAX_LAYERS 2

// defesa (#17): janela ativa, recarga e forca do empurrao em quem encosta
#define DEFENSE_DURATION 2.0
#define DEFENSE_COOLDOWN 3.0
#define DEFENSE_PUSH_TIME 0.5
#define DEFENSE_PUSH_MULTIPLIER 2.0f

// intervalo entre disparos (#64)
#define ATTACK_COOLDOWN 1.0

// antibiotico errado: nao muda a bacteria, mas trava o disparo por 2 s,
// o custo de chutar a arma; a perseguicao da RF-06 saiu no 2.0 (#215)
#define FIRE_LOCK_TIME 2.0

// tratamento em duas doses: depois da 1a, o prazo para a 2a; se ele zera,
// a bacteria fica resistente ao antibiotico que recebeu (#215)
#define INCOMPLETE_TIME 6.0

// pontos perdidos por resistencia criada; a pontuacao nao fica negativa (#215)
#define RESISTANCE_PENALTY 5

// antibiotico de reserva: cargas por fase, que voltam no inicio de cada uma (#216)
#define RESERVE_CHARGES 2

// divisao binaria: cada bacteria viva se divide a cada 22 s, com 4 s de sorteio
// para mais ou para menos, ate o teto de bacterias vivas na fase (#217)
#define DIVISION_TIME 22.0f
#define DIVISION_JITTER 4.0f
#define BACTERIA_CAP 16

// cursor a menos disso do nanobo cai na direcao do facing (#198)
#define AIM_DEAD_ZONE 8.0f

// pontos por ameaca neutralizada, concedidos uma vez por agente (#66)
#define SCORE_PER_THREAT 10

// destino da Fase 1: canto oposto ao nascimento, visivel desde o inicio (#26)
#define DESTINATION_X ((float)WORLD_W - 64.0f)
#define DESTINATION_Y ((float)WORLD_H - 64.0f)
#define DESTINATION_RADIUS 24

// bonus de identificacao completa ao concluir a fase (#67)
#define IDENTIFICATION_BONUS 15

// quanto tempo a mensagem educacional fica na tela, em segundos (#178)
#define FEEDBACK_TIME 2.5

// tempo de invulnerabilidade depois de levar dano (#11)
#define INVULNERABILITY_TIME 1.0

// IA dos invasores (#235): alerta e perda do alerta, aviso antes do ataque,
// perseguicao e investida do bacilo, deriva e colonizacao do coco
#define ALERT_RANGE 200.0f

// calma no comeco de cada fase: ninguem entra em alerta antes disso, para o
// jogador ver o mapa e aprender o scan (#235)
#define AI_GRACE_TIME 5.0f
#define LOSE_RANGE 320.0f
#define LOSE_TIME 3.0f
#define WINDUP_TIME 0.4f
#define BACILLUS_CHASE_SPEED 70.0f
#define BACILLUS_LUNGE_SPEED 150.0f
#define BACILLUS_LUNGE_TIME 0.4f
#define BACILLUS_LUNGE_INTERVAL 2.5f
#define COCCUS_DRIFT_SPEED 20.0f

// toxina do coco (#236): alcance, intervalo, voo e dano; some sozinha depois
// de ENEMY_SHOT_LIFE; a defesa ativa destroi o que toca o escudo
#define TOXIN_RANGE 180.0f
#define TOXIN_INTERVAL 3.0f
#define TOXIN_SPEED 90.0f
#define TOXIN_DAMAGE 8.0f
#define ENEMY_SHOT_LIFE 4.0f
#define SHIELD_RADIUS 22.0f

// perigos (#237): o biofilme nasce de uma colonia com mais de BIOFILM_TIME,
// deixa o nanobo lento e pede uma dose a mais; dura BIOFILM_LINGER depois da
// colonia. A nuvem de endotoxina dura ENDOTOXIN_TIME e fere ENDOTOXIN_DPS por segundo
#define BIOFILM_TIME 6.0f
#define BIOFILM_RADIUS 72.0f
#define BIOFILM_SLOW 0.5f
#define BIOFILM_LINGER 10.0f
#define ENDOTOXIN_RADIUS 36.0f
#define ENDOTOXIN_TIME 2.5f
#define ENDOTOXIN_DPS 10.0f
#define COLONIZE_DAMAGE 5.0f
#define COLONIZE_REACH 2.0f

// raio de colisao em escala biologica: virus < bacteria < celula do paciente (#235)
#define VIRUS_RADIUS 7
#define BACTERIA_RADIUS 14
#define CELL_RADIUS 28

// duas barras (#234): a integridade do nanobo (player.health) cai com o que
// o fere; os vitais do paciente caem quando uma celula dele e perdida ou ferida.
// nao ha drenagem por tempo: a pressao vem dos invasores (revisao de 14/11)
#define CONTACT_DAMAGE 10.0f
#define PATIENT_VITALS_MAX 100.0f
#define CELL_SHOT_VITALS 15.0f
#define CELL_LOSS_VITALS 20.0f
#define CELL_HEALTH 100.0f
#define VITALS_MIN_BETWEEN 60.0f
#define VITALS_BONUS_RATE 0.5f

// tempo da partida em segundos, so conta dentro do estado GAME
extern double match_time;

// tempo somado da run, atravessa as fases (#M4)
extern double run_time;

// progresso da partida para o resumo educativo (#200): so memoria, sem arquivo
typedef struct
{
    int scans_gram_positive; // agentes Gram+ identificados pelo scanner
    int scans_gram_negative; // agentes Gram- identificados pelo scanner
    int correct_decisions;   // disparos com o modulo certo no alvo
    int wrong_decisions;     // disparos com o modulo errado no alvo
    int cell_attacks;        // disparos que acertaram uma celula propria
    int treatments_completed; // bacterias eliminadas com as duas doses certas (#223)
    int resistances;          // bacterias que ficaram resistentes (#223)
    int reserve_on_resistant; // cargas de Reserva gastas numa resistente (#223)
    int reserve_on_common;    // cargas de Reserva gastas numa bacteria comum (#223)
    int shape_guesses;        // antibiotico errado depois de ler so a forma (#223)
} MatchProgress;

extern MatchProgress progress;

// totais acumulados da run, para as telas de Vitoria e Game Over (#M4)
typedef struct
{
    int threats;     // ameacas das fases ja jogadas
    int neutralized; // ameacas neutralizadas na run
    int agents;      // agentes das fases ja jogadas
    int identified;  // agentes identificados na run
    int reserve_spent; // cargas de Reserva gastas na run, para a Fase 3 (#216)
} RunProgress;

extern RunProgress run_progress;

// pontos da partida atual: a neutralizacao de uma ameaca soma (#66)
extern int score;

// vitais do paciente, de 0 a PATIENT_VITALS_MAX; atravessam as fases (#234)
extern float patient_vitals;

// celula do paciente perdida: some, e os vitais caem CELL_LOSS_VITALS (#234)
void lose_patient_cell(int index);

// teclas seguradas agora, o main escreve isso na entrada e o update le pra mover
extern bool key_up;
extern bool key_down;
extern bool key_left;
extern bool key_right;

// onde nasce a bacteria do tutorial, em relacao ao nanobo: dentro da camera
// do primeiro quadro, um pouco alem do alcance do scan, para ensinar a chegar perto (#219)
#define TUTORIAL_OFFSET_X 140.0f
#define TUTORIAL_OFFSET_Y -40.0f

// dica do tutorial no mundo: o agente onde ela aparece, -1 quando escondida (#219)
int tutorial_hint_target(void);

// raio de colisao do agente pela especie (#235)
float agent_radius(int index);

// velocidade do nanobo agora: BIOFILM_SLOW dentro do biofilme, 1 fora (#237)
float player_slow_factor(void);

#ifdef _DEBUG
// tecla de QA K: um biofilme e uma nuvem de endotoxina perto do nanobo (#237)
void qa_spawn_hazards(void);
#endif

// textos das camadas do scanner pelo indice do agente (#214)
// camada 1 = forma; camada 2 = coloracao, estrutura e nome
const char* layer_one_text(int index);
const char* layer_two_text(int index);
const char* layer_two_reason(int index);
int pick_target(float x, float y);
void use_scanner();

// scan sob o cursor: alvo com tolerancia do clique e alcance do nanobo (#198)
int pick_target_at(float x, float y);
void scan_at(float x, float y);

// defesa: ativa, estado pro anel do HUD e empurrao de quem encosta (#17)
void activate_defense();
int defense_state();

// ataque: um projetil por aperto, respeitando o cooldown (#63, #64)
void shoot();
void shoot_toward(float x, float y);

// ferramentas: a roda alterna entre as da fase; 1, 2 e 3 escolhem direto (#198, #216)
void toggle_module();

// Reserva: so existe em fase com bacteria; cargas que restam nesta fase (#216)
bool reserve_available(void);
int reserve_charges_left(void);

// contato: index do agente que o nanobo esta encostando, -1 quando nenhum
int touching_agent();

// virus da Fase 2: estado lisogenico e tempo que falta para o litico (#84)
bool virus_is_lysogenic(int index);
float virus_lysogenic_left(int index);

// acerto valido no agente: devolve true quando esse acerto o neutralizou (#49)
bool hit_agent(int index);

// mensagem educacional da ultima decisao; NULL quando nao ha nada a mostrar (#178)
const char* feedback_text();

// total de ameacas da fase, mesmo depois de neutralizadas (#19)
int threats_total();

// true quando o bonus de identificacao foi pago na conclusao (#67)
bool identification_bonus_awarded();

// pontos que os vitais renderam na conclusao da fase, 0 antes dela (#218)
int vitals_bonus_awarded(void);

// true quando o disparo esta liberado agora (#19)
bool attack_ready();

// trava do disparo depois de arma errada (RF-06); o HUD usa os dois
bool fire_locked();
float fire_lock_left();

// fracao restante da recarga do ataque, 0 quando pronto (barra do HUD)
float attack_cooldown_left();

// fase: ameacas ainda ativas e se a condicao de conclusao ja foi cumprida (#26)
int threats_left();
bool phase_completed();

// evento de audio da ultima acao: o main toca o som, o gameplay fica sem Allegro (#31)
typedef enum
{
    SFX_EVENT_NONE = 0,
    SFX_EVENT_SCAN,
    SFX_EVENT_SHOT,
    SFX_EVENT_HIT,
    SFX_EVENT_NEUTRALIZED,
    SFX_EVENT_WRONG,
    SFX_EVENT_DAMAGE,
    SFX_EVENT_DEFENSE,
    SFX_EVENT_PHASE_DONE
} SfxEvent;

// identificador do ultimo evento e o tipo dele: o main compara o id (#31)
unsigned int sfx_event_id(void);
SfxEvent sfx_event_kind(void);

// cards de conceito: explicam um conteudo na primeira vez que ele acontece (#222)
typedef enum
{
    CARD_NONE = -1,
    CARD_GRAM = 0,
    CARD_RESISTANCE = 1,
    CARD_FLAGELLUM = 2,
    CARD_BIOFILM = 3,
    CARD_ENDOTOXIN = 4,
    CARD_COUNT
} ConceptCard;

// card esperando a vez; com card na tela a partida fica parada (#222)
ConceptCard pending_card(void);
void dismiss_card(void);

// onde o ultimo evento aconteceu no mundo: o main desenha a resposta ali (#221)
void sfx_event_position(float* x, float* y);

// segundos que faltam da invulnerabilidade, para o nanobo piscar (#221)
float invulnerability_left(void);

// partida: o tamanho do mundo vem das constantes, nao do main
void reset_match();
void update_match(double dt);

// fases: a run comeca na Fase 1 e os Resultados avancam para a proxima (#M4)
int phase_count(void);
int current_phase(void);
int agents_total(void);

// agentes com que a fase nasceu: as filhas da divisao ficam de fora (#217)
int agents_initial(void);
bool phase_has_bacteria();
bool has_next_phase(void);
void start_next_phase(void);

// fim da run: grava o recorde so na vitoria, quando a pontuacao supera o melhor (#M4)
void finish_run(void);
int hi_score(void);
void load_hi_score(void);

// ultimo alvo lido pelo scanner, -1 antes da primeira leitura
int last_scan_target();

#endif
