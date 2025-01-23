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

void smiley_draw(smiley_type_e type);
bool smiley_has_mouse_collision(Vector2 mouse_pos);

extern int smiley_x;
extern int smiley_y;
extern int smiley_is_pressed;

#endif

