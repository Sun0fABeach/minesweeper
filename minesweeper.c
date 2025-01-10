#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "shared.h"

void init_game_state(void);
void register_input_handlers(void);
void handle_tile_select(tile_coords_s coords);
void handle_difficulty_select(difficulty_e selected_difficulty);
void game_loop(void);

difficulty_e current_difficulty = INTERMEDIATE;
game_state_s game_state;


int main(void)
{
  init_game_state();
	ui_init(current_difficulty);
  register_input_handlers();
  game_loop();
	ui_deinit();

  return EXIT_SUCCESS;
}

void init_game_state(void)
{
  memset(game_state.tiles, 0, sizeof(game_state.tiles));
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
  game_state.tiles[coords.y][coords.x].revealed = true;
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

