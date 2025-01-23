#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"
#include "tile.h"
#include "board_tile.h"
#include "shared.h"

typedef struct board_pressed_tile {
  int row, col;
  bool active;
} board_pressed_tile_s;

extern int board_x;
extern int board_y;
extern int board_inner_width;
extern int board_inner_height;
extern int board_width;
extern int board_height;
extern int board_num_rows;
extern int board_num_cols;
extern board_pressed_tile_s board_pressed_tile;


void board_draw(
  const board_tile_s board[static NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
);
bool board_has_mouse_collision(Vector2 mouse_pos);

static inline int board_get_row(const int pos_y)
{
  const int inner_y = board_y + TILE_BORDER_THICKNESS;
  return (pos_y - inner_y) / BOARD_TILE_HEIGHT;
}

static inline int board_get_col(const int pos_x)
{
  const int inner_x = board_x + TILE_BORDER_THICKNESS;
  return (pos_x - inner_x) / BOARD_TILE_WIDTH;
}

#endif
