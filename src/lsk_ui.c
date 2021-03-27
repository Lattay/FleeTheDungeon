#include <stdlib.h>
#include "lsk_ui.h"

void init_button(LSKButton* but, const char* text, int x, int y, int fontsize){
  int l = 0;
  while(text[l++] != 0);
  but->text = malloc(l * sizeof(char));
  for(int i = 0; i < l; i++)
    but->text[i] = text[i];
  int width = MeasureText(text, fontsize);
  but->position.y = y;
  but->position.x = x - width/2;
  but->box.x = x - 5 - width/2;
  but->box.y = y - 5;
  but->box.width = width + 10;
  but->box.height = 10 + fontsize;
  but->fontsize = fontsize;
  but->disabled = false;
}

void draw_button(LSKButton* but){
  if(but->disabled) return;
  DrawRectangleRec(but->box, BLACK);
  DrawText(but->text, but->position.x, but->position.y, but->fontsize, RAYWHITE);
}

bool button_pressed(LSKButton* but){
  if(but->disabled) return false;
  return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), but->box);
}

void free_button(LSKButton* but){
  free(but->text);
}

void init_label(LSKLabel* lab, const char* text, int x, int y, int fontsize){
  int l = 0;
  while(text[l++] != 0);
  lab->text = malloc(l * sizeof(char));
  for(int i = 0; i < l; i++)
    lab->text[i] = text[i];
  int width = MeasureText(text, fontsize);
  lab->position.y = y;
  lab->position.x = x - width/2;
  lab->fontsize = fontsize;
  lab->disabled = false;
}

void draw_label(LSKLabel* lab){
  if(lab->disabled) return;
  DrawText(lab->text, lab->position.x, lab->position.y, lab->fontsize, BLACK);
}

void set_label(LSKLabel* lab, const char* text){
  int w0 = MeasureText(lab->text, lab->fontsize);
  
  /* reallocate the space to fit */
  int l = 0;
  while(text[l++] != 0);
  lab->text = realloc(lab->text, l * sizeof(char));

  /* copy data */
  for(int i = 0; i < l; i++)
    lab->text[i] = text[i];

  /* recenter */
  int width = MeasureText(text, lab->fontsize);
  lab->position.x = lab->position.x + w0/2 - width/2;
}

void free_label(LSKLabel* lab){
  free(lab->text);
}

void init_mlabel(LSKManagedLabel* lab, int x, int y, int fontsize){
  lab->text = NULL;
  lab->fontsize = fontsize;
  lab->position = (Vector2){x, y};
  lab->half_width = 0;
  lab->changed = false;
  lab->disabled = false;
}

void draw_mlabel(LSKManagedLabel* lab){
  if(lab->disabled)
    return;
  if(lab->changed){
    lab->changed = false;
    lab->half_width = MeasureText(lab->text, lab->fontsize) / 2.0;
  }
  DrawText(lab->text, lab->position.x - lab->half_width, lab->position.y, lab->fontsize, BLACK);
}

void content_changed_mlabel(LSKManagedLabel* lab){
  lab->changed = true;
}

void set_content_mlabel(LSKManagedLabel* lab, char* content){
  content_changed_mlabel(lab);
  lab->text = content;
}
