#ifndef NANOBO_ENTITIES_H
#define NANOBO_ENTITIES_H

#include <stdbool.h>

// quantos agentes o encontro da Fase 1 tem: 3 bacterias Gram+, 3 Gram- e 3 celulas
#define NUM_AGENTES 9

// vagas do array de agentes: a fase nasce com ate 9 e a divisao binaria
// ocupa as vagas seguintes, uma por filha (#217)
#define MAX_AGENTS 24

// quantas fases a partida tem hoje (#M4: Fase 1 e a Fase 2 minima de virus)
#define PHASE_COUNT 2

// tamanho total do mundo em pixels: 3 telas de 640 por 2 faixas de 256
#define WORLD_W 1920
#define WORLD_H 512

// raio de desenho e de colisao dos agentes em pixels (metade do sprite de 32x32)
#define AGENT_RADIUS 16

// velocidade dos agentes da Fase 1 em pixels por segundo
#define AGENT_SPEED 30

// celula saudavel anda na metade da velocidade dos agentes: lenta e sem perseguir
#define CELL_SPEED 15

// especie do agente: define o texto do scanner e o desenho dele
typedef enum
{
    SPECIES_GRAM_POSITIVE = 0,
    SPECIES_GRAM_NEGATIVE = 1,
    SPECIES_CELL = 2,
    SPECIES_VIRUS = 3
} Species;

// forma da bacteria: a camada 1 do scanner revela so isto (#214)
// a forma sozinha nao decide a arma: a fase mistura cocos e bacilos dos dois grupos
typedef enum
{
    SHAPE_NONE = 0,
    SHAPE_COCCUS = 1,
    SHAPE_BACILLUS = 2
} Shape;

// resistencia de combate dos agentes: quantos acertos validos ate neutralizar (#49)
#define BACTERIA_RESISTANCE 2

// celula saudavel nao tem resistencia de combate: nenhum acerto a remove (#14)
#define CELL_RESISTANCE 0

// virus da Fase 2 minima: um acerto de qualquer arma neutraliza (#M4)
#define VIRUS_RESISTANCE 1

// tempo do virus parado e piscando antes do ciclo litico, em segundos (#84)
// valor de partida a validar no PoC (fase2-virus.md)
#define LYSOGENIC_TIME 12.0f

// meio periodo do piscar do virus lisogenico: opaco, depois semitransparente (#84)
#define LYSOGENIC_BLINK 0.25f

// estado do virus da Fase 2: nasce lisogenico e fica pronto para o litico (#84)
// a #85 consome o VIRUS_LYTIC_READY e abre o eclipse
typedef enum
{
    VIRUS_LYSOGENIC = 0,
    VIRUS_LYTIC_READY = 1
} VirusState;

// sprites por especie, sorteados no nascimento do agente (#211)
// o mesmo numero em todo grupo, senao a variante vira pista do Gram
#define SPRITE_VARIANTS 2

// ferramenta escolhida pelo jogador: Ruptura (Gram+), Dissolucao (Gram-) e,
// so na Fase 1, a Reserva, que elimina qualquer bacteria com uma dose (#62, #216)
typedef enum
{
    LASER_GRAM_POSITIVE = 0,
    LASER_GRAM_NEGATIVE = 1,
    LASER_RESERVE = 2
} Laser;

// quantos projeteis podem existir ao mesmo tempo
#define MAX_PROJECTILES 4

// velocidade do projetil em pixels por segundo (#63)
#define PROJECTILE_SPEED 320

// raio de desenho e de colisao do projetil em pixels
#define PROJECTILE_RADIUS 3

// estado da IA dos invasores: vagar, alerta (o nanobo perto ou tiro levado) e
// segurado (a bacteria do tutorial ate o primeiro scan) (#235)
typedef enum
{
    AI_WANDER = 0,
    AI_ALERT = 1,
    AI_HOLD = 2
} AiState;

