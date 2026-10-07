#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#ifndef NANOBO_SCENES_H
#define NANOBO_SCENES_H

typedef enum GameScene
{
    GAME_SCENE_MENU,
    GAME_SCENE_GAME,
    GAME_SCENE_PAUSE,
    GAME_SCENE_RESULTS,
    GAME_SCENE_VICTORY,
    GAME_SCENE_GAME_OVER,

} GameScene;


// Estrutura para gerenciar o estado que o jogo esta
typedef struct
{
    GameScene current_scene;
    bool running;

} GameState;

// Cria uma função para cada cena
static void scane_menu_updade(GameState *state)
{
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_KEYBOARD_STATE key_state;
    al_get_keyboard_state(&key_state);

    if (al_key_down(&key_state, ALLEGRO_KEY_ENTER)) 
    {
        state->current_scene = GAME_SCENE_GAME;
    }
    if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE)) 
    {
        state->current_scene = GAME_SCENE_GAME_OVER;
    }

    al_destroy_event_queue(queue);
}


#endif
