#ifndef UI_H
#define UI_H

#include "shared.h"

void ui_init(difficulty_e difficulty);
void ui_deinit(void);
void ui_change_difficulty(difficulty_e difficulty);
void ui_draw_game(const game_state_s game_state[static 1]);
bool ui_should_close(void);

void ui_register_input_handlers(
  void select_tile(tile_coords_s tile_coords),
  void flag_tile(tile_coords_s tile_coords),
  void change_difficulty(difficulty_e selected_difficulty)
);
void ui_handle_inputs(void);

#endif
