#include <stdlib.h>
#include <raylib.h>
#include "menu.h"
#include "lsk_ui.h"

typedef struct {
  LSKButton play_button;
  LSKButton about_button;
  LSKLabel title;
} MenuData;

static MenuData* data;

void menu_init(GameData* gdata){
  data = malloc(sizeof(MenuData));
  init_button(&data->play_button, "Play", 300, 400, 20);
  init_button(&data->about_button, "About this game", 300, 500, 20);
  init_label(&data->title, "Flee The Dungeon", 300, 200, 30);
}

void draw_title(const char* title, int x, int y){
  int width = MeasureText(title, 30);
  DrawText(title, x - width/2, y, 30, BLACK);
}

void menu_draw(void){
  ClearBackground(RAYWHITE);
  draw_label(&data->title);
  draw_button(&data->play_button);
  draw_button(&data->about_button);
}

StateName menu_update(void){
  if (button_pressed(&data->play_button)){
    return LEVEL_MENU;
  } else if (button_pressed(&data->about_button)){
    return CREDIT;
  }
  return MENU;
}

void menu_suspend(void){
  free_label(&data->title);
  free_button(&data->play_button);
  free_button(&data->about_button);
  free(data);
  data = NULL;
}
