#include "ui.h"
#include "raylib.h"

typedef enum tile_variant {
  INSET,
  PROTRUDING,
} tile_variant_e;

/* fixed ui element sizes */
#define TILE_WIDTH_INNER 18
#define TILE_HEIGHT_INNER 18
#define TILE_BORDER_THICKNESS 2
#define TILE_WIDTH (TILE_WIDTH_INNER + TILE_BORDER_THICKNESS*2)
#define TILE_HEIGHT (TILE_HEIGHT_INNER + TILE_BORDER_THICKNESS*2)
#define GAME_FRAME_PADDING 8
#define INFO_BOX_MARGIN_BOTTOM GAME_FRAME_PADDING
#define INFO_BOX_HEIGHT (46 + TILE_BORDER_THICKNESS*2)

/* dynamic ui element sizes */
static int num_tiles_y;
static int num_tiles_x;
static int board_inner_width;
static int board_inner_height;
static int board_width;
static int board_height;
static int info_box_width;
static int game_frame_width;
static int game_frame_height;

static Color background_grey;
static int game_x, game_y;

static void init_layout_dimensions(difficulty_e difficulty);
static void draw_game_frame(void);
static void draw_info_box(void);
static void draw_board(void);
static void draw_board_tiles(void);
static void draw_tile_with_shadow(
  int pos_x,
  int pos_y,
  int width,
  int height,
  Color color,
  tile_variant_e variant
);
static void check_difficulty_change_input(void);

static struct {
  void (*change_difficulty)(difficulty_e selected_difficulty);
} input_callbacks;


void ui_init(const difficulty_e difficulty)
{
  background_grey = ColorFromHSV(0, 0, 0.75);

  init_layout_dimensions(difficulty);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(game_frame_width, game_frame_height, "Minesweeper");
}

void ui_deinit(void)
{
  CloseWindow();
}

void ui_change_difficulty(const difficulty_e difficulty)
{
  init_layout_dimensions(difficulty);
  SetWindowSize(game_frame_width, game_frame_height);
}

static void init_layout_dimensions(const difficulty_e difficulty)
{
  num_tiles_x = difficulty_settings[difficulty].num_tiles_x;
  num_tiles_y = difficulty_settings[difficulty].num_tiles_y;
  board_inner_width = TILE_WIDTH * num_tiles_x;
  board_inner_height = TILE_HEIGHT * num_tiles_y;
  board_width = board_inner_width + TILE_BORDER_THICKNESS*2;
  board_height = board_inner_height + TILE_BORDER_THICKNESS*2;
  info_box_width = board_width;
  game_frame_width = board_width + GAME_FRAME_PADDING*2 +
    TILE_BORDER_THICKNESS*2;
  game_frame_height = INFO_BOX_HEIGHT + INFO_BOX_MARGIN_BOTTOM +
    board_height + GAME_FRAME_PADDING*2 + TILE_BORDER_THICKNESS*2;
}

bool ui_should_close(void)
{
  return WindowShouldClose();
}

void ui_draw_game(void)
{
  game_x = (GetScreenWidth() - game_frame_width)/2;
  game_y = (GetScreenHeight() - game_frame_height)/2;

  BeginDrawing();
    ClearBackground(background_grey);
    draw_game_frame();
    draw_info_box();
		draw_board();
	EndDrawing();
}

static void draw_game_frame(void)
{
  draw_tile_with_shadow(
    game_x, game_y,
    game_frame_width, game_frame_height,
    background_grey,
    PROTRUDING
  );
}

static void draw_info_box(void)
{
  const int pos_x = game_x + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING;
  const int pos_y = game_y + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING;

  draw_tile_with_shadow(
    pos_x, pos_y,
    info_box_width, INFO_BOX_HEIGHT,
    background_grey,
    INSET
  );
}

static void draw_board(void)
{
  const int pos_x = game_x + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING;
  const int pos_y = game_y + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    INFO_BOX_HEIGHT + INFO_BOX_MARGIN_BOTTOM;

  draw_tile_with_shadow(
    pos_x,
    pos_y,
    board_width,
    board_height,
    background_grey,
    INSET
  );
  draw_board_tiles();
}

static void draw_board_tiles(void)
{
  const int pos_x = game_x + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    TILE_BORDER_THICKNESS;
  const int pos_y = game_y + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    INFO_BOX_HEIGHT + INFO_BOX_MARGIN_BOTTOM + TILE_BORDER_THICKNESS;

  for(int row = 0; row < num_tiles_y; row++) {
    for(int col = 0; col < num_tiles_x; col++) {
      draw_tile_with_shadow(
        pos_x + TILE_WIDTH * col,
        pos_y + TILE_HEIGHT * row,
        TILE_WIDTH, TILE_HEIGHT,
        background_grey,
        PROTRUDING
      );
    }
  }
}

static void draw_tile_with_shadow(
  const int pos_x,
  const int pos_y,
  const int width,
  const int height,
  const Color color,
  const tile_variant_e variant
)
{
  DrawRectangle(pos_x, pos_y, width, height, color);

  const Color shadow_grey = ColorFromHSV(0, 0, 0.48);
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

/*** INPUT LOGIC ***/

void ui_register_input_handlers(
  void change_difficulty(difficulty_e selected_difficulty)
)
{
  input_callbacks.change_difficulty = change_difficulty;
}

void ui_handle_inputs(void)
{
  check_difficulty_change_input();
}

static void check_difficulty_change_input(void)
{
  switch(GetKeyPressed()) {
    case KEY_NULL:
      return; // no key pressed
    case KEY_ONE:
      input_callbacks.change_difficulty(BEGINNER);
      break;
    case KEY_TWO:
      input_callbacks.change_difficulty(INTERMEDIATE);
      break;
    case KEY_THREE:
      input_callbacks.change_difficulty(EXPERT);
      break;
    default:
      return;
  }
}
