#ifndef SHARED_H
#define SHARED_H

#define DEBUG false

#define NUM_TILES_X_BEGINNER 9
#define NUM_TILES_Y_BEGINNER 9
#define NUM_MINES_BEGINNER 10
#define NUM_TILES_X_INTERMEDIATE 16
#define NUM_TILES_Y_INTERMEDIATE 16
#define NUM_MINES_INTERMEDIATE 40
#define NUM_TILES_X_EXPERT 30
#define NUM_TILES_Y_EXPERT 16
#define NUM_MINES_EXPERT 99

typedef enum difficulty {
  BEGINNER,
  INTERMEDIATE,
  EXPERT
} difficulty_e;

typedef struct board_tile {
  unsigned num_adjacent_mines: 4;
  unsigned has_mine: 1;
  unsigned flagged: 1;
  unsigned revealed: 1;
  unsigned mine_exploded: 1;
} board_tile_s;

typedef struct game_state {
  bool game_over;
  bool won;
  int remaining_flags;
  int elapsed_time_secs;
  board_tile_s tiles[NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT];
} game_state_s;

#endif

