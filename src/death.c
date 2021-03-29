#include "death.h"

typedef struct {
  float start;
} StateData;

static StateData data;

void death_init(GameData * d) {
  data.start = GetTime();
}

void death_draw(void) {
  ClearBackground(RAYWHITE);
}

StateName death_update(void) {
  if (GetTime() - data.start > 0.5) {
    return PLAY;
  } else {
    return DEATH;
  }
}

void death_suspend(void) {

}
