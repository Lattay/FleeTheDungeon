#include <stdlib.h>
#include "play.h"
#include "lsk_ui.h"
#include "level.h"
#include "entities.h"
#include "string.h"

static const int W = 600;
static const int H = 600;
int ox = 50;
int oy = 70;

typedef enum {
  IDLE = 0,
  RUN,
  CREATE_BLOCK,
  ENEMY_TURN,
  ENEMY_TURN_ANIM,
  DIE,
  WIN,
} PlayState;

typedef struct {
  GameData* gdata;

  /* player data */
  int x;
  int y;
  int nx;
  int ny;

  /* enemies data */
  int n_enemies;
  Enemy* enemies;

  /* general data */
  Level level;
  PlayState state;
  int turn;
  float anim_t;
  LSKLabel* hints;
  LSKLabel* turn_info;
  int map_ox;
  int map_oy;
} PlayData;

static inline Tile get_tile(Level * lvl, int x, int y) {
  return lvl->map[x + lvl->w * y];
}

static inline void set_tile(Level * lvl, int x, int y, Tile t) {
  lvl->map[x + lvl->w * y] = t;
}

static PlayData* data;

static void set_offset() {
  if (data->state == RUN && data->anim_t != 0) {
    float x = lerp(data->x, data->nx, data->anim_t);
    float y = lerp(data->y, data->ny, data->anim_t);
    data->map_ox = x * 50 + ox + 25 - W / 2;
    data->map_oy = y * 50 + oy + 25 - H / 2;
  } else {
    data->map_ox = data->x * 50 + ox + 25 - W / 2;
    data->map_oy = data->y * 50 + oy + 25 - H / 2;
  }
}

static void load_level(int level_num) {
  char layout_name[16];
  strcpy(layout_name, TextFormat("levels/l%03d.txt", level_num));
  int err =
      level_load(layout_name, &data->level, &data->enemies, &data->n_enemies,
                 &data->x, &data->y);

  if (err) {
    TraceLog(LOG_ERROR,
             TextFormat("Cannot load level %d (%s): %s", level_num, layout_name,
                        level_msg_error(err)));
    exit(1);
  }
  TraceLog(LOG_INFO, TextFormat("Successfully loaded level %d", level_num));
}

void play_init(GameData * gdata) {

  SetTraceLogLevel(LOG_DEBUG);

  data = malloc(sizeof(PlayData));
  data->gdata = gdata;

  /* load level */
  load_level(gdata->next_level);

  set_offset();

  /* setup general data */
  data->anim_t = 0;
  data->state = IDLE;
  data->turn = 0;
  data->hints = malloc(sizeof(LSKLabel));
  data->turn_info = malloc(sizeof(LSKManagedLabel));
  init_label(data->hints,
             "WASD to move, left click to create a block, R to reset the level",
             W / 2, 10, 15);
  init_label(data->turn_info, "Turn: 0", W / 2, 30, 20);
}

void play_suspend() {
  TraceLog(LOG_DEBUG, "Freeing play state.");
  free(data->enemies);
  free(data);
  data = NULL;
  TraceLog(LOG_DEBUG, "Leaving play state.");
}

/*
 * Transition to a different play state
 */
static void transit(PlayState new_state) {
  TraceLog(LOG_DEBUG, "transit from %d to %d", data->state, new_state);
  data->state = new_state;
}

static bool valid_pos(int nx, int ny, bool door_is_valid, bool player_is_valid) {
  if (nx < 0 || ny < 0 || nx >= data->level.w || ny >= data->level.h) {
    return false;
  }
  Tile t = get_tile(&data->level, nx, ny);
  if (t != FLOOR && (t != DOOR || !door_is_valid)) {
    return false;
  }
  if (!player_is_valid && data->x == nx && data->y == ny) {
    return false;
  }
  for (int i = 0; i < data->n_enemies; ++i) {
    if (data->enemies[i].x == nx && data->enemies[i].y == ny) {
      return false;
    }
  }
  return true;
}

