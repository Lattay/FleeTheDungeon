#ifndef LSK_LEVEL_TRANSITION_H
#define LSK_LEVEL_TRANSITION_H
#include "main.h"

void level_transition_init(GameData* data);
void level_transition_draw(void);
StateName level_transition_update(void);
void level_transition_suspend(void);

#ifdef IMPL_MAIN_LOOP
GameState level_transition_state = {
  LEVEL_TRANSITION,
  &level_transition_init, // Optional, can be NULL
  &level_transition_update,
  &level_transition_draw,
  &level_transition_suspend // Optional, can be NULL
};
#endif
#endif
