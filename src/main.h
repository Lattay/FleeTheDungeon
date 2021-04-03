#ifndef LSK_H
#define LSK_H
#include <stdbool.h>
#include <math.h>
#include "lsk_basics.h"
#include "lsk_camera.h"

typedef enum {
  MENU = 0,
  CREDIT,
  PLAY,
  DEATH,
  LEVEL_TRANSITION,
  LEVEL_MENU,
  END,
  STATE_NUM,  // Not a state, but gives the number of states
  CLOSE,  // Special state, used to close the main loop
} StateName;

typedef struct {
  LSKCamera cam;
  int next_level;
  int max_level;
} GameData;

typedef struct {
  StateName name;
  void (*init)(GameData* data); // optional
    StateName(*update) (void);
  void (*draw)(void);
  void (*suspend)(void);  // optional
  // void (*free)(); // optional
} GameState;

#ifdef IMPL_MAIN_LOOP
// Add new state headers here:
#include "menu.h"
#include "credit.h"
#include "play.h"
#include "death.h"
#include "level_transition.h"
#include "level_menu.h"
#include "end.h"

int screenWidth = 600;
int screenHeight = 600;

// Add new GameState pointers here:
GameState* states[(int)STATE_NUM] = {
  &menu_state,
  &credit_state,
  &play_state,
  &death_state,
  &level_transition_state,
  &level_menu_state,
  &end_state
};
#endif
#endif
