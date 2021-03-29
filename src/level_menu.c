#include "level_menu.h"
#include "lsk_ui.h"

typedef struct {
  GameData* gdata;
  int n_levels;
  LSKButton levels[];
} MenuState;

MenuState state;

inline static int button_x(int i) {
  return 20 + 50 * (i % 4);
}

inline static int button_y(int i) {
  return 80 + 50 * ((int)(i / 4));
}

void level_menu_init(GameData * data) {
  state.gdata = data;
  state.n_levels = data->max_level;
  for (int i = 0; i < state.n_levels; ++i) {
    init_button(&state.levels[i], TextFormat("%d", i + 1), button_x(i),
                button_y(i), 25);
  }
}

void level_menu_draw() {
  ClearBackground(RAYWHITE);
  for (int i = 0; i < state.n_levels; ++i) {
    draw_button(&state.levels[i]);
  }
}

StateName level_menu_update() {
  for (int i = 0; i < state.n_levels; ++i) {
    if (button_pressed(&state.levels[i])) {
      state.gdata->next_level = i + 1;
      return LEVEL_TRANSITION;
    }
  }
  return LEVEL_MENU;
}

void level_menu_suspend() {
  for (int i = 0; i < state.n_levels; ++i) {
    free_button(&state.levels[i]);
  }
}
