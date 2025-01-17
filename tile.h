#ifndef TILE_H
#define TILE_H

#include "raylib.h"

#define TILE_BORDER_THICKNESS 2

typedef enum tile_variant {
  INSET,
  PROTRUDING,
} tile_variant_e;

void tile_init(void);
void tile_draw_revealed(int pos_x, int pos_y, int width, int height);
void tile_draw_with_shadow(
  int pos_x, int pos_y,
  int width, int height,
  tile_variant_e variant
);

extern Color background_grey;
extern Color shadow_grey;

#endif

