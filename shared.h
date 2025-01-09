#ifndef SHARED_H
#define SHARED_H

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
    .num_tiles_x = 9,
    .num_tiles_y = 9,
    .num_mines = 10
  },
  [INTERMEDIATE] = {
    .num_tiles_x = 16,
    .num_tiles_y = 16,
    .num_mines = 40
  },
  [EXPERT] = {
    .num_tiles_x = 30,
    .num_tiles_y = 16,
    .num_mines = 99
  }
};

#endif

