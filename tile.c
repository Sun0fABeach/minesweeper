#include "tile.h"

Color background_grey;
Color shadow_grey;


void tile_init(void)
{
  background_grey = ColorFromHSV(0, 0, 0.75);
  shadow_grey = ColorFromHSV(0, 0, 0.48);
}

void tile_draw_revealed(
  const int pos_x, const int pos_y, const int width, const int height
)
{
  DrawRectangle(pos_x, pos_y, width, height, background_grey);
  DrawLine(pos_x, pos_y + 1, pos_x + width, pos_y + 1, shadow_grey);
  DrawLine(pos_x + 1, pos_y, pos_x + 1, pos_y + height, shadow_grey);
}

void tile_draw_with_shadow(
  const int pos_x, const int pos_y,
  const int width, const int height,
  const tile_variant_e variant
)
{
  DrawRectangle(pos_x, pos_y, width, height, background_grey);

  const Color top_left_lighting = variant == INSET ? shadow_grey : WHITE;
  const Color bottom_right_lighting = variant == INSET ? WHITE : shadow_grey;

  constexpr int half_border_thickness = TILE_BORDER_THICKNESS/2;

  const int offset_x_left = pos_x + half_border_thickness;
  const int offset_x_right = pos_x + width - half_border_thickness;
  const int offset_y_top = pos_y + half_border_thickness;
  const int offset_y_bottom = pos_y + height - half_border_thickness;

  // top left -> top right
  DrawLineEx(
    (Vector2) { pos_x, offset_y_top },
    (Vector2) { offset_x_right, offset_y_top },
    TILE_BORDER_THICKNESS, top_left_lighting
  );
  // top left -> bottom left
  DrawLineEx(
    (Vector2) { offset_x_left, pos_y },
    (Vector2) { offset_x_left, pos_y + height},
    TILE_BORDER_THICKNESS, top_left_lighting
  );
  // bottom left -> bottom right
  DrawLineEx(
    (Vector2) { pos_x, offset_y_bottom },
    (Vector2) { pos_x + width, offset_y_bottom },
    TILE_BORDER_THICKNESS, bottom_right_lighting
  );
  // top right -> bottom right
  DrawLineEx(
    (Vector2) { offset_x_right, pos_y },
    (Vector2) { offset_x_right, pos_y + height },
    TILE_BORDER_THICKNESS, bottom_right_lighting
  );

  // bottom left corner
  const int inner_y = pos_y + height - TILE_BORDER_THICKNESS;
  DrawTriangle(
    (Vector2) { pos_x, pos_y + height },
    (Vector2) { pos_x + TILE_BORDER_THICKNESS, inner_y },
    (Vector2) { pos_x, inner_y },
    top_left_lighting
  );
  // top right corner
  const int inner_x = pos_x + width - TILE_BORDER_THICKNESS;
  DrawTriangle(
    (Vector2) { inner_x, pos_y + TILE_BORDER_THICKNESS },
    (Vector2) { pos_x + width, pos_y },
    (Vector2) { inner_x, pos_y },
    top_left_lighting
  );
}
