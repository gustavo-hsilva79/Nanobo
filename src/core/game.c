#include "game.h"

static GameScene currentScene = GAME_SCENE_MENU;

GameScene game_get_scene(void)
{
    return currentScene;
}

// troca a cena atual, mas so se a transicao existir no fluxo
void game_set_scene(GameScene scene)
{
    if (scene < GAME_SCENE_MENU || scene > GAME_SCENE_GAME_OVER)
    {
        return;
    }

    if (scene == currentScene)
    {
        return;
    }

    switch (currentScene)
    {
        case GAME_SCENE_MENU:
            if (scene == GAME_SCENE_GAME)
            {
                currentScene = scene;
            }
            break;

        case GAME_SCENE_GAME:
            if (scene == GAME_SCENE_PAUSE || scene == GAME_SCENE_RESULTS || scene == GAME_SCENE_GAME_OVER)
            {
                currentScene = scene;
            }
            break;

        case GAME_SCENE_PAUSE:
            if (scene == GAME_SCENE_GAME)
            {
                currentScene = scene;
            }
            break;

        case GAME_SCENE_RESULTS:
            if (scene == GAME_SCENE_GAME || scene == GAME_SCENE_VICTORY)
            {
                currentScene = scene;
            }
            break;

        case GAME_SCENE_VICTORY:
            if (scene == GAME_SCENE_GAME)
            {
                currentScene = scene;
            }
            break;

        case GAME_SCENE_GAME_OVER:
            if (scene == GAME_SCENE_GAME)
            {
                currentScene = scene;
            }
            break;
    }
}
