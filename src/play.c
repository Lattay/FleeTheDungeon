#include <stdlib.h>
#include "play.h"
#include "lsk_ui.h"

static const int W = 600;
static const int H = 600;
int ox = 50;
int oy = 70;

typedef enum {
  IDLE = 0,
  RUN,
  CREATE_BLOCK,
  ENNEMY_TURN,
  ENNEMY_TURN_ANIM,
  DIE,
  WIN,
} PlayState;

typedef struct {
  int w;
  int h;
  enum tile {
    WALL,
    FLOOR,
    DOOR,
    BLOCK,
  } map[];
} Level;

typedef struct {
  int type;
  int x;
  int y;
  int nx;
  int ny;
  int dir_x;
  int dir_y;
} Ennemy;

typedef struct {
  GameData* gdata;

  /* player data */
  int x;
  int y;
  int nx;
  int ny;

  /* ennemies data */
  size_t n_ennemies;
  Ennemy* ennemies;

  /* general data */
  Level* level;
  PlayState state;
  int turn;
  float anim_t;
  LSKLabel* hints;
  LSKLabel* turn_info;
  int map_ox;
  int map_oy;
} PlayData;

static inline enum tile get_tile(Level* lvl, int x, int y){
  return lvl->map[x + lvl->w * y];
}


static inline void set_tile(Level* lvl, int x, int y, enum tile t){
  lvl->map[x + lvl->w * y] = t;
}

static PlayData* data;

static void set_offset(){
  if(data->state == RUN && data->anim_t != 0){
    float x = lerp(data->x, data->nx, data->anim_t);
    float y = lerp(data->y, data->ny, data->anim_t);
    data->map_ox = x * 50 + ox + 25 - W / 2;
    data->map_oy = y * 50 + oy + 25 - H / 2;
  } else {
    data->map_ox = data->x * 50 + ox + 25 - W / 2;
    data->map_oy = data->y * 50 + oy + 25 - H / 2;
  }
}

void play_init(GameData* gdata){

  SetTraceLogLevel(LOG_DEBUG);

  data = malloc(sizeof(PlayData));
  data->gdata = gdata;

  /* load level */

  /* TODO load level layout */
  /* temporary hard coded "level" */
  data->level = malloc(sizeof(Level) + 36 * sizeof(enum tile));
  data->level->w = 6;
  data->level->h = 6;
  for(int x = 0; x < 6; ++x){
    for(int y = 0; y < 6; ++y){
      if(x == 0 || x == 5 || y == 0 || y == 5){
        data->level->map[x + 6 * y] = WALL;
      } else {
        data->level->map[x + 6 * y] = FLOOR;
      }

      if(x == 1 && y == 0){
        data->level->map[x + 6 * y] = DOOR;
      }
    }
  }

  /* temporary hard coded ennemies */
  data->n_ennemies = 1;
  data->ennemies = malloc(sizeof(Ennemy) * data->n_ennemies);
  /* TODO load ennemies */
  data->ennemies[0].type = 0;
  data->ennemies[0].x = 1;
  data->ennemies[0].y = 1;
  data->ennemies[0].dir_x = 1;
  data->ennemies[0].dir_y = 0;

  /* setup player */
  data->x = 4;
  data->y = 4;
  set_offset();

  /* setup general data */
  data->anim_t = 0;
  data->state = IDLE;
  data->turn = 0;
  data->hints = malloc(sizeof(LSKLabel));
  data->turn_info = malloc(sizeof(LSKManagedLabel));
  init_label(data->hints, "WASD to move, left click to create a block, R to reset the level", W/2, 10, 15);
  init_label(data->turn_info, "Turn: 0", W/2, 30, 20);
}

void play_suspend(){
  TraceLog(LOG_DEBUG, "Freeing play state.");
  free(data->ennemies);
  free(data->level);
  free(data);
  data = NULL;
  TraceLog(LOG_DEBUG, "Leaving play state.");
}

/*
 * Transition to a different play state
 */
static void transit(PlayState new_state){
  TraceLog(LOG_DEBUG, "transit from %d to %d", data->state, new_state);
  data->state = new_state;
}

static bool valid_pos(int nx, int ny, bool door_is_valid, bool player_is_valid){
  if(nx < 0 || ny < 0 || nx >= data->level->w || ny >= data->level->h){
    return false;
  }
  enum tile t = get_tile(data->level, nx, ny);
  if(t != FLOOR && (t != DOOR || !door_is_valid)){
    return false;
  }
  if(!player_is_valid && data->x == nx && data->y == ny){
    return false;
  }
  for(size_t i = 0; i < data->n_ennemies; ++i){
    if(data->ennemies[i].x == nx && data->ennemies[i].y == ny){
      return false;
    }
  }
  return true;
}

