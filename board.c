#include "board.h"

static void board_draw_tiles(
  const board_tile_s board[static const NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
);

int board_x;
int board_y;
int board_inner_width;
int board_inner_height;
int board_width;
int board_height;
int board_num_rows;
int board_num_cols;
board_pressed_tile_s board_pressed_tile;


void board_draw(
  const board_tile_s board[static const NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
)
{
  tile_draw_with_shadow(
    board_x, board_y,
    board_width, board_height,
    INSET
  );
  board_draw_tiles(board);
}

static void board_draw_tiles(
  const board_tile_s board[static const NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
)
{
  const int pos_x = board_x + TILE_BORDER_THICKNESS;
  const int pos_y = board_y + TILE_BORDER_THICKNESS;

  for(int row = 0; row < board_num_rows; row++) {
    const int tile_y = pos_y + BOARD_TILE_HEIGHT * row;

    for(int col = 0; col < board_num_cols; col++) {
      const int tile_x = pos_x + BOARD_TILE_WIDTH * col;

      if(board[row][col].revealed) {
        board_tile_draw_revealed(tile_x, tile_y, board[row][col]);
      } else {
        board_tile_draw_with_shadow(
          tile_x, tile_y,
          PROTRUDING,
          board[row][col]
        );
      }
    }
  }

  if(board_pressed_tile.active) {
    const int col = board_pressed_tile.col;
    const int row = board_pressed_tile.row;

    if(!board[row][col].revealed && !board[row][col].flagged)
      tile_draw_revealed(
        pos_x + BOARD_TILE_WIDTH * col,
        pos_y + BOARD_TILE_HEIGHT * row,
        BOARD_TILE_WIDTH, BOARD_TILE_HEIGHT
      );
  }
}

bool board_has_mouse_collision(const Vector2 mouse_pos)
{
  const int pos_x = board_x + TILE_BORDER_THICKNESS;
  const int pos_y = board_y + TILE_BORDER_THICKNESS;

  const Rectangle board_rect = {
    .x = pos_x,
    .y = pos_y,
    .width = board_inner_width,
    .height = board_inner_height
  };

  return CheckCollisionPointRec(mouse_pos, board_rect);
}

