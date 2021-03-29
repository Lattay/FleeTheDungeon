#ifndef LEVEL_H
#define LEVEL_H
#include <stdlib.h>
#include "entities.h"

typedef enum {
  NOT_A_TILE = -1,
  WALL = 0,
  FLOOR = 1,
  DOOR = 2,
  LOCKED_DOOR = 3,
  BLOCK = 4,
  TILE_NUM,
} Tile;

typedef struct {
  int w;
  int h;
  Tile* map;
} Level;

int level_load(const char* filename, Level * data, Enemy ** enemies,
               int* lenght, int* px, int* py);
const char* level_msg_error(int code);
#endif
