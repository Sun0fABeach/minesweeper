#include <stddef.h>
#include "ui.h"
#include "textures.h"
#include "raylib.h"

typedef enum tile_variant {
  INSET,
  PROTRUDING,
} tile_variant_e;

static void init_layout_dimensions(difficulty_e difficulty);
static void draw_game_frame(void);
static void draw_info_box(void);
static void draw_board(
  const tile_s tiles[static NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
);
static void draw_board_tiles(
  const tile_s tiles[static NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
);
static void draw_tile_with_shadow_with_content(
  int pos_x, int pos_y,
  int width, int height,
  tile_variant_e variant,
  tile_s tile
);
static void draw_tile_with_shadow(
  int pos_x, int pos_y,
  int width, int height,
  tile_variant_e variant
);
static void draw_tile_revealed_with_content(
  int pos_x, int pos_y,
  int width, int height,
  tile_s tile
);
static void draw_tile_revealed(int pos_x, int pos_y, int width, int height);

static void check_tile_select_input(void);
static void check_difficulty_change_input(void);


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
static Color shadow_grey;
static int game_x, game_y;

static struct {
  void (*select_tile)(tile_coords_s tile_coords);
  void (*change_difficulty)(difficulty_e selected_difficulty);
} input_callbacks;

static struct {
  tile_coords_s coords;
  bool active;
} pressed_tile;


void ui_init(const difficulty_e difficulty)
{
  background_grey = ColorFromHSV(0, 0, 0.75);
  shadow_grey = ColorFromHSV(0, 0, 0.48);

  init_layout_dimensions(difficulty);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(game_frame_width, game_frame_height, "Minesweeper");
  SetTargetFPS(60);

  textures_load();
}

void ui_deinit(void)
{
  textures_unload();
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

void ui_draw_game(const game_state_s game_state[const static 1])
{
  game_x = (GetScreenWidth() - game_frame_width)/2;
  game_y = (GetScreenHeight() - game_frame_height)/2;

  BeginDrawing();
    ClearBackground(background_grey);
    draw_game_frame();
    draw_info_box();
    draw_board(game_state->tiles);
	EndDrawing();
}

static void draw_game_frame(void)
{
  draw_tile_with_shadow(
    game_x, game_y,
    game_frame_width, game_frame_height,
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
    INSET
  );
}

static void draw_board(
  const tile_s tiles[static const NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
)
{
  const int pos_x = game_x + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING;
  const int pos_y = game_y + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    INFO_BOX_HEIGHT + INFO_BOX_MARGIN_BOTTOM;

  draw_tile_with_shadow(
    pos_x,
    pos_y,
    board_width,
    board_height,
    INSET
  );
  draw_board_tiles(tiles);
}

static void draw_board_tiles(
  const tile_s tiles[static const NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
)
{
  const int board_x = game_x + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    TILE_BORDER_THICKNESS;
  const int board_y = game_y + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    INFO_BOX_HEIGHT + INFO_BOX_MARGIN_BOTTOM + TILE_BORDER_THICKNESS;

  for(int row = 0; row < num_tiles_y; row++) {
    const int tile_y = board_y + TILE_HEIGHT * row;

    for(int col = 0; col < num_tiles_x; col++) {
      const int tile_x = board_x + TILE_WIDTH * col;

      if(tiles[row][col].revealed) {
        draw_tile_revealed_with_content(
          tile_x,
          tile_y,
          TILE_WIDTH,
          TILE_HEIGHT,
          tiles[row][col]
        );
      } else {
        draw_tile_with_shadow_with_content(
          tile_x, tile_y,
          TILE_WIDTH, TILE_HEIGHT,
          PROTRUDING,
          tiles[row][col]
        );
      }
    }
  }

  if(pressed_tile.active) {
    const int col = pressed_tile.coords.x;
    const int row = pressed_tile.coords.y;

    if(!tiles[row][col].revealed)
      draw_tile_revealed(
        board_x + TILE_WIDTH * col,
        board_y + TILE_HEIGHT * row,
        TILE_WIDTH, TILE_HEIGHT
      );
  }
}

static void draw_tile_with_shadow_with_content(
  const int pos_x,
  const int pos_y,
  const int width,
  const int height,
  const tile_variant_e variant,
  const tile_s tile
)
{
  draw_tile_with_shadow(pos_x, pos_y, width, height, variant);

  if(tile.flagged) {
    DrawTexturePro(
      textures.sprite_atlas,
      textures.src_rects.flag,
      (Rectangle) {
        pos_x + TILE_BORDER_THICKNESS,
        pos_y + TILE_BORDER_THICKNESS,
        width - TILE_BORDER_THICKNESS*2,
        height - TILE_BORDER_THICKNESS*2
      },
      (Vector2) { 0, 0 }, 0, WHITE
    );
  }
}

static void draw_tile_with_shadow(
  const int pos_x,
  const int pos_y,
  const int width,
  const int height,
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

static void draw_tile_revealed_with_content(
  const int pos_x,
  const int pos_y,
  const int width,
  const int height,
  const tile_s tile
)
{
  draw_tile_revealed(pos_x, pos_y, width, height);

  const Rectangle *src_rect = NULL;

  if(tile.has_mine) {
    if(tile.bomb_exploded)
      src_rect = &textures.src_rects.bomb_exploded;
    else
      src_rect = &textures.src_rects.bomb;
  } else if(tile.flagged) {
    src_rect = &textures.src_rects.bomb_false_guess;
  } else if(tile.num_adjacent_mines > 0) {
    src_rect = &textures.src_rects.numbers[tile.num_adjacent_mines - 1];
  }

  if(src_rect) {
    DrawTexturePro(
      textures.sprite_atlas,
      *src_rect,
      (Rectangle) { pos_x + 1, pos_y + 1, width - 2, height - 2 },
      (Vector2) { 0, 0 }, 0, WHITE
    );
  }
}

static void draw_tile_revealed(
  const int pos_x,
  const int pos_y,
  const int width,
  const int height
)
{
  DrawRectangle(pos_x, pos_y, width, height, background_grey);
  DrawLine(pos_x, pos_y + 1, pos_x + width, pos_y + 1, shadow_grey);
  DrawLine(pos_x + 1, pos_y, pos_x + 1, pos_y + height, shadow_grey);
}

/*** INPUT LOGIC ***/

void ui_register_input_handlers(
  void select_tile(tile_coords_s tile_coords),
  void change_difficulty(difficulty_e selected_difficulty)
)
{
  input_callbacks.select_tile = select_tile;
  input_callbacks.change_difficulty = change_difficulty;
}

void ui_handle_inputs(void)
{
  check_tile_select_input();
  check_difficulty_change_input();
}

static void check_tile_select_input(void)
{
  pressed_tile.active = false;

  if(
    !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
    !IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
  )
    return;

  const int board_x = game_x + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    TILE_BORDER_THICKNESS;
  const int board_y = game_y + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    INFO_BOX_HEIGHT + INFO_BOX_MARGIN_BOTTOM + TILE_BORDER_THICKNESS;

  const Rectangle board_rect = {
    .x = board_x,
    .y = board_y,
    .width = board_inner_width,
    .height = board_inner_height
  };

  const Vector2 mouse_pos = GetMousePosition();

  if(!CheckCollisionPointRec(mouse_pos, board_rect))
    return;

  const tile_coords_s tile_coords = {
    .x = ((int)mouse_pos.x - board_x) / TILE_WIDTH,
    .y = ((int)mouse_pos.y - board_y) / TILE_HEIGHT
  };

  if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    pressed_tile.coords = tile_coords;
    pressed_tile.active = true;
  } else {
    input_callbacks.select_tile(tile_coords);
  }
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
