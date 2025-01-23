#include "options.h"
#include "textures.h"
#include "tile.h"

#define TOGGLE_COLLISION_BOX_PADDING 2
#define FONT_SIZE 11
#define TEXT_OFFSET_Y ((OPTIONS_BUTTON_HEIGHT - FONT_SIZE) / 2)

static inline void options_draw_toggle(void);
static inline void options_draw_dropdown(difficulty_e pressed_button);

int options_toggle_x;
int options_toggle_y;
int options_dropdown_x;
int options_dropdown_y;

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

void options_draw(const bool is_open, const difficulty_e pressed_button)
{
  options_draw_toggle();

  if(is_open)
    options_draw_dropdown(pressed_button);
}

static inline void options_draw_toggle(void)
{
  textures_draw_cogwheel(
    options_toggle_x, options_toggle_y,
    OPTIONS_TOGGLE_WIDTH, OPTIONS_TOGGLE_HEIGHT
  );
}

static inline void options_draw_dropdown(const difficulty_e pressed_button)
{
  DrawRectangle(
    options_dropdown_x, options_dropdown_y,
    OPTIONS_DROPDOWN_WIDTH, OPTIONS_DROPDOWN_HEIGHT,
    shadow_grey
  );
  int inner_x = options_dropdown_x + 1; // account for 1 px frame
  int inner_y = options_dropdown_y + 1;

  for(difficulty_e i = BEGINNER; i <= EXPERT; i++) {
    const int button_y = inner_y + OPTIONS_BUTTON_HEIGHT * i;
    int text_x = inner_x + text_config.x_offsets[i];
    int text_y = button_y + TEXT_OFFSET_Y;

    if(i == pressed_button) {
      tile_draw_revealed(
        inner_x, button_y,
        OPTIONS_BUTTON_WIDTH, OPTIONS_BUTTON_HEIGHT
      );
      text_x += 1;
      text_y += 1;
    } else {
      tile_draw_with_shadow(
        inner_x, button_y,
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

bool options_toggle_has_mouse_collision(const Vector2 mouse_pos)
{
  const Rectangle toggle_rect = {
    .x = options_toggle_x - TOGGLE_COLLISION_BOX_PADDING,
    .y = options_toggle_y - TOGGLE_COLLISION_BOX_PADDING,
    .width = OPTIONS_TOGGLE_WIDTH + TOGGLE_COLLISION_BOX_PADDING * 2,
    .height = OPTIONS_TOGGLE_HEIGHT + TOGGLE_COLLISION_BOX_PADDING * 2
  };

  return CheckCollisionPointRec(mouse_pos, toggle_rect);
}

bool options_dropdown_has_mouse_collision(const Vector2 mouse_pos)
{
  const Rectangle dropdown_rect = {
    .x = options_dropdown_x + 1, // account for 1 px frame
    .y = options_dropdown_y + 1,
    .width = OPTIONS_DROPDOWN_WIDTH - 2,
    .height = OPTIONS_DROPDOWN_HEIGHT - 2
  };

  return CheckCollisionPointRec(mouse_pos, dropdown_rect);
}

difficulty_e options_get_selected_difficulty(const int mouse_y)
{
  return (mouse_y - (options_dropdown_y + 1)) / OPTIONS_BUTTON_HEIGHT;
}
