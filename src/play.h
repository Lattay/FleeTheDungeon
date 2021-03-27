#ifndef LSK_PLAY_H
#define LSK_PLAY_H
#include <raylib.h>
#include "main.h"

void play_init(GameData* data);
StateName play_update();
void play_draw();
void play_suspend();

#ifdef IMPL_MAIN_LOOP
GameState play_state = {
  PLAY,
  &play_init,
  &play_update,
  &play_draw,
  &play_suspend,
};
#endif
#endif
