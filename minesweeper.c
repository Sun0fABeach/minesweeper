#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ui.h"
#include "shared.h"

typedef struct adjacent_space {
  tile_coords_s start;
  tile_coords_s end;
} adjacent_space_s;

static void init_game_state(void);
static void place_mines(int num_rows, int num_cols, int num_mines);
static void count_adjacent_mines(int row, int col);
static void register_input_handlers(void);
static void handle_tile_select(tile_coords_s coords);
static void handle_tile_flagging(tile_coords_s coords);
static void reveal_mineless_space(int row, int col);
static void reveal_all_mines(void);
static void flag_all_mines(void);
static adjacent_space_s get_adjacent_space(int row, int col);
static void handle_smiley_press(void);
static void handle_difficulty_select(difficulty_e selected_difficulty);
static void game_loop(void);
static void update_timer(void);
#if DEBUG
static void reveal_all_tiles(void);
#endif

static bool timer_active;
static time_t start_time;
static difficulty_e current_difficulty = EXPERT;
static game_state_s game_state;


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

static void init_game_state(void)
{
  memset(game_state.tiles, 0, sizeof(game_state.tiles));

  const difficulty_settings_s config = difficulty_settings[current_difficulty];
  game_state.num_mines = config.num_mines;
  game_state.remaining_tiles =
    config.num_tiles_x * config.num_tiles_y - config.num_mines;
  game_state.flagged_tiles = 0;
  game_state.game_over = false;
  game_state.won = false;
  game_state.elapsed_time_secs = 0;
  timer_active = false;

  place_mines(config.num_tiles_y, config.num_tiles_x, config.num_mines);

#if DEBUG
  reveal_all_tiles();
#endif
}

static void place_mines(
  const int num_rows, const int num_cols, const int num_mines
)
{

  for(int placed_mines = 0; placed_mines < num_mines;) {
    const int row = rand() % num_rows;
    const int col = rand() % num_cols;
    if(game_state.tiles[row][col].has_mine)
      continue;
    game_state.tiles[row][col].has_mine = true;
    placed_mines++;

    count_adjacent_mines(row, col);
  }
}

static void count_adjacent_mines(const int row, const int col)
{
  const adjacent_space_s adjacent_space = get_adjacent_space(row, col);

  for(int y = adjacent_space.start.y; y <= adjacent_space.end.y; y++)
    for(int x = adjacent_space.start.x; x <= adjacent_space.end.x; x++)
      if(!(y == row && x == col))
        game_state.tiles[y][x].num_adjacent_mines++;
}

static void register_input_handlers(void)
{
  ui_register_input_handlers(
    handle_tile_select,
    handle_tile_flagging,
    handle_smiley_press,
    handle_difficulty_select
  );
}

static void handle_tile_select(const tile_coords_s coords)
{
  if(game_state.game_over)
    return;

  tile_s *const tile = &game_state.tiles[coords.y][coords.x];

  if(tile->flagged || tile->revealed)
    return;

  tile->revealed = true;
  game_state.remaining_tiles--;

  if(!timer_active) {
    timer_active = true;
    start_time = time(NULL);
  }

  if(tile->has_mine) {
    tile->mine_exploded = true;
    game_state.game_over = true;
    timer_active = false;
    reveal_all_mines();
  } else {
    if(tile->num_adjacent_mines == 0)
      reveal_mineless_space(coords.y, coords.x);
    if(game_state.remaining_tiles == 0) {
      game_state.game_over = true;
      game_state.won = true;
      timer_active = false;
      flag_all_mines();
    }
  }
}

static void handle_tile_flagging(const tile_coords_s coords)
{
  if(game_state.game_over)
    return;

  tile_s *const tile = &game_state.tiles[coords.y][coords.x];

  if(!tile->revealed) {
    if(tile->flagged) {
      tile->flagged = false;
      game_state.flagged_tiles--;
    } else {
      tile->flagged = true;
      game_state.flagged_tiles++;
    }
  }
}

static void handle_smiley_press(void)
{
  init_game_state();
}

static void reveal_mineless_space(const int row, const int col)
{
  const adjacent_space_s adjacent_space = get_adjacent_space(row, col);

  for(int y = adjacent_space.start.y; y <= adjacent_space.end.y; y++) {
    for(int x = adjacent_space.start.x; x <= adjacent_space.end.x; x++) {
      if(!(y == row && x == col)) {
        tile_s *const adjacent_tile = &game_state.tiles[y][x];

        if(!adjacent_tile->revealed && !adjacent_tile->flagged) {
          adjacent_tile->revealed = true;
          game_state.remaining_tiles--;
          if(adjacent_tile->num_adjacent_mines == 0) {
            reveal_mineless_space(y, x);
          }
        }
      }
    }
  }
}

static void reveal_all_mines(void)
{
  const difficulty_settings_s config = difficulty_settings[current_difficulty];

  for(int row = 0; row < config.num_tiles_y; row++) {
    for(int col = 0; col < config.num_tiles_x; col++) {
      tile_s *const tile = &game_state.tiles[row][col];
      if(!tile->revealed)
        tile->revealed = tile->flagged ^ tile->has_mine;
    }
  }
}

static void flag_all_mines(void)
{
  const difficulty_settings_s config = difficulty_settings[current_difficulty];

  for(int row = 0; row < config.num_tiles_y; row++) {
    for(int col = 0; col < config.num_tiles_x; col++) {
      tile_s *const tile = &game_state.tiles[row][col];
      if(tile->has_mine && !tile->flagged)
        tile->flagged = true;
    }
  }
}

adjacent_space_s get_adjacent_space(const int row, const int col)
{
  const int num_rows = difficulty_settings[current_difficulty].num_tiles_y;
  const int num_cols = difficulty_settings[current_difficulty].num_tiles_x;

  return (adjacent_space_s) {
    .start = {
      .y = row > 0 ? row - 1 : row,
      .x = col > 0 ? col - 1 : col
    },
    .end = {
      .y = row == num_rows - 1 ? row : row + 1,
      .x = col == num_cols - 1 ? col : col + 1
    }
  };
}

static void handle_difficulty_select(const difficulty_e selected_difficulty)
{
  if(selected_difficulty == current_difficulty)
    return;
  current_difficulty = selected_difficulty;

  init_game_state();
  ui_change_difficulty(current_difficulty);
}

static void game_loop(void)
{
  while(true) {
    if(ui_should_close())
      return;
    ui_handle_inputs();
    update_timer();
    ui_draw_game(&game_state);
	}
}

static void update_timer(void)
{
  if(timer_active)
    game_state.elapsed_time_secs = difftime(time(NULL), start_time);
}

#if DEBUG
static void reveal_all_tiles(void)
{
  for (int row = 0; row < NUM_TILES_Y_EXPERT; row++)
    for (int col = 0; col < NUM_TILES_X_EXPERT; col++)
      game_state.tiles[row][col].revealed = true;
}
#endif
