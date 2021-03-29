#ifndef LSK_MENU_H
#define LSK_MENU_H
#include "main.h"

void menu_init(GameData * data);
void menu_draw(void);
StateName menu_update(void);
void menu_suspend(void);

#ifdef IMPL_MAIN_LOOP
GameState menu_state = {
  MENU,
  &menu_init,
  &menu_update,
  &menu_draw,
  &menu_suspend,
};
#endif
#endif
