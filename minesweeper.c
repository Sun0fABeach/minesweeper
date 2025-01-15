#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ui.h"
#include "shared.h"

#if DEBUG
void reveal_all_tiles(void);
#endif

void init_game_state(void);
void place_mines(void);
void count_adjacent_mines(int row, int col, int num_rows, int num_cols);
void register_input_handlers(void);
void handle_tile_select(tile_coords_s coords);
void reveal_all_mines(void);
void handle_difficulty_select(difficulty_e selected_difficulty);
void game_loop(void);

difficulty_e current_difficulty = EXPERT;
game_state_s game_state;


int main(void)
{
  srand(time(NULL));

  init_game_state();
	ui_init(current_difficulty);
  register_input_handlers();
  game_loop();
	ui_deinit();

  return EXIT_SUCCESS;
}

void init_game_state(void)
{
  game_state.game_over = false;
  memset(game_state.tiles, 0, sizeof(game_state.tiles));
  place_mines();

#if DEBUG
  reveal_all_tiles();
#endif
}

void place_mines(void)
{
  const difficulty_settings_s config = difficulty_settings[current_difficulty];
  for(int placed_mines = 0; placed_mines < config.num_mines;) {
    const int row = rand() % config.num_tiles_y;
    const int col = rand() % config.num_tiles_x;
    if(game_state.tiles[row][col].has_mine)
      continue;
    game_state.tiles[row][col].has_mine = true;
    placed_mines++;

    count_adjacent_mines(row, col, config.num_tiles_y, config.num_tiles_x);
  }
}

void count_adjacent_mines(
  const int row, const int col, const int num_rows, const int num_cols
)
{
  const int start_row = row > 0 ? row - 1 : row;
  const int end_row = row == num_rows - 1 ? row : row + 1;
  const int start_col = col > 0 ? col - 1 : col;
  const int end_col = col == num_cols - 1 ? col : col + 1;

  for(int y = start_row; y <= end_row; y++)
    for(int x = start_col; x <= end_col; x++)
      if(!(y == row && x == col))
        game_state.tiles[y][x].num_adjacent_mines++;
}

void register_input_handlers(void)
{
  ui_register_input_handlers(
    handle_tile_select,
    handle_difficulty_select
  );
}

void handle_tile_select(const tile_coords_s coords)
{
  if(game_state.game_over)
    return;

  tile_s *const tile = &game_state.tiles[coords.y][coords.x];

  tile->revealed = true;
  if(tile->has_mine) {
    tile->mine_exploded = true;
    game_state.game_over = true;
    reveal_all_mines();
  }
}

void reveal_all_mines(void)
{
  for(int row = 0; row < NUM_TILES_Y_EXPERT; row++)
    for(int col = 0; col < NUM_TILES_X_EXPERT; col++)
      if(game_state.tiles[row][col].has_mine)
        game_state.tiles[row][col].revealed = true;
}

void handle_difficulty_select(const difficulty_e selected_difficulty)
{
  if(selected_difficulty == current_difficulty)
    return;
  current_difficulty = selected_difficulty;

  init_game_state();
  ui_change_difficulty(current_difficulty);
}

void game_loop(void)
{
  while(true) {
    if(ui_should_close())
      return;
    ui_handle_inputs();
    ui_draw_game(&game_state);
	}
}

#if DEBUG
void reveal_all_tiles(void)
{
  for (int row = 0; row < NUM_TILES_Y_EXPERT; row++)
    for (int col = 0; col < NUM_TILES_X_EXPERT; col++)
      game_state.tiles[row][col].revealed = true;
}
#endif
