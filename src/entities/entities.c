#include "entities.h"

   Agent agents[NUM_AGENTES] =
   {
       { 1, 140, 130, 0, 0, false },
       { 2, 490, 150, 1, 0, false },
       { 3, 320, 280, 2, 0, false }
   };

   int identified_count = 0;

   // onde o nanobo esta na partida
   float player_x = 320;
   float player_y = 260;

   // quantos pixels ele anda por segundo
   float player_speed = 200;

   // para onde ele olha agora
   int player_facing = PLAYER_FACING_DOWN;