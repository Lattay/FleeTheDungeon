#ifndef LSK_UI_H
#define LSK_UI_H
#include <raylib.h>

typedef struct {
  char* text;
  int fontsize;
  Vector2 position;
  Rectangle box;
  bool disabled;
} LSKButton;

void init_button(LSKButton * but, const char* text, int x, int y, int fontsize);
void draw_button(LSKButton * but);
bool button_pressed(LSKButton * but);
void free_button(LSKButton * but);

typedef struct {
  char* text;
  int fontsize;
  Vector2 position;
  bool disabled;
} LSKLabel;

void init_label(LSKLabel * lab, const char* text, int x, int y, int fontsize);
void draw_label(LSKLabel * lab);
void free_label(LSKLabel * lab);
void set_label(LSKLabel * lab, const char* text);

/* A label whose content is managed by the user */
typedef struct {
  char* text;
  int fontsize;
  Vector2 position;
  int half_width;
  bool changed;
  bool disabled;
} LSKManagedLabel;

void init_mlabel(LSKManagedLabel * lab, int x, int y, int fontsize);
void draw_mlabel(LSKManagedLabel * lab);
void content_changed_mlabel(LSKManagedLabel * lab);
void set_content_mlabel(LSKManagedLabel * lab, char* content);

#endif
