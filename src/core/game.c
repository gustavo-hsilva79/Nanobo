#include "game.h"

static GameScene current_scene = GAME_SCENE_MENU;
// origem do "como jogar", gravada ao entrar: a volta so vale para ela
static GameScene howto_origin = GAME_SCENE_MENU;

GameScene game_get_scene(void)
{
    return current_scene;
}

GameScene game_howto_origin(void)
{
    return howto_origin;
}

// troca a cena atual, mas so se a transicao existir no fluxo
void game_set_scene(GameScene scene)
{
    if (scene < GAME_SCENE_MENU || scene > GAME_SCENE_HOWTO)
    {
        return;
    }

    if (scene == current_scene)
    {
        return;
    }

    switch (current_scene)
    {
        case GAME_SCENE_MENU:
            if (scene == GAME_SCENE_BRIEFING)
            {
                current_scene = scene;
            }
            else if (scene == GAME_SCENE_HOWTO)
            {
                howto_origin = GAME_SCENE_MENU;
                current_scene = scene;
            }
            break;

        case GAME_SCENE_BRIEFING:
            if (scene == GAME_SCENE_GAME)
            {
                current_scene = scene;
            }
            break;

        case GAME_SCENE_GAME:
            if (scene == GAME_SCENE_PAUSE || scene == GAME_SCENE_RESULTS || scene == GAME_SCENE_GAME_OVER)
            {
                current_scene = scene;
            }
            break;

        case GAME_SCENE_PAUSE:
            if (scene == GAME_SCENE_GAME)
            {
                current_scene = scene;
            }
            else if (scene == GAME_SCENE_HOWTO)
            {
                howto_origin = GAME_SCENE_PAUSE;
                current_scene = scene;
            }
            break;

        case GAME_SCENE_HOWTO:
            // a volta so vale para a cena de onde ele abriu
            if (scene == howto_origin)
            {
                current_scene = scene;
            }
            break;

        case GAME_SCENE_RESULTS:
            if (scene == GAME_SCENE_GAME || scene == GAME_SCENE_VICTORY)
            {
                current_scene = scene;
            }
            break;

        case GAME_SCENE_VICTORY:
            // briefing antes de toda partida, pulavel com ESC
            if (scene == GAME_SCENE_BRIEFING)
            {
                current_scene = scene;
            }
            break;

        case GAME_SCENE_GAME_OVER:
            // mesma regra: toda partida passa pelo briefing
            if (scene == GAME_SCENE_BRIEFING)
            {
                current_scene = scene;
            }
            break;
    }
}
