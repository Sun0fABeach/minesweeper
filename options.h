#ifndef OPTIONS_H
#define OPTIONS_H

#include "raylib.h"
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
bool options_toggle_has_mouse_collision(
  int start_x, int start_y, Vector2 mouse_pos
);
bool options_dropdown_has_mouse_collision(
  int start_x, int start_y, Vector2 mouse_pos
);
difficulty_e options_get_selected_difficulty(int start_y, int mouse_y);

#endif

