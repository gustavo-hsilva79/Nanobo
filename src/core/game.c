#include "game.h"

static GameScene currentScene = GAME_SCENE_MENU;

GameScene game_get_scene(void)
{
    return currentScene;
}

void game_set_scene(GameScene scene)
{
    if (scene < GAME_SCENE_MENU || scene > GAME_SCENE_GAME_OVER)
    {
        return;
    }

    currentScene = scene;
}
