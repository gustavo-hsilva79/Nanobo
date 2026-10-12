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

// posicao e velocidade do nanobo
extern float player_x;
extern float player_y;
extern float player_speed;

#endif
