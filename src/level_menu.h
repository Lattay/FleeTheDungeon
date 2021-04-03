#ifndef LEVEL_MENU_H
#define LEVEL_MENU_H
#include "main.h"

void level_menu_init(GameData* data);
void level_menu_draw(void);
StateName level_menu_update(void);
void level_menu_suspend(void);

#ifdef IMPL_MAIN_LOOP
GameState level_menu_state = {
  LEVEL_MENU,
  &level_menu_init, // Optional, can be NULL
  &level_menu_update,
  &level_menu_draw,
  &level_menu_suspend // Optional, can be NULL
};
#endif
#endif
