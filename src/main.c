/*
 * Neutron
 * A simple raylib based game implementing the Neutron abstract strategy game
 *
 */

#include <stdlib.h>
#include <raylib.h>

#define IMPL_MAIN_LOOP
#include "main.h"

int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------

  InitWindow(screenWidth, screenHeight, "Starter kit");

  StateName new_state;
  GameState* state = &menu_state;

  GameData game_data;
  game_data.max_level = 3;
  init_camera(&game_data.cam, 600, 600);

  state->init(&game_data);
  bool window_should_close = false;

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //---------------------------------------------------------------------------------------

  // Main game loop
  while (!window_should_close)  // Detect window close button or ESC key
  {
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    state->draw();

    EndDrawing();

    // Update
    //----------------------------------------------------------------------------------
    new_state = state->update();

    if (new_state != state->name) {
      if (state->suspend) {
        state->suspend();
      }
      if (new_state != CLOSE) {
        for (int i = 0; i < STATE_NUM; i++) {
          if (new_state == states[i]->name) {
            state = states[i];
            break;
          }
        }
        if (state->init) {
          state->init(&game_data);
        }
      }
    }
    //----------------------------------------------------------------------------------
    window_should_close = WindowShouldClose() || new_state == CLOSE;
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
