#include <raylib.h>
#include "end.h"
#include "lsk_ui.h"

static LSKButton quit_button;
static LSKButton menu_button;

void end_init(GameData* data){
  init_button(&quit_button, "Exit game", 300, 200, 20);
  init_button(&menu_button, "Go back to menu", 300, 400, 20);
}

void end_draw(){
  ClearBackground(RAYWHITE);
  draw_button(&quit_button);
  draw_button(&menu_button);
}

StateName end_update(){
  if(button_pressed(&quit_button)){
    return CLOSE;
  } else if(button_pressed(&menu_button)){
    return MENU;
  } else {
    return END;
  }
}

void end_suspend(){
  free_button(&quit_button);
  free_button(&menu_button);
}
