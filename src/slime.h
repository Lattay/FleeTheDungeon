#ifndef SLIME_H
#define SLIME_H

void slime_try_move(int x, int y, int dir_x, int dir_y, int* nx, int* ny);
void slime_change_dir(int* dir_x, int* dir_y);

#endif
#ifdef ENTITIES_IMPL
void slime_try_move(int x, int y, int dir_x, int dir_y, int* nx, int* ny){
  *nx = x + dir_x;
  *ny = y + dir_y;
}

void slime_change_dir(int* dir_x, int* dir_y){
  *dir_x *= -1;
  *dir_y *= -1;
}

#endif