StateName play_update() {
  switch (data->state) {
    case IDLE:
    {
      /* wait for interactions, play idle animation */
      int nx = data->x;
      int ny = data->y;
      bool move = false;
      bool create = false;
      if (IsKeyDown(KEY_R)) {
        GameData* gdata = data->gdata;
        play_suspend();
        play_init(gdata);
      } else if (IsKeyDown(KEY_A)) {
        nx -= 1;
        move = true;
      } else if (IsKeyDown(KEY_D)) {
        nx += 1;
        move = true;
      } else if (IsKeyDown(KEY_W)) {
        ny -= 1;
        move = true;
      } else if (IsKeyDown(KEY_S)) {
        ny += 1;
        move = true;
      } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        nx = (GetMouseX() + data->map_ox - ox) / 50;
        ny = (GetMouseY() + data->map_oy - oy) / 50;
        create = true;
      }
      if ((move || create) && valid_pos(nx, ny, move, !create)) {
        if (move) {
          data->nx = nx;
          data->ny = ny;

          TraceLog(LOG_DEBUG, "move to %d %d", nx, ny);
          set_label(data->turn_info, TextFormat("Turn: %d", ++data->turn));

          transit(RUN);
        } else {  /* create */
          data->nx = nx;
          data->ny = ny;

          TraceLog(LOG_DEBUG, "create a block at %d %d", nx, ny);
          set_label(data->turn_info, TextFormat("Turn: %d", ++data->turn));

          transit(CREATE_BLOCK);
        }
      }
      break;
    }
    case RUN:
      /* play run animation up to the target, then transit to ENEMY_TURN */
      if (data->anim_t >= 1.0) {
        data->x = data->nx;
        data->y = data->ny;
        data->anim_t = 0;
        if (get_tile(&data->level, data->x, data->y) == DOOR) {
          transit(WIN);
        } else {
          transit(ENEMY_TURN);
        }
      } else {
        data->anim_t += 0.1;
      }
      break;
    case CREATE_BLOCK:
      /* play create_block animation, then transit to ENEMY_TURN */
      if (data->anim_t >= 1.0) {
        set_tile(&data->level, data->nx, data->ny, BLOCK);
        data->anim_t = 0;
        transit(ENEMY_TURN);
      } else {
        data->anim_t += 0.1;
      }
      break;
    case ENEMY_TURN:
      /* decide where the enemies are going, then transit to ENEMY_TURN_ANIM */
      for (int i = 0; i < data->n_enemies; ++i) {
        Enemy* e = &data->enemies[i];
        enemy_try_move(e, true);
        int fx = e->nx;
        int fy = e->ny;
        bool move = false;
        do {
          if (valid_pos(e->nx, e->ny, false, true)) { /* monsters cannot go through the door but can go on player */
            move = true;
          } else {
            enemy_try_move(e, false);
          }
          /* loop until a valid move has been found or all possible moves has been exhausted. */
        } while (!move && (e->nx != fx || e->ny != fy));
        if (!move) {
          /* restore if no movement is valid */
          e->nx = e->x;
          e->ny = e->y;
        }
      }
      transit(ENEMY_TURN_ANIM);
      break;
    case ENEMY_TURN_ANIM:
    {
      /* play move animation for all enemies up to there targets, then transit to ENEMY_TURN */
      if (data->anim_t >= 1.0) {
        bool death = false;
        for (int i = 0; i < data->n_enemies; ++i) {
          Enemy* e = &data->enemies[i];
          e->x = e->nx;
          e->y = e->ny;
          if (e->x == data->x && e->y == data->y) {
            death = true;
          }
        }
        data->anim_t = 0;
        if (death) {
          transit(DIE);
        } else {
          transit(IDLE);
        }
      } else {
        data->anim_t += 0.1;
      }
      break;
    }
    case DIE:
      /* play death animation, then transit to END */
      return DEATH;
    case WIN:
      /* play victory animation then change level */
      data->gdata->next_level++;
      return LEVEL_TRANSITION;
  }
  set_offset();
  return PLAY;
}

