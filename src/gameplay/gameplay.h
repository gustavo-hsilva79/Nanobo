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

// tela logica: o jogo desenha num bitmap deste tamanho e a janela amplia
#define SCREEN_W 640
#define SCREEN_H 360

// faixa visivel do mundo; multipla de 32, o lado de um tile do mapa futuro
#define PLAY_H 256

// painel do scanner no rodape da tela logica
#define PANEL_H 104

// o painel comeca onde a faixa visivel do mundo acaba
#define SCAN_PANEL_Y PLAY_H

// a tela e a faixa mais o painel, e tudo fecha com o tile de 32 px
_Static_assert(PANEL_H + PLAY_H == SCREEN_H, "tela = faixa do mundo + painel");
_Static_assert(SCREEN_W % 32 == 0, "largura da tela precisa ser multipla de 32");
_Static_assert(PLAY_H % 32 == 0, "faixa do mundo precisa ser multipla de 32");
_Static_assert(WORLD_W % SCREEN_W == 0, "mundo precisa ter um numero inteiro de telas");
_Static_assert(WORLD_H % PLAY_H == 0, "mundo precisa ter um numero inteiro de faixas");
_Static_assert(WORLD_W % 32 == 0 && WORLD_H % 32 == 0, "mundo precisa fechar no tile de 32");

// camadas que o scanner revela antes de identificar o agente
#define MAX_LAYERS 2

// defesa (#17): janela ativa, recarga e forca do empurrao em quem encosta
#define DEFENSE_DURATION 2.0
#define DEFENSE_COOLDOWN 3.0
#define DEFENSE_PUSH_TIME 0.5
#define DEFENSE_PUSH_MULTIPLIER 2.0f

// intervalo entre disparos (#64)
#define ATTACK_COOLDOWN 1.0

// pontos por ameaca neutralizada, concedidos uma vez por agente (#66)
#define SCORE_PER_THREAT 10

// destino da Fase 1: canto oposto ao nascimento, visivel desde o inicio (#26)
#define DESTINATION_X ((float)WORLD_W - 64.0f)
#define DESTINATION_Y ((float)WORLD_H - 64.0f)
#define DESTINATION_RADIUS 24

// bonus de identificacao completa ao concluir a fase (#67)
#define IDENTIFICATION_BONUS 15

// quanto tempo a mensagem educacional fica na tela, em segundos (#178)
#define FEEDBACK_TIME 2.5

// tempo de invulnerabilidade depois de levar dano (#11)
#define INVULNERABILITY_TIME 1.0

// tempo da partida em segundos, so conta dentro do estado GAME
extern double match_time;

// pontos da partida atual: a neutralizacao de uma ameaca soma (#66)
extern int score;

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

// mensagem educacional da ultima decisao; NULL quando nao ha nada a mostrar (#178)
const char* feedback_text();

// total de ameacas da fase, mesmo depois de neutralizadas (#19)
int threats_total();

// true quando o bonus de identificacao foi pago na conclusao (#67)
bool identification_bonus_awarded();

// true quando o disparo esta liberado agora (#19)
bool attack_ready();

// fase: ameacas ainda ativas e se a condicao de conclusao ja foi cumprida (#26)
int threats_left();
bool phase_completed();

// partida: o tamanho do mundo vem das constantes, nao do main
void reset_match();
void update_match(double dt);

// ultimo alvo lido pelo scanner, -1 antes da primeira leitura
int last_scan_target();

#endif
