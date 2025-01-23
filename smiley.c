#include "smiley.h"
#include "textures.h"
#include "tile.h"

int smiley_x;
int smiley_y;
int smiley_is_pressed;

void smiley_draw(const smiley_type_e type)
{
  DrawRectangle(smiley_x, smiley_y, SMILEY_WIDTH, SMILEY_HEIGHT, shadow_grey);

  // account for 1 px frame
  int texture_x = smiley_x + 1 + TILE_BORDER_THICKNESS + SMILEY_PADDING;
  int texture_y = smiley_y + 1 + TILE_BORDER_THICKNESS + SMILEY_PADDING;
  Rectangle src_rect;

  if(smiley_is_pressed) {
    tile_draw_revealed(
      smiley_x + 1,
      smiley_y + 1,
      SMILEY_WIDTH - 2,
      SMILEY_HEIGHT - 2
    );
    texture_x += TILE_BORDER_THICKNESS;
    texture_y += TILE_BORDER_THICKNESS;
    src_rect = textures.src_rects.smiley;
  } else {
    tile_draw_with_shadow(
      smiley_x + 1,
      smiley_y + 1,
      SMILEY_WIDTH - 2,
      SMILEY_HEIGHT - 2,
      PROTRUDING
    );

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
    texture_x, texture_y,
    SMILEY_WIDTH_INNER, SMILEY_HEIGHT_INNER
  );
}

bool smiley_has_mouse_collision(const Vector2 mouse_pos)
{
  const Rectangle smiley_rect = {
    .x = smiley_x,
    .y = smiley_y,
    .width = SMILEY_WIDTH,
    .height = SMILEY_HEIGHT
  };

  return CheckCollisionPointRec(mouse_pos, smiley_rect);
}

