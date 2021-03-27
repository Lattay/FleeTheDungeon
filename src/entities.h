#ifndef ENTITIES_H
#define ENTITIES_H
#include <stdbool.h>

typedef struct {
  int type;
  int x;
  int y;
  int nx;
  int ny;
  int dir_x;
  int dir_y;
} Enemy;

void enemy_try_move(Enemy* e, bool first_try);

#endif
