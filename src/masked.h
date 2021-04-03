#ifndef masked_H
#define masked_H

void masked_try_move(int x, int y, int dir_x, int dir_y, int* nx, int* ny);
void masked_change_dir(int* dir_x, int* dir_y);

#endif
#ifdef ENTITIES_IMPL
void masked_try_move(int x, int y, int dir_x, int dir_y, int* nx, int* ny) {
  *nx = x + dir_x;
  *ny = y + dir_y;
}

void masked_change_dir(int* dir_x, int* dir_y) {
  *dir_x *= -1;
  *dir_y *= -1;
}

#endif
