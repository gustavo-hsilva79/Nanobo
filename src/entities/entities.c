#include "entities.h"

// composicao fixa da Fase 1: 2 bacterias Gram+, 1 Gram- e 3 celulas saudaveis
// as posicoes iniciais sao so um valor de partida, o sorteio da #25 sobrescreve
Agent agents[NUM_AGENTES] =
{
    // id, health, x, y, dx, dy, speed, species, layers, identified, direction_timer, active
    { 1, 2, 140, 130, 1, 1, 30, SPECIES_GRAM_POSITIVE, 0, false, 0, true },
    { 2, 2, 490, 150, 1, 1, 30, SPECIES_GRAM_NEGATIVE, 0, false, 0, true },
    { 3, 2, 320, 280, 1, 1, 30, SPECIES_CELL, 0, false, 0, true },
    { 4, 2, 100, 330, 1, 1, 30, SPECIES_GRAM_POSITIVE, 0, false, 0, true },
    { 5, 2, 300, 120, 1, 1, 30, SPECIES_CELL, 0, false, 0, true },
    { 6, 2, 540, 320, 1, 1, 30, SPECIES_CELL, 0, false, 0, true }
};

int identified_count = 0;

// inicializar player
Nanobo player = { 320, 260, 200, PLAYER_FACING_DOWN, PLAYER_MAX_HEALTH };

