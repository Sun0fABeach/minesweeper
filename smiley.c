#include "raylib.h"
#include "smiley.h"
#include "textures.h"
#include "tile.h"

void smiley_draw(
  int pos_x, int pos_y, const smiley_type_e type, const bool pressed
)
{
  DrawRectangle(pos_x, pos_y, SMILEY_WIDTH, SMILEY_HEIGHT, shadow_grey);

  Rectangle src_rect;

  if(pressed) {
    tile_draw_revealed(
      pos_x + 1,
      pos_y + 1,
      SMILEY_WIDTH - 2,
      SMILEY_HEIGHT - 2
    );
    pos_x += 1 + TILE_BORDER_THICKNESS*2 + SMILEY_PADDING;
    pos_y += 1 + TILE_BORDER_THICKNESS*2 + SMILEY_PADDING;
    src_rect = textures.src_rects.smiley;
  } else {
    tile_draw_with_shadow(
      pos_x + 1,
      pos_y + 1,
      SMILEY_WIDTH - 2,
      SMILEY_HEIGHT - 2,
      PROTRUDING
    );
    pos_x += 1 + TILE_BORDER_THICKNESS + SMILEY_PADDING;
    pos_y += 1 + TILE_BORDER_THICKNESS + SMILEY_PADDING;

    switch(type) {
      case SMILEY_HAPPY:
        src_rect = textures.src_rects.smiley;
        break;
      case SMILEY_EXCITED:
        src_rect = textures.src_rects.smiley_excited;
        break;
      case SMILEY_DEAD:
        src_rect = textures.src_rects.smiley_dead;
        break;
      case SMILEY_COOL:
        src_rect = textures.src_rects.smiley_cool;
        break;
      default: // should never be reached
        src_rect = textures.src_rects.smiley;
    }
  }

  textures_draw_from_atlas(
    src_rect,
    pos_x, pos_y,
    SMILEY_WIDTH_INNER, SMILEY_HEIGHT_INNER
  );
}
