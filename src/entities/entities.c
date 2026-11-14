#include "entities.h"

// composicao da Fase 1: 3 bacterias Gram+, 3 Gram- e 3 celulas do paciente
// as formas se cruzam com o Gram: 2 cocos e 1 bacilo Gram+, 1 coco e 2 bacilos Gram- (#214)
// no vaso sanguineo, a celula do paciente e a do endotelio que forra o vaso
static const PhaseAgent phase_one_agents[] =
{
    { SPECIES_GRAM_POSITIVE, "Staphylococcus aureus", 1, 0, SHAPE_COCCUS },
    { SPECIES_GRAM_NEGATIVE, "Escherichia coli", 2, 0, SHAPE_BACILLUS },
    { SPECIES_CELL, "Célula endotelial", 3, 0, SHAPE_NONE },
    { SPECIES_GRAM_POSITIVE, "Streptococcus pyogenes", 4, 0, SHAPE_COCCUS },
    { SPECIES_GRAM_NEGATIVE, "Neisseria meningitidis", 5, 0, SHAPE_COCCUS },
    { SPECIES_CELL, "Célula endotelial", 6, 0, SHAPE_NONE },
    { SPECIES_GRAM_POSITIVE, "Bacillus cereus", 7, 0, SHAPE_BACILLUS },
    { SPECIES_GRAM_NEGATIVE, "Pseudomonas aeruginosa", 8, 0, SHAPE_BACILLUS },
    { SPECIES_CELL, "Célula endotelial", 9, 0, SHAPE_NONE }
};

// Fase 2: os tres virus e as celulas que eles precisam preservar (#M4)
// cada virus nasce ligado a uma celula hospedeira pelo id dela (#89)
static const PhaseAgent phase_two_agents[] =
{
    { SPECIES_VIRUS, "Vírus da fase", 1, 4 },
    { SPECIES_VIRUS, "Vírus da fase", 2, 5 },
    { SPECIES_VIRUS, "Vírus da fase", 3, 6 },
    { SPECIES_CELL, "Célula epitelial", 4, 0 },
    { SPECIES_CELL, "Célula epitelial", 5, 0 },
    { SPECIES_CELL, "Célula epitelial", 6, 0 }
};

// a ordem das fases e a ordem do jogo: a run comeca na primeira
const Phase phases[PHASE_COUNT] =
{
    { "Fase 1", phase_one_agents, NUM_AGENTES },
    { "Fase 2", phase_two_agents, 6 }
};

// o array nasce vazio: start_phase() preenche os slots da fase em cada run
Agent agents[MAX_AGENTS];

int identified_count = 0;

// camera no canto do mundo; o update_match move ela atras do nanobo
Camera camera = { 0.0f, 0.0f };

// nenhum projetil vivo antes do primeiro disparo da partida
Projectile projectiles[MAX_PROJECTILES];

// nenhum projetil inimigo antes do primeiro ataque (#236)
EnemyShot enemy_shots[MAX_ENEMY_SHOTS];

// inicializar player
Nanobo player = { .x = 960.0f, .y = 256.0f, .speed = 200.0f, .vx = 0.0f, .vy = 0.0f,
                  .facing = PLAYER_FACING_DOWN,
                  .health = PLAYER_MAX_HEALTH, .laser = LASER_GRAM_POSITIVE };
