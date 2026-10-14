#ifndef NANOBO_SCENES_H
#define NANOBO_SCENES_H

// cenas do jogo, a ordem importa porque o game.c compara os valores
typedef enum GameScene
{
    GAME_SCENE_MENU,
    GAME_SCENE_GAME,
    GAME_SCENE_PAUSE,
    GAME_SCENE_RESULTS,
    GAME_SCENE_VICTORY,
    GAME_SCENE_GAME_OVER,
    GAME_SCENE_EXIT

} GameScene;

#endif
