#include <stddef.h>
#include "board_tile.h"
#include "textures.h"

void board_tile_draw_with_shadow(
  const int pos_x, const int pos_y,
  const tile_variant_e variant,
  const board_tile_s tile
)
{
  tile_draw_with_shadow(
    pos_x, pos_y,
    BOARD_TILE_WIDTH, BOARD_TILE_HEIGHT,
    variant
  );

  if(tile.flagged) {
    textures_draw_from_atlas(
      textures.src_rects.flag,
      pos_x + TILE_BORDER_THICKNESS,
      pos_y + TILE_BORDER_THICKNESS,
      BOARD_TILE_WIDTH - TILE_BORDER_THICKNESS*2,
      BOARD_TILE_HEIGHT - TILE_BORDER_THICKNESS*2
    );
  }
}

void board_tile_draw_revealed(
  const int pos_x, const int pos_y, const board_tile_s tile
)
{
  tile_draw_revealed(pos_x, pos_y, BOARD_TILE_WIDTH, BOARD_TILE_HEIGHT);

  const Rectangle *src_rect = NULL;

  if(tile.has_mine) {
    if(tile.mine_exploded)
      src_rect = &textures.src_rects.mine_exploded;
    else
      src_rect = &textures.src_rects.mine;
  } else if(tile.flagged) {
    src_rect = &textures.src_rects.mine_false_guess;
  } else if(tile.num_adjacent_mines > 0) {
    src_rect = &textures.src_rects.numbers[tile.num_adjacent_mines - 1];
  }

  if(src_rect) {
    textures_draw_from_atlas(
      *src_rect,
      pos_x + 1,
      pos_y + 1,
      BOARD_TILE_WIDTH - 1,
      BOARD_TILE_HEIGHT - 1
    );
  }
}
