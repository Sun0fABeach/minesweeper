#ifndef OPTIONS_H
#define OPTIONS_H

#include "shared.h"

#define OPTIONS_TOGGLE_WIDTH 15
#define OPTIONS_TOGGLE_HEIGHT 15
#define OPTIONS_BUTTON_WIDTH 80
#define OPTIONS_BUTTON_HEIGHT 25
#define OPTIONS_DROPDOWN_WIDTH (OPTIONS_BUTTON_WIDTH + 2)
#define OPTIONS_DROPDOWN_HEIGHT (OPTIONS_BUTTON_HEIGHT*3 + 2)
#define OPTIONS_DROPDOWN_MARGIN_TOP 2
#define OPTIONS_DROPDOWN_MARGIN_LEFT 1

void options_draw(
  int start_x, int start_y, bool is_open, difficulty_e pressed_button
);

#endif

