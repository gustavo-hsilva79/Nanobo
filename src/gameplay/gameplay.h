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

// arma errada: trava o disparo e poe a bacteria atingida atras do nanobo (RF-06)
#define FIRE_LOCK_TIME 2.0
#define CHASE_SPEED 60

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
} MatchProgress;

extern MatchProgress progress;

// totais acumulados da run, para as telas de Vitoria e Game Over (#M4)
typedef struct
{
    int threats;     // ameacas das fases ja jogadas
    int neutralized; // ameacas neutralizadas na run
    int agents;      // agentes das fases ja jogadas
    int identified;  // agentes identificados na run
} RunProgress;

extern RunProgress run_progress;

// pontos da partida atual: a neutralizacao de uma ameaca soma (#66)
extern int score;

// teclas seguradas agora, o main escreve isso na entrada e o update le pra mover
extern bool key_up;
extern bool key_down;
extern bool key_left;
extern bool key_right;

// leitura do scanner: camada 1 = coloracao + nome; camada 2 = grupo + estrutura
const char* layer_one_text(Species species, const char* name);
const char* layer_two_text(Species species);
const char* layer_two_reason(Species species);
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

// modulo A e B: a roda do mouse alterna, 1 e 2 escolhem direto (#198)
void toggle_module();

// contato: index do agente que o nanobo esta encostando, -1 quando nenhum
int touching_agent();

// acerto valido no agente: devolve true quando esse acerto o neutralizou (#49)
bool hit_agent(int index);

// mensagem educacional da ultima decisao; NULL quando nao ha nada a mostrar (#178)
const char* feedback_text();

// total de ameacas da fase, mesmo depois de neutralizadas (#19)
int threats_total();

// true quando o bonus de identificacao foi pago na conclusao (#67)
bool identification_bonus_awarded();

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

// partida: o tamanho do mundo vem das constantes, nao do main
void reset_match();
void update_match(double dt);

// fases: a run comeca na Fase 1 e os Resultados avancam para a proxima (#M4)
int phase_count(void);
int current_phase(void);
int agents_total(void);
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
