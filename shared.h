#ifndef SHARED_H
#define SHARED_H

#include <stdint.h>

#define NUM_TILES_X_BEGINNER 9
#define NUM_TILES_Y_BEGINNER 9
#define NUM_TILES_X_INTERMEDIATE 16
#define NUM_TILES_Y_INTERMEDIATE 16
#define NUM_TILES_X_EXPERT 30
#define NUM_TILES_Y_EXPERT 16
#define NUM_MINES_BEGINNER 10
#define NUM_MINES_INTERMEDIATE 40
#define NUM_MINES_EXPERT 99

typedef enum difficulty {
  BEGINNER,
  INTERMEDIATE,
  EXPERT
} difficulty_e;

typedef struct difficulty_settings {
  const int num_tiles_x;
  const int num_tiles_y;
  const int num_mines;
} difficulty_settings_s;

static constexpr difficulty_settings_s difficulty_settings[] = {
  [BEGINNER] = {
    .num_tiles_x = NUM_TILES_X_BEGINNER,
    .num_tiles_y = NUM_TILES_Y_BEGINNER,
    .num_mines = NUM_MINES_BEGINNER
  },
  [INTERMEDIATE] = {
    .num_tiles_x = NUM_TILES_X_INTERMEDIATE,
    .num_tiles_y = NUM_TILES_Y_INTERMEDIATE,
    .num_mines = NUM_MINES_INTERMEDIATE
  },
  [EXPERT] = {
    .num_tiles_x = NUM_TILES_X_EXPERT,
    .num_tiles_y = NUM_TILES_Y_EXPERT,
    .num_mines = NUM_MINES_EXPERT
  }
};

typedef struct tile {
  unsigned num_adjacent_bombs: 4;
  unsigned has_bomb: 1;
  unsigned has_flag: 1;
  unsigned revealed: 1;
  unsigned bomb_exploded: 1;
} tile_s;

typedef struct game_state {
  // other stuff
  tile_s tiles[NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT];
} game_state_s;

typedef struct tile_coords {
  uint8_t x;
  uint8_t y;
} tile_coords_s;

#endif