static void draw_idle() {
  DrawCircle(ox + data->x * 50 - data->map_ox + 25,
             oy + data->y * 50 - data->map_oy + 25, 20, GOLD);
}

static void draw_enemy(Enemy * e) {
  DrawCircle(ox + e->x * 50 - data->map_ox + 25,
             oy + e->y * 50 - data->map_oy + 25, 20, PURPLE);
}

void play_draw() {
  ClearBackground(RAYWHITE);

  /* draw background */
  DrawRectangle(ox, oy, 500, 500, BLUE);
  draw_label(data->hints);
  draw_label(data->turn_info);
  /* draw level */
  int w = data->level.w;
  int h = data->level.h;
  int x0 = 0; // max(data->x - w / 2, 0);
  int y0 = 0; // max(data->y - h / 2, 0);
  for (int x = x0, i = 0; i < w; ++x, ++i) {
    for (int y = y0, j = 0; j < h; ++y, ++j) {
      switch (get_tile(&data->level, x, y)) {
        case WALL:
          DrawRectangle(ox + i * 50 - data->map_ox, oy + j * 50 - data->map_oy,
                        50, 50, RED);
          break;
        case FLOOR:
          DrawRectangle(ox + 2 + i * 50 - data->map_ox,
                        oy + 2 + j * 50 - data->map_oy, 46, 46, GREEN);
          break;
        case DOOR:
          DrawRectangle(ox + 2 + i * 50 - data->map_ox,
                        oy + j * 50 - data->map_oy, 46, 50, YELLOW);
          break;
        case LOCKED_DOOR:
          DrawRectangle(ox + 2 + i * 50 - data->map_ox,
                        oy + j * 50 - data->map_oy, 46, 50, YELLOW);
          DrawRectangle(ox + 2 + i * 50 - data->map_ox,
                        oy + j * 50 - data->map_oy + 20, 46, 10, BLACK);
          break;
        case BLOCK:
          DrawRectangle(ox + 2 + i * 50 - data->map_ox,
                        oy + 2 + j * 50 - data->map_oy, 46, 46, BLACK);
          break;
      }
    }
  }

  if (data->state == ENEMY_TURN || data->state == ENEMY_TURN_ANIM) {
    /* draw idle player */
    draw_idle();
    /* draw enemy moving */
    for (int i = 0; i < data->n_enemies; ++i) {
      Enemy* e = &data->enemies[i];
      int x = lerp(e->x * 50, e->nx * 50, data->anim_t) - data->map_ox;
      int y = lerp(e->y * 50, e->ny * 50, data->anim_t) - data->map_oy;
      DrawCircle(ox + x + 25, oy + y + 25, 20, PURPLE);
    }

  } else {
    /* draw idle monsters */
    for (int i = 0; i < data->n_enemies; ++i) {
      draw_enemy(&data->enemies[i]);
    }
    switch (data->state) {
      case IDLE:
        /* draw idle player */
        draw_idle();
        break;
      case RUN:
      {
        /* draw run animation */
        int x = lerp(data->x * 50, data->nx * 50, data->anim_t) - data->map_ox;
        int y = lerp(data->y * 50, data->ny * 50, data->anim_t) - data->map_oy;
        DrawCircle(ox + x + 25, oy + y + 25, 20, GOLD);
        break;
      }
      case CREATE_BLOCK:
      {
        /* draw idle player */
        draw_idle();
        /* draw create_block animation */
        int h = lerp(0, 46, data->anim_t);
        DrawRectangle(ox + 2 + data->nx * 50 - data->map_ox,
                      oy + 2 + data->ny * 50 - data->map_oy + 46 - h, 46, h,
                      BLACK);
        break;
      }
      case DIE:
        /* play death animation */
        break;
      case WIN:
        /* play victory animation */
        break;
    }
  }
}
