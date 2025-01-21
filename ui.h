#ifndef UI_H
#define UI_H

#include "shared.h"
#include "raylib.h"

void ui_init(int num_rows, int num_cols);
void ui_deinit(void);
void ui_change_difficulty(int num_rows, int num_cols);
void ui_draw_game(const game_state_s game_state[static 1]);
static inline bool ui_should_close(void)
{
  return WindowShouldClose();
}


void ui_register_input_handlers(
  void select_board_tile(int row, int col),
  void flag_board_tile(int row, int col),
  void push_smiley(void),
  void change_difficulty(difficulty_e selected_difficulty)
);
void ui_handle_inputs(void);

#endif
