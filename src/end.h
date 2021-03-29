#ifndef LSK_END_H
#define LSK_END_H
#include "main.h"

void end_init(GameData * data);
void end_draw(void);
StateName end_update(void);
void end_suspend(void);

#ifdef IMPL_MAIN_LOOP
GameState end_state = {
  END,
  &end_init,
  &end_update,
  &end_draw,
  &end_suspend
};
#endif
#endif
