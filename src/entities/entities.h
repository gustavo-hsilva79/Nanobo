#ifndef NANOBO_ENTITIES_H
#define NANOBO_ENTITIES_H

#include <stdbool.h>

// quantos agentes o encontro tem: 3 bacterias e 3 celulas saudaveis na Fase 1
#define NUM_AGENTES 6

// raio de desenho e de colisao dos agentes em pixels (metade do sprite de 32x32)
#define AGENT_RADIUS 16

// especie do agente: define o texto do scanner e o desenho dele
#define SPECIES_GRAM_POSITIVE 0
#define SPECIES_GRAM_NEGATIVE 1
#define SPECIES_CELL 2

// agente do encontro
typedef struct
{
    int id;
    int health;
    float x;
    float y;
    float dx; // direcao em x
    float dy; // direcao em y
    int speed;
    int species; // SPECIES_*
    int layers; // camadas que o scanner ja revelou
    bool identified;
    float direction_timer; // tempo ate a proxima mudanca de direcao
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
} Nanobo;

extern Nanobo player;

#endif
