#ifndef NANOBO_GAMEPLAY_H
#define NANOBO_GAMEPLAY_H

#include <stdbool.h>

// regras centrais de gameplay ficam declaradas aqui
// (alvo do scanner, camadas, vida, dano, cooldown, pontuacao e fim de fase)

// alcance do scanner em pixels
#define SCAN_RANGE 130

// camadas que o scanner revela antes de identificar o agente
#define MAX_LAYERS 2

// tempo da partida em segundos, so conta dentro do estado GAME
extern double match_time;

// teclas seguradas agora, o main escreve isso na entrada e o update le pra mover
extern bool key_up;
extern bool key_down;
extern bool key_left;
extern bool key_right;

// leitura do scanner
const char* layer_one_text(int kind);
const char* layer_two_text(int kind);
int pick_target(float x, float y);
void use_scanner();

// partida
void reset_match();
void update_match(double dt, float screen_w, float screen_h);

// sorteia as posicoes dos agentes no comeco de cada partida
void place_agents();

#endif
