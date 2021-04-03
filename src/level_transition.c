#include "level_transition.h"

int next;

void level_transition_init(GameData* data) {
  if (data->next_level > data->max_level) {
    next = -1;
  } else {
    next = data->next_level;
  }
}

void level_transition_draw(void) {

}

StateName level_transition_update(void) {
  if (next <= 0) {
    return END;
  } else {
    return PLAY;
  }
}

void level_transition_suspend(void) {

}
