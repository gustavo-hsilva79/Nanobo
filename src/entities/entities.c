#include "entities.h"

// composicao fixa da Fase 1: 2 bacterias Gram+, 1 Gram- e 3 celulas saudaveis
// as posicoes iniciais sao so um valor de partida, o sorteio da #25 sobrescreve
Agent agents[NUM_AGENTES] =
{
    { 1, 140, 130, SPECIES_GRAM_POSITIVE, 0, false },
    { 2, 490, 150, SPECIES_GRAM_NEGATIVE, 0, false },
    { 3, 320, 280, SPECIES_CELL, 0, false },
    { 4, 100, 330, SPECIES_GRAM_POSITIVE, 0, false },
    { 5, 300, 120, SPECIES_CELL, 0, false },
    { 6, 540, 320, SPECIES_CELL, 0, false }
};

int identified_count = 0;

// onde o nanobo esta na partida
float player_x = 320;
float player_y = 260;

// quantos pixels ele anda por segundo
float player_speed = 200;

// para onde ele olha agora
int player_facing = PLAYER_FACING_DOWN;
