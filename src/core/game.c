#include "game.h"

static GameScene currentScene = GAME_SCENE_MENU;
// origem do "como jogar", gravada ao entrar: a volta so vale para ela
static GameScene howtoOrigin = GAME_SCENE_MENU;

GameScene game_get_scene(void)
{
    return currentScene;
}

GameScene game_howto_origin(void)
{
    return howtoOrigin;
}

// troca a cena atual, mas so se a transicao existir no fluxo
void game_set_scene(GameScene scene)
{
    if (scene < GAME_SCENE_MENU || scene > GAME_SCENE_HOWTO)
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
            if (scene == GAME_SCENE_BRIEFING)
            {
                currentScene = scene;
            }
            else if (scene == GAME_SCENE_HOWTO)
            {
                howtoOrigin = GAME_SCENE_MENU;
                currentScene = scene;
            }
            break;

        case GAME_SCENE_BRIEFING:
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
            else if (scene == GAME_SCENE_HOWTO)
            {
                howtoOrigin = GAME_SCENE_PAUSE;
                currentScene = scene;
            }
            break;

        case GAME_SCENE_HOWTO:
            // a volta so vale para a cena de onde ele abriu
            if (scene == howtoOrigin)
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
            // briefing antes de toda partida, pulavel com ESC
            if (scene == GAME_SCENE_BRIEFING)
            {
                currentScene = scene;
            }
            break;

        case GAME_SCENE_GAME_OVER:
            // mesma regra: toda partida passa pelo briefing
            if (scene == GAME_SCENE_BRIEFING)
            {
                currentScene = scene;
            }
            break;
    }
}
