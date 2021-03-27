#ifndef LSK_CREDIT_H
#define LSK_CREDIT_H
#include "main.h"

void credit_init(GameData* data);
void credit_draw(void);
StateName credit_update(void);
void credit_suspend(void);

#ifdef IMPL_MAIN_LOOP
GameState credit_state = {
  CREDIT,
  &credit_init,
  &credit_update,
  &credit_draw,
  &credit_suspend
};
#endif
#endif
