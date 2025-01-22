#ifndef SMILEY_H
#define SMILEY_H

#include "raylib.h"

#define SMILEY_WIDTH_INNER 32
#define SMILEY_HEIGHT_INNER 32
#define SMILEY_PADDING 2
#define SMILEY_WIDTH (SMILEY_WIDTH_INNER + 2 + TILE_BORDER_THICKNESS*2 + \
  SMILEY_PADDING*2)
#define SMILEY_HEIGHT (SMILEY_HEIGHT_INNER + 2 + TILE_BORDER_THICKNESS*2 + \
  SMILEY_PADDING*2)

typedef enum smiley_type {
  SMILEY_HAPPY,
  SMILEY_EXCITED,
  SMILEY_DEAD,
  SMILEY_COOL
} smiley_type_e;

void smiley_draw(int pos_x, int pos_y, smiley_type_e type, bool pressed);
bool smiley_has_mouse_collision(int start_x, int start_y, Vector2 mouse_pos);

#endif

