#ifndef NANOBO_GAME_H
#define NANOBO_GAME_H

#include "../scenes/scenes.h"

GameScene game_get_scene(void);
void game_set_scene(GameScene scene);

// cena de onde o "como jogar" abriu (menu ou pausa)
GameScene game_howto_origin(void);

#endif
