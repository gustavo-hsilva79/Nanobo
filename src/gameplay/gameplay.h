#ifndef NANOBO_GAMEPLAY_H
#define NANOBO_GAMEPLAY_H

#include <stdbool.h>

// as regras falam das especies e direcoes do jogo
#include "../entities/entities.h"

// o projeto compila com /std:c11; sem isso o arquivo nao compila de proposito
_Static_assert(__STDC_VERSION__ >= 201112L, "compilar com /std:c11 (ver Nanobo.vcxproj)");

// regras centrais de gameplay ficam declaradas aqui
// (alvo do scanner, camadas, vida, dano, cooldown, pontuacao e fim de fase)

// alcance do scanner em pixels
#define SCAN_RANGE 130

// linha onde comeca o painel do scanner no desenho; a area dos agentes acaba aqui
#define SCAN_PANEL_Y 370

// camadas que o scanner revela antes de identificar o agente
#define MAX_LAYERS 2

// defesa (#17): janela ativa, recarga e forca do empurrao em quem encosta
#define DEFENSE_DURATION 2.0
#define DEFENSE_COOLDOWN 3.0
#define DEFENSE_PUSH_TIME 0.5
#define DEFENSE_PUSH_MULTIPLIER 2.0f

// intervalo entre disparos (#64)
#define ATTACK_COOLDOWN 1.0

// tempo de invulnerabilidade depois de levar dano (#11)
#define INVULNERABILITY_TIME 1.0

// tempo da partida em segundos, so conta dentro do estado GAME
extern double match_time;

// teclas seguradas agora, o main escreve isso na entrada e o update le pra mover
extern bool key_up;
extern bool key_down;
extern bool key_left;
extern bool key_right;

// leitura do scanner
const char* layer_one_text(Species species);
const char* layer_two_text(Species species);
int pick_target(float x, float y);
void use_scanner();

// defesa: ativa, estado pro anel do HUD e empurrao de quem encosta (#17)
void activate_defense();
int defense_state();

// ataque: um projetil por aperto, respeitando o cooldown (#63, #64)
void shoot();

// contato: index do agente que o nanobo esta encostando, -1 quando nenhum
int touching_agent();

// acerto valido no agente: devolve true quando esse acerto o neutralizou (#49)
bool hit_agent(int index);

// partida
void reset_match();
void update_match(double dt, float screen_w, float screen_h);

#endif
