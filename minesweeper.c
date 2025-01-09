#include <stdlib.h>
#include "ui.h"
#include "shared.h"

void register_input_handlers(void);
void game_loop(void);
void handle_difficulty_select(difficulty_e selected_difficulty);

difficulty_e current_difficulty = INTERMEDIATE;


int main(void)
{
	ui_init(current_difficulty);
  register_input_handlers();
  game_loop();
	ui_deinit();

  return EXIT_SUCCESS;
}

void register_input_handlers(void)
{
  ui_register_input_handlers(
    handle_difficulty_select
  );
}

void game_loop(void)
{
	while(true) {
    if(ui_should_close())
      return;
    ui_handle_inputs();
    ui_draw_game();
	}
}

void handle_difficulty_select(const difficulty_e selected_difficulty)
{
  if(selected_difficulty == current_difficulty)
    return;
  current_difficulty = selected_difficulty;
  ui_change_difficulty(current_difficulty);
}
