#include "entities.h"

// composicao fixa da Fase 1: 2 bacterias Gram+, 1 Gram- e 3 celulas saudaveis
// estes sao os valores de partida: o reset_match re-sorteia posicao, direcao e
// temporizador e devolve a resistencia cheia antes do primeiro quadro de jogo
Agent agents[NUM_AGENTES] =
{
    // id, resistencia, x, y, dx, dy, speed, species, layers, identified, direction_timer, active
    { 1, BACTERIA_RESISTANCE, 140, 130, 1, 0, AGENT_SPEED, SPECIES_GRAM_POSITIVE, 0, false, 1, true },
    { 2, BACTERIA_RESISTANCE, 490, 150, 1, 0, AGENT_SPEED, SPECIES_GRAM_NEGATIVE, 0, false, 1, true },
    { 3, CELL_RESISTANCE, 320, 280, 1, 0, AGENT_SPEED, SPECIES_CELL, 0, false, 1, true },
    { 4, BACTERIA_RESISTANCE, 100, 330, 1, 0, AGENT_SPEED, SPECIES_GRAM_POSITIVE, 0, false, 1, true },
    { 5, CELL_RESISTANCE, 300, 120, 1, 0, AGENT_SPEED, SPECIES_CELL, 0, false, 1, true },
    { 6, CELL_RESISTANCE, 540, 320, 1, 0, AGENT_SPEED, SPECIES_CELL, 0, false, 1, true }
};

int identified_count = 0;

// inicializar player
Nanobo player = { 320, 260, 200, PLAYER_FACING_DOWN, PLAYER_MAX_HEALTH };

