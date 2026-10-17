#ifndef NANOBO_ENTITIES_H
#define NANOBO_ENTITIES_H

#include <stdbool.h>

// quantos agentes o encontro tem: 3 bacterias e 3 celulas saudaveis na Fase 1
#define NUM_AGENTES 6

// raio de desenho e de colisao dos agentes em pixels (metade do sprite de 32x32)
#define AGENT_RADIUS 16

// velocidade dos agentes da Fase 1 em pixels por segundo
#define AGENT_SPEED 30

// especie do agente: define o texto do scanner e o desenho dele
#define SPECIES_GRAM_POSITIVE 0
#define SPECIES_GRAM_NEGATIVE 1
#define SPECIES_CELL 2

// resistencia de combate dos agentes: quantos acertos validos ate neutralizar (#49)
#define BACTERIA_RESISTANCE 2

// celula saudavel nao tem resistencia de combate: nenhum acerto a remove (#14)
#define CELL_RESISTANCE 0

// raio escolhido pelo jogador: roxo para Gram+, vermelho para Gram- (#62)
#define LASER_GRAM_POSITIVE 0
#define LASER_GRAM_NEGATIVE 1

// quantos projeteis podem existir ao mesmo tempo
#define MAX_PROJECTILES 4

// velocidade do projetil em pixels por segundo (#63)
#define PROJECTILE_SPEED 320

// raio de desenho e de colisao do projetil em pixels
#define PROJECTILE_RADIUS 3

// agente do encontro
typedef struct
{
    int id;
    int resistance; // quantos acertos validos ate neutralizar (#49)
    float x;
    float y;
    float dx; // direcao em x
    float dy; // direcao em y
    int speed;
    int species; // SPECIES_*
    int layers; // camadas que o scanner ja revelou
    bool identified;
    float direction_timer; // tempo ate a proxima mudanca de direcao
    float impulse_timer; // tempo restante do empurrao da defesa (#17)
    bool active;
} Agent;

extern Agent agents[NUM_AGENTES];
extern int identified_count;

// raio de colisao do nanobo em pixels (metade do sprite de 32x32)
#define PLAYER_RADIUS 16

// vida maxima do player
#define PLAYER_MAX_HEALTH 3

// direcao para onde o nanobo olha, usada pelo disparo (#63)
#define PLAYER_FACING_UP 0
#define PLAYER_FACING_DOWN 1
#define PLAYER_FACING_LEFT 2
#define PLAYER_FACING_RIGHT 3

// struct do player
typedef struct
{
    float x;
    float y;
    float speed;
    int facing;
    int health;
    int laser; // LASER_* escolhido pelo jogador (#62)
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
    int laser; // raio escolhido no momento do disparo
} Projectile;

extern Projectile projectiles[MAX_PROJECTILES];

#endif