// agente do encontro
typedef struct
{
    const char* name;
    int id;
    int resistance; // quantos acertos validos ate neutralizar (#49)
    float x;
    float y;
    float dx; // direcao em x
    float dy; // direcao em y
    int speed;
    Species species; // SPECIES_*
    int layers; // camadas que o scanner ja revelou
    bool identified;
    float dose_timer; // prazo da 2a dose; 0 sem tratamento pela metade (#215)
    bool resistant; // tratamento incompleto: o antibiotico certo nao age mais (#215)
    float direction_timer; // tempo ate a proxima mudanca de direcao
    float impulse_timer; // tempo restante do empurrao da defesa (#17)
    bool active;
    VirusState virus_state; // so vale para SPECIES_VIRUS (#84)
    float lysogenic_timer; // tempo ate o virus ficar pronto para o litico (#84)
    int generation; // 0 nos virus da fase; a eclosao soma 1 (#86)
    int host_cell; // indice da celula hospedeira, -1 sem celula (#89)
    int variant; // sprite sorteado no nascimento, de 0 a SPRITE_VARIANTS - 1 (#211)
    Shape shape; // coco ou bacilo; SHAPE_NONE fora das bacterias (#214)
    float division_timer; // tempo ate a proxima divisao binaria, so bacteria (#217)
    AiState ai_state; // vagar, alerta ou segurado (#235)
    float lost_timer; // tempo com o nanobo longe, ate perder o alerta (#235)
    float attack_timer; // tempo ate o proximo ataque (#235)
    float windup; // aviso antes do ataque; maior que 0 enquanto avisa (#235)
    float lunge_timer; // investida do bacilo em curso (#235)
    int target_cell; // celula que o coco procura ou coloniza; -1 sem alvo (#235)
    bool colonizing; // coco preso numa celula, ferindo-a (#235)
    float colonize_time; // tempo colonizando, para o biofilme (#237)
    float cell_health; // vida da celula do paciente (#235)
} Agent;

// composicao de uma fase: uma linha por agente que ela coloca no mundo (#M4)
typedef struct
{
    Species species;
    const char* name;
    int id;
    int host; // id da celula hospedeira do virus, 0 sem celula (#89)
    Shape shape; // forma da bacteria; SHAPE_NONE fora das bacterias (#214)
} PhaseAgent;

// fase jogavel: titulo, composicao e quantos agentes ela tem
typedef struct
{
    const char* title;
    const PhaseAgent* members;
    int count;
} Phase;

extern const Phase phases[PHASE_COUNT];

extern Agent agents[MAX_AGENTS];
extern int identified_count;

// raio de colisao do nanobo em pixels (metade do sprite de 32x32)
#define PLAYER_RADIUS 16

// sinais vitais do paciente: a barra do HUD vai de 0 a 100 (#218)
#define PLAYER_MAX_HEALTH 100.0f

// aceleracao e frenagem do nanobo em pixels por segundo ao quadrado (#179)
#define PLAYER_ACCEL 1200.0f
#define PLAYER_DECEL 1600.0f

// direcao para onde o nanobo olha, usada pelo disparo (#63)
typedef enum
{
    PLAYER_FACING_UP = 0,
    PLAYER_FACING_DOWN = 1,
    PLAYER_FACING_LEFT = 2,
    PLAYER_FACING_RIGHT = 3
} Facing;

// struct do player
typedef struct
{
    float x;
    float y;
    float speed; // velocidade maxima em pixels por segundo (#179)
    float vx;    // velocidade atual em x, sobe e desce pelo delta time (#179)
    float vy;    // velocidade atual em y, sobe e desce pelo delta time (#179)
    Facing facing;
    float health; // sinais vitais do paciente, de 0 a PLAYER_MAX_HEALTH (#218)
    Laser laser; // LASER_* escolhido pelo jogador (#62)
} Nanobo;

extern Nanobo player;

// projetil vivo na tela: nasce no disparo e some no impacto (#63)
typedef struct
{
    bool active;
    float x;
    float y;
    float dx; // direcao unitaria em x
    float dy; // direcao unitaria em y
    Laser laser; // raio escolhido no momento do disparo
} Projectile;

extern Projectile projectiles[MAX_PROJECTILES];

// camera: canto superior esquerdo da faixa visivel do mundo
typedef struct
{
    float x;
    float y;
} Camera;

extern Camera camera;

#endif
