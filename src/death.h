#ifndef LSK_DEATH_H
#define LSK_DEATH_H
#include "main.h"

void death_init(GameData * data);
void death_draw(void);
StateName death_update(void);
void death_suspend(void);

#ifdef IMPL_MAIN_LOOP
GameState death_state = {
  DEATH,
  &death_init,  // Optional, can be NULL
  &death_update,
  &death_draw,
  &death_suspend  // Optional, can be NULL
};
#endif
#endif
