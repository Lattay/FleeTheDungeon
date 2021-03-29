#ifndef ZOMBIE_H
#define ZOMBIE_H

void zombie_try_move(int x, int y, int dir_x, int dir_y, int* nx, int* ny);
void zombie_change_dir(int* dir_x, int* dir_y);

#endif
#ifdef ENTITIES_IMPL

void zombie_try_move(int x, int y, int dir_x, int dir_y, int* nx, int* ny) {
  *nx = x + dir_x;
  *ny = y + dir_y;
}

void zombie_change_dir(int* dir_x, int* dir_y) {
  int dx = *dir_x;
  *dir_x = (*dir_y);
  *dir_y = -dx;
}

#endif