StateName play_update(){
  switch(data->state){
    case IDLE:
      {
        /* wait for interactions, play idle animation */
        int nx = data->x;
        int ny = data->y;
        bool move = false;
        bool create = false;
        if(IsKeyDown(KEY_R)){
          GameData* gdata = data->gdata;
          play_suspend();
          play_init(gdata);
        } else if(IsKeyDown(KEY_A)){
          nx -= 1;
          move = true;
        } else if(IsKeyDown(KEY_D)){
          nx += 1;
          move = true;
        } else if(IsKeyDown(KEY_W)){
          ny -= 1;
          move = true;
        } else if(IsKeyDown(KEY_S)){
          ny += 1;
          move = true;
        } else if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
          nx = (GetMouseX() + data->map_ox - ox) / 50;
          ny = (GetMouseY() + data->map_oy - oy) / 50;
          create = true;
        }
        if((move || create) && valid_pos(nx, ny, move, !create)){
          if(move){
            data->nx = nx;
            data->ny = ny;

            TraceLog(LOG_DEBUG, "move to %d %d", nx, ny);
            set_label(data->turn_info, TextFormat("Turn: %d", ++data->turn));

            transit(RUN);
          } else { /* create */
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
      /* play run animation up to the target, then transit to ENNEMY_TURN */
      if(data->anim_t >= 1.0){
        data->x = data->nx;
        data->y = data->ny;
        data->anim_t = 0;
        if(get_tile(data->level, data->x, data->y) == DOOR){
          transit(WIN);
        } else {
          transit(ENNEMY_TURN);
        }
      } else {
        data->anim_t += 0.1;
      }
      break;
    case CREATE_BLOCK:
      /* play create_block animation, then transit to ENNEMY_TURN */
      if(data->anim_t >= 1.0){
        set_tile(data->level, data->nx, data->ny, BLOCK);
        data->anim_t = 0;
        transit(ENNEMY_TURN);
      } else {
        data->anim_t += 0.1;
      }
      break;
    case ENNEMY_TURN:
      /* decide where the ennemies are going, then transit to ENNEMY_TURN_ANIM */
      for(size_t i = 0; i < data->n_ennemies; ++i){
        Ennemy* e = &data->ennemies[i];
        int nx = e->x + e->dir_x;
        int ny = e->y + e->dir_y;
        int fx = nx;
        int fy = ny;
        bool move = false;
        do{
          if(valid_pos(nx, ny, false, true)){ /* monsters cannot go through the door but can go on player*/
            data->nx = nx;
            data->ny = ny;
            move = true;
          } else {
            e->dir_x *= -1;
            e->dir_y *= -1;
            nx = e->x + e->dir_x;
            ny = e->y + e->dir_y;
          }
        } while(!move && (nx != fx || ny != fy)); /* loop until a valid move has been found or all possible moves has been exhausted. */
        e->nx = move ? nx : e->x;
        e->ny = move ? ny : e->y;
      }
      transit(ENNEMY_TURN_ANIM);
      break;
    case ENNEMY_TURN_ANIM:
      {
        /* play move animation for all ennemies up to there targets, then transit to ENNEMY_TURN */
        if(data->anim_t >= 1.0){
          bool death = false;
          for(size_t i = 0; i < data->n_ennemies; ++i){
            Ennemy* e = &data->ennemies[i];
            e->x = e->nx;
            e->y = e->ny;
            if(e->x == data->x && e->y == data->y){
              death = true;
            }
          }
          data->anim_t = 0;
          if(death){
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
      return END;
  }
  set_offset();
  return PLAY;
}

static void draw_idle(){
  DrawCircle(ox + data->x * 50 - data->map_ox + 25, oy + data->y * 50 - data->map_oy + 25, 20, GOLD);
}

static void draw_ennemy(Ennemy* e){
  DrawCircle(ox + e->x * 50 - data->map_ox + 25, oy + e->y * 50 - data->map_oy + 25, 20, PURPLE);
}

void play_draw(){
  ClearBackground(RAYWHITE);

  /* draw background */
  DrawRectangle(ox, oy, 500, 500, BLUE);
  draw_label(data->hints);
  draw_label(data->turn_info);
  /* draw level */
  int w = data->level->w;
  int h = data->level->h;
  int x0 = 0; // max(data->x - w / 2, 0);
  int y0 = 0; // max(data->y - h / 2, 0);
  for(int x = x0, i = 0; i < w; ++x, ++i){
    for(int y = y0, j = 0; j < h; ++y, ++j){
      switch(get_tile(data->level, x, y)){
        case WALL:
          DrawRectangle(ox + i * 50 - data->map_ox, oy + j * 50 - data->map_oy, 50, 50, RED);
          break;
        case FLOOR:
          DrawRectangle(ox + 2 + i * 50 - data->map_ox, oy + 2 + j * 50 - data->map_oy, 46, 46, GREEN);
          break;
        case DOOR:
          DrawRectangle(ox + 2 + i * 50 - data->map_ox, oy + j * 50 - data->map_oy, 46, 50, YELLOW);
          break;
        case BLOCK:
          DrawRectangle(ox + 2 + i * 50 - data->map_ox, oy + 2 + j * 50 - data->map_oy, 46, 46, BLACK);
          break;
      }
    }
  }


  if(data->state == ENNEMY_TURN || data->state == ENNEMY_TURN_ANIM){
    /* draw idle player */
    draw_idle();
    /* draw ennemy moving */
    for(size_t i = 0; i < data->n_ennemies; ++i){
      Ennemy* e = &data->ennemies[i];
      int x = lerp(e->x * 50, e->nx * 50, data->anim_t) - data->map_ox;
      int y = lerp(e->y * 50, e->ny * 50, data->anim_t) - data->map_oy;
      DrawCircle(ox + x + 25, oy + y + 25, 20, PURPLE);
    }

  } else {
    /* draw idle monsters */
    for(size_t i = 0; i < data->n_ennemies; ++i){
      draw_ennemy(&data->ennemies[i]);
    }
    switch(data->state){
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
          DrawRectangle(ox + 2 + data->nx * 50 - data->map_ox, oy + 2 + data->ny * 50 - data->map_oy + 46 - h, 46, h, BLACK);
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
