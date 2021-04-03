#include <stdio.h>
#include "level.h"
#include <raylib.h>

const char* level_msg_error(int code) {
  switch (code) {
    case 1:
      return "File not found.";
    case 2:
      return "Syntax error.";
    default:
      return "";
  }
}

static void eat_spaces(FILE* f) {
  int c;
  do {
    c = fgetc(f);
  } while (c == ' ' || c == '\n' || c == '\r' || c == '\t');
  if (c != EOF) {
    ungetc(c, f);
  }
}

int level_load(const char* filename, Level* data, Enemy** enemies,
               int* length, int* px, int* py) {
  int res;
  FILE* f = fopen(filename, "r");
  if (!f) {
    return 1;
  }

  res = fscanf(f, "PLAYER %d %d", px, py);
  if (res <= 0) {
    fclose(f);
    return 2;
  }

  TraceLog(LOG_DEBUG, TextFormat("Player at (%d, %d)", *px, *py));
  eat_spaces(f);

  res = fscanf(f, "ENEMIES %d", length);
  if (res <= 0) {
    fclose(f);
    return 2;
  }
  TraceLog(LOG_DEBUG, TextFormat("There are %d enemies", *length));
  eat_spaces(f);

  *enemies = malloc(sizeof(Enemy) * *length);

  for (int i = 0; i < *length; ++i) {
    Enemy* e = &((*enemies)[i]);
    res =
        fscanf(f, "ENEMY %d (%d, %d)>(%d, %d)", &e->type, &e->x, &e->y,
               &e->dir_x, &e->dir_y);
    if (res <= 0) {
      fclose(f);
      free(*enemies);
      return 2;
    }
    TraceLog(LOG_DEBUG, TextFormat("Enemy at (%d, %d)", e->x, e->y));
    e->nx = e->x;
    e->ny = e->y;
    eat_spaces(f);
  }

  res = fscanf(f, "MAP %d %d", &data->w, &data->h);
  if (res <= 0) {
    fclose(f);
    free(*enemies);
    return 2;
  }
  eat_spaces(f);

  Tile* map = malloc(sizeof(Tile) * data->w * data->h);

  for (int y = 0; y < data->h; ++y) {
    for (int x = 0; x < data->w; ++x) {
      int d;
      res = fscanf(f, "%d", &d);
      if (res <= 0) {
        fclose(f);
        free(*enemies);
        free(map);
        return 2;
      }

      eat_spaces(f);

      if (d <= NOT_A_TILE || d >= TILE_NUM) {
        map[x + data->w * y] = WALL;
      } else {
        map[x + data->w * y] = d;
      }
    }
  }

  data->map = map;

  res = fclose(f);
  return res == 0 ? 0 : 3;
}
