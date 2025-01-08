#include <stdlib.h>
#include "ui.h"

void game_loop(void);

int main(void)
{
	ui_init();
  game_loop();
	ui_deinit();

  return EXIT_SUCCESS;
}

void game_loop(void)
{
	while(true) {
		// do stuff
    ui_draw_game();
    if(ui_should_close())
      return;
	}
}

