#include "lsk_camera.h"

void init_camera(LSKCamera* cam, int width, int height){
  cam->width = width;
  cam->height = height;
  cam->x = 0;
  cam->y = 0;
  cam->background = BLACK;
  update_camera(cam);
}

void update_camera(LSKCamera* cam){
  int swidth = GetScreenWidth();
  int sheight = GetScreenHeight();

  float cam_prop = ((float)cam->height)/((float)cam->width);
  float screen_prop = ((float)sheight)/((float)swidth);

  if(cam_prop > screen_prop){
    cam->h_band = 0;
    cam->scale = ((float)sheight)/((float)cam->height);
    cam->v_band = 0.5 * (swidth - cam->width * cam->scale);
  } else {
    cam->v_band = 0;
    cam->scale = ((float)swidth)/((float)cam->width);
    cam->h_band = 0.5 * (sheight - cam->height * cam->scale);
  }
}

void camera_set_offset(LSKCamera* cam, int x, int y){
  cam->x = x;
  cam->y = y;
}

void camera_clear_background(LSKCamera* cam, Color background){
  ClearBackground(cam->background);
  DrawRectangle(cam->v_band, cam->h_band, cam->width * cam->scale, cam->height * cam->scale, background);
}

void camera_draw_rectangle(LSKCamera* cam, Vector2 pos, Vector2 size, Color col){
  DrawRectangle(
      cam->v_band + (pos.x - cam->x) * cam->scale,
      cam->h_band + (pos.y - cam->y) * cam->scale,
      cam->scale * size.x,
      cam->scale * size.y,
      col
  );
}

void camera_draw_rectangle_c(LSKCamera* cam, Vector2 pos, Vector2 size, Color col){
  DrawRectangle(
      cam->v_band + (pos.x - cam->x - 0.5 * size.x) * cam->scale,
      cam->h_band + (pos.y - cam->y - 0.5 * size.y) * cam->scale,
      cam->scale * size.x,
      cam->scale * size.y,
      col
  );
}

void camera_draw_circle(LSKCamera* cam, Vector2 pos, float radius, Color col){
  DrawCircle(
      cam->v_band + (pos.x - cam->x) * cam->scale,
      cam->h_band + (pos.y - cam->y) * cam->scale,
      cam->scale * radius,
      col
  );
}
