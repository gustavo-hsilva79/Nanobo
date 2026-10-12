#ifndef NANOBO_ENTITIES_H
#define NANOBO_ENTITIES_H

#include <stdbool.h>

// quantos agentes o encontro tem
#define NUM_AGENTES 3

// agente do encontro
typedef struct
{
    int id;
    float x;
    float y;
    int variant;      // 0 = gram positiva, 1 = gram negativa, 2 = celula propria
    int layers;    // camadas que o scanner ja revelou
    bool identified;
} Agent;

extern Agent agents[NUM_AGENTES];
extern int identified_count;

// raio de colisao do nanobo em pixels (metade do sprite de 32x32)
#define PLAYER_RADIUS 16

// direcao para onde o nanobo olha, usada pelo disparo (#63)
#define PLAYER_FACING_UP 0
#define PLAYER_FACING_DOWN 1
#define PLAYER_FACING_LEFT 2
#define PLAYER_FACING_RIGHT 3

// posicao e velocidade do nanobo
extern float player_x;
extern float player_y;
extern float player_speed;

// para onde ele olha agora
extern int player_facing;

#endif
