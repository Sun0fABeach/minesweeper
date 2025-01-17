#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ui.h"
#include "shared.h"

typedef struct adjacent_space {
  int start_row, start_col;
  int end_row, end_col;
} adjacent_space_s;

typedef struct difficulty_settings {
  const int num_cols;
  const int num_rows;
  const int num_mines;
} difficulty_settings_s;

static void init_game_state(void);
static void place_mines(void);
static void count_adjacent_mines(int row, int col);
static void register_input_handlers(void);
static void handle_tile_select(int row, int col);
static void handle_tile_flagging(int row, int col);
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


static constexpr difficulty_settings_s difficulty_settings[] = {
  [BEGINNER] = {
    .num_cols = NUM_TILES_X_BEGINNER,
    .num_rows = NUM_TILES_Y_BEGINNER,
    .num_mines = NUM_MINES_BEGINNER
  },
  [INTERMEDIATE] = {
    .num_cols = NUM_TILES_X_INTERMEDIATE,
    .num_rows = NUM_TILES_Y_INTERMEDIATE,
    .num_mines = NUM_MINES_INTERMEDIATE
  },
  [EXPERT] = {
    .num_cols = NUM_TILES_X_EXPERT,
    .num_rows = NUM_TILES_Y_EXPERT,
    .num_mines = NUM_MINES_EXPERT
  }
};

/* game state not shared with the ui */
static const difficulty_settings_s *difficulty = &difficulty_settings[EXPERT];
static bool timer_active;
static time_t start_time;
static int remaining_tiles;
/* game state shared with the ui */
static game_state_s game_state;


int main(void)
{
  srand(time(NULL));

  init_game_state();
  ui_init(difficulty->num_rows, difficulty->num_cols);
  register_input_handlers();
  game_loop();
  ui_deinit();

  return EXIT_SUCCESS;
}

static void init_game_state(void)
{
  memset(game_state.tiles, 0, sizeof(game_state.tiles));

  remaining_tiles =
    difficulty->num_rows * difficulty->num_cols - difficulty->num_mines;
  timer_active = false;
  game_state.remaining_flags = difficulty->num_mines;
  game_state.elapsed_time_secs = 0;
  game_state.game_over = false;
  game_state.won = false;

  place_mines();

#if DEBUG
  reveal_all_tiles();
#endif
}

static void place_mines(void)
{
  for(int placed_mines = 0; placed_mines < difficulty->num_mines;) {
    const int row = rand() % difficulty->num_rows;
    const int col = rand() % difficulty->num_cols;
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

  for(int y = adjacent_space.start_row; y <= adjacent_space.end_row; y++)
    for(int x = adjacent_space.start_col; x <= adjacent_space.end_col; x++)
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

static void handle_tile_select(const int row, const int col)
{
  if(game_state.game_over)
    return;

  tile_s *const tile = &game_state.tiles[row][col];

  if(tile->flagged || tile->revealed)
    return;

  if(!timer_active) {
    timer_active = true;
    start_time = time(NULL);
  }

  tile->revealed = true;
  remaining_tiles--;

  if(tile->has_mine) {
    tile->mine_exploded = true;
    game_state.game_over = true;
    timer_active = false;
    reveal_all_mines();
  } else {
    if(tile->num_adjacent_mines == 0)
      reveal_mineless_space(row, col);
    if(remaining_tiles == 0) {
      game_state.game_over = true;
      game_state.won = true;
      timer_active = false;
      flag_all_mines();
    }
  }
}

static void handle_tile_flagging(const int row, const int col)
{
  if(game_state.game_over)
    return;

  tile_s *const tile = &game_state.tiles[row][col];

  if(!tile->revealed) {
    if(tile->flagged) {
      tile->flagged = false;
      game_state.remaining_flags++;
    } else {
      tile->flagged = true;
      game_state.remaining_flags--;
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

  for(int y = adjacent_space.start_row; y <= adjacent_space.end_row; y++) {
    for(int x = adjacent_space.start_col; x <= adjacent_space.end_col; x++) {
      if(y == row && x == col)
        continue;

      tile_s *const adjacent_tile = &game_state.tiles[y][x];

      if(!adjacent_tile->revealed && !adjacent_tile->flagged) {
        adjacent_tile->revealed = true;
        remaining_tiles--;
        if(adjacent_tile->num_adjacent_mines == 0)
          reveal_mineless_space(y, x);
      }
    }
  }
}

static void reveal_all_mines(void)
{
  for(int row = 0; row < difficulty->num_rows; row++) {
    for(int col = 0; col < difficulty->num_cols; col++) {
      tile_s *const tile = &game_state.tiles[row][col];
      if(!tile->revealed)
        tile->revealed = tile->flagged ^ tile->has_mine;
    }
  }
}

static void flag_all_mines(void)
{
  for(int row = 0; row < difficulty->num_rows; row++) {
    for(int col = 0; col < difficulty->num_cols; col++) {
      tile_s *const tile = &game_state.tiles[row][col];
      if(tile->has_mine && !tile->flagged)
        tile->flagged = true;
    }
  }
}

adjacent_space_s get_adjacent_space(const int row, const int col)
{
  const int num_rows = difficulty->num_rows;
  const int num_cols = difficulty->num_cols;

  return (adjacent_space_s) {
    .start_row = row > 0 ? row - 1 : row,
    .start_col = col > 0 ? col - 1 : col,
    .end_row = row == num_rows - 1 ? row : row + 1,
    .end_col = col == num_cols - 1 ? col : col + 1
  };
}

static void handle_difficulty_select(const difficulty_e selected_difficulty)
{
  if(&difficulty_settings[selected_difficulty] == difficulty)
    return;
  difficulty = &difficulty_settings[selected_difficulty];

  init_game_state();
  ui_change_difficulty(difficulty->num_rows, difficulty->num_cols);
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
  for (int row = 0; row < difficulty->num_rows; row++)
    for (int col = 0; col < difficulty->num_cols; col++)
      game_state.tiles[row][col].revealed = true;
}
#endif
