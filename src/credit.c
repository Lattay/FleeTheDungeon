#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include "credit.h"

static Rectangle box  = {20, 20, 560, 560 };
static char* text;


void credit_init(GameData* data){
  FILE* f = fopen("credit.txt", "r");
  fseek(f, 0, SEEK_END);
  int length = ftell(f);
  text = malloc((length + 1)* sizeof(char));
  rewind(f);
  fread(text, 1, length, f);
  text[length] = 0;
}

void credit_draw(void){
  ClearBackground(RAYWHITE);
  DrawTextRec(GetFontDefault(), text, box, 16, 2, false, BLACK);
}

StateName credit_update(void){
  if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    return MENU;
  } else {
    return CREDIT;
  }
}

void credit_suspend(void){
  free(text);
}
