#include "entities.h"

// composicao fixa da Fase 1: 3 bacterias Gram+, 3 Gram- e 3 celulas saudaveis
// estes sao os valores de partida: o reset_match re-sorteia posicao, direcao e
// temporizador e devolve a resistencia cheia antes do primeiro quadro de jogo
Agent agents[NUM_AGENTES] =
{
    { .id = 1, .resistance = BACTERIA_RESISTANCE, .x = 160.0f, .y = 96.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = AGENT_SPEED, .species = SPECIES_GRAM_POSITIVE, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 2, .resistance = BACTERIA_RESISTANCE, .x = 480.0f, .y = 140.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = AGENT_SPEED, .species = SPECIES_GRAM_NEGATIVE, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 3, .resistance = CELL_RESISTANCE, .x = 800.0f, .y = 96.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = CELL_SPEED, .species = SPECIES_CELL, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 4, .resistance = BACTERIA_RESISTANCE, .x = 1120.0f, .y = 140.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = AGENT_SPEED, .species = SPECIES_GRAM_POSITIVE, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 5, .resistance = BACTERIA_RESISTANCE, .x = 1440.0f, .y = 96.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = AGENT_SPEED, .species = SPECIES_GRAM_NEGATIVE, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 6, .resistance = CELL_RESISTANCE, .x = 1760.0f, .y = 140.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = CELL_SPEED, .species = SPECIES_CELL, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 7, .resistance = BACTERIA_RESISTANCE, .x = 160.0f, .y = 420.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = AGENT_SPEED, .species = SPECIES_GRAM_POSITIVE, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 8, .resistance = BACTERIA_RESISTANCE, .x = 480.0f, .y = 380.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = AGENT_SPEED, .species = SPECIES_GRAM_NEGATIVE, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true },
    { .id = 9, .resistance = CELL_RESISTANCE, .x = 800.0f, .y = 420.0f, .dx = 1.0f, .dy = 0.0f,
      .speed = CELL_SPEED, .species = SPECIES_CELL, .layers = 0, .identified = false,
      .direction_timer = 1.0f, .impulse_timer = 0.0f, .active = true }
};

int identified_count = 0;

// camera no canto do mundo; o update_match move ela atras do nanobo
Camera camera = { 0.0f, 0.0f };

// nenhum projetil vivo antes do primeiro disparo da partida
Projectile projectiles[MAX_PROJECTILES];

// inicializar player
Nanobo player = { .x = 960.0f, .y = 256.0f, .speed = 200.0f, .facing = PLAYER_FACING_DOWN,
                  .health = PLAYER_MAX_HEALTH, .laser = LASER_GRAM_POSITIVE };
