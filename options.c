#include "raylib.h"
#include "options.h"
#include "textures.h"
#include "tile.h"

#define FONT_SIZE 11
#define TEXT_OFFSET_Y ((OPTIONS_BUTTON_HEIGHT - FONT_SIZE) / 2)

static inline void options_draw_toggle(int start_x, int start_y);
static inline void options_draw_dropdown(
  int start_x, int start_y, difficulty_e pressed_button
);

static const struct {
  const char *texts[3];
  int x_offsets[3];
} text_config = {
  .texts = {
    [BEGINNER] = "Beginner",
    [INTERMEDIATE] = "Intermediate",
    [EXPERT] = "Expert"
  },
  .x_offsets = {
    [BEGINNER] = 17,
    [INTERMEDIATE] = 6,
    [EXPERT] = 22
  }
};

void options_draw(
  const int start_x, const int start_y,
  const bool is_open, const difficulty_e pressed_button
)
{
  options_draw_toggle(start_x, start_y);

  if(is_open)
    options_draw_dropdown(
      start_x + OPTIONS_DROPDOWN_MARGIN_LEFT,
      start_y + OPTIONS_TOGGLE_HEIGHT + OPTIONS_DROPDOWN_MARGIN_TOP,
      pressed_button
    );
}

static inline void options_draw_toggle(const int start_x, const int start_y)
{
  textures_draw_cogwheel(
    start_x, start_y,
    OPTIONS_TOGGLE_WIDTH, OPTIONS_TOGGLE_HEIGHT
  );
}

static inline void options_draw_dropdown(
  int dropdown_x, int dropdown_y, const difficulty_e pressed_button
)
{
  DrawRectangle(
    dropdown_x, dropdown_y,
    OPTIONS_DROPDOWN_WIDTH, OPTIONS_DROPDOWN_HEIGHT,
    shadow_grey
  );
  dropdown_x += 1;
  dropdown_y += 1;

  for(difficulty_e i = BEGINNER; i <= EXPERT; i++) {
    const int button_y = dropdown_y + OPTIONS_BUTTON_HEIGHT * i;
    int text_x = dropdown_x + text_config.x_offsets[i];
    int text_y = button_y + TEXT_OFFSET_Y;

    if(i == pressed_button) {
      tile_draw_revealed(
        dropdown_x, button_y,
        OPTIONS_BUTTON_WIDTH, OPTIONS_BUTTON_HEIGHT
      );
      text_x += 1;
      text_y += 1;
    } else {
      tile_draw_with_shadow(
        dropdown_x, button_y,
        OPTIONS_BUTTON_WIDTH, OPTIONS_BUTTON_HEIGHT,
        PROTRUDING
      );
    }

    DrawText(
      text_config.texts[i],
      text_x, text_y,
      FONT_SIZE,
      BLACK
    );
  }
}
