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

void options_draw(void);
bool options_toggle_has_mouse_collision(Vector2 mouse_pos);
bool options_dropdown_has_mouse_collision(Vector2 mouse_pos);
difficulty_e options_get_selected_difficulty(int mouse_y);

extern int options_toggle_x;
extern int options_toggle_y;
extern int options_dropdown_x;
extern int options_dropdown_y;
extern bool options_open;
extern bool options_capture_inputs;
extern difficulty_e options_pressed_difficulty;

#endif

