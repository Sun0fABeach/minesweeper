#ifndef BOARD_TILE_H
#define BOARD_TILE_H

#include "tile.h"
#include "shared.h"

#define BOARD_TILE_WIDTH_INNER 20
#define BOARD_TILE_HEIGHT_INNER 20
#define BOARD_TILE_WIDTH (BOARD_TILE_WIDTH_INNER + TILE_BORDER_THICKNESS*2)
#define BOARD_TILE_HEIGHT (BOARD_TILE_HEIGHT_INNER + TILE_BORDER_THICKNESS*2)

void board_tile_draw_with_shadow(
  int pos_x, int pos_y,
  tile_variant_e variant,
  board_tile_s tile
);
void board_tile_draw_revealed(int pos_x, int pos_y, board_tile_s tile);

#endif

