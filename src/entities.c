#include <stdio.h>
#include <stdlib.h>
#include "entities.h"

#define ENTITIES_IMPL
#include "slime.h"
#include "zombie.h"

typedef struct {
  void (*try_move)(int x, int y, int dir_x, int dir_y, int* nx, int* ny);
  void (*change_dir)(int* dir_x, int* dir_y);
} EnemyType;

static EnemyType enemy_types[] = {
  {.try_move = &slime_try_move,.change_dir = &slime_change_dir},
  {.try_move = &zombie_try_move,.change_dir = &zombie_change_dir},
};

void enemy_try_move(Enemy* e, bool first_try) {
  if (!first_try) {
    enemy_types[e->type].change_dir(&e->dir_x, &e->dir_y);
  }
  enemy_types[e->type].try_move(e->x, e->y, e->dir_x, e->dir_y, &e->nx, &e->ny);
}
