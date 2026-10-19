#include "entities.h"

// composicao fixa da Fase 1: 2 bacterias Gram+, 1 Gram- e 3 celulas saudaveis
// estes sao os valores de partida: o reset_match re-sorteia posicao, direcao e
// temporizador e devolve a resistencia cheia antes do primeiro quadro de jogo
Agent agents[NUM_AGENTES] =
{
    { .id = 1, .resistance = BACTERIA_RESISTANCE, .x = 140.0f, .y = 130.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = AGENT_SPEED, .species = SPECIES_GRAM_POSITIVE, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 2, .resistance = BACTERIA_RESISTANCE, .x = 490.0f, .y = 150.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = AGENT_SPEED, .species = SPECIES_GRAM_NEGATIVE, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 3, .resistance = CELL_RESISTANCE, .x = 320.0f, .y = 280.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = CELL_SPEED, .species = SPECIES_CELL, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 4, .resistance = BACTERIA_RESISTANCE, .x = 100.0f, .y = 330.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = AGENT_SPEED, .species = SPECIES_GRAM_POSITIVE, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 5, .resistance = CELL_RESISTANCE, .x = 300.0f, .y = 120.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = CELL_SPEED, .species = SPECIES_CELL, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 6, .resistance = CELL_RESISTANCE, .x = 540.0f, .y = 320.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = CELL_SPEED, .species = SPECIES_CELL, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true }
};

int identified_count = 0;

// nenhum projetil vivo antes do primeiro disparo da partida
Projectile projectiles[MAX_PROJECTILES];

// inicializar player
Nanobo player = { .x = 320.0f, .y = 260.0f, .speed = 200.0f, .facing = PLAYER_FACING_DOWN,
                  .health = PLAYER_MAX_HEALTH, .laser = LASER_GRAM_POSITIVE };
