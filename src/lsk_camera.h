#ifndef LSK_CAMERA_H
#define LSK_CAMERA_H
#include <raylib.h>

typedef struct {
  int width, height;
  int x, y;
  float scale;
  int h_band;
  int v_band;
  Color background;
} LSKCamera;

void init_camera(LSKCamera* cam, int width, int height);
void update_camera(LSKCamera* cam);
void camera_set_offset(LSKCamera* cam, int x, int y);

void camera_clear_background(LSKCamera* cam, Color background);
void camera_draw_rectangle_v(LSKCamera* cam, Vector2 pos, Vector2 size, Color col);
void camera_draw_centered_rect_v(LSKCamera* cam, Vector2 pos, Vector2 size, Color col);
void camera_draw_circle_v(LSKCamera* cam, Vector2 pos, float radius, Color col);

#endif
