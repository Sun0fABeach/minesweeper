#include <stddef.h>
#include "ui.h"
#include "tile.h"
#include "textures.h"

static void init_layout_dimensions(int rows, int cols);
static void draw_game_frame(void);
static void draw_info_box(const game_state_s *game_state);
static void draw_remaining_mines_indicator(
  int board_x, int board_y, int remaining_mines
);
static void draw_time_indicator(
  int info_box_x, int info_box_y, int elapsed_secs
);
static void draw_info_box_indicator(int start_x, int start_y, int value);
static void draw_smiley(
  int info_box_x, int info_box_y,
  const game_state_s *game_state
);
static void draw_board(
  const tile_s tiles[static NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
);
static void draw_board_tiles(
  const tile_s tiles[static NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
);
static void draw_board_tile_with_shadow(
  int pos_x, int pos_y,
  int width, int height,
  tile_variant_e variant,
  tile_s tile
);
static void draw_board_tile_revealed(
  int pos_x, int pos_y,
  int width, int height,
  tile_s tile
);

static void check_tile_select_input(void);
static void check_smiley_input(void);
static void check_difficulty_change_input(void);


/* fixed ui element sizes */
#define BOARD_TILE_WIDTH_INNER 20
#define BOARD_TILE_HEIGHT_INNER 20
#define BOARD_TILE_WIDTH (BOARD_TILE_WIDTH_INNER + TILE_BORDER_THICKNESS*2)
#define BOARD_TILE_HEIGHT (BOARD_TILE_HEIGHT_INNER + TILE_BORDER_THICKNESS*2)
#define GAME_FRAME_PADDING 8
#define SMILEY_WIDTH_INNER 32
#define SMILEY_HEIGHT_INNER 32
#define SMILEY_PADDING 2
#define SMILEY_WIDTH (SMILEY_WIDTH_INNER + 2 + TILE_BORDER_THICKNESS*2 + \
  SMILEY_PADDING*2)
#define SMILEY_HEIGHT (SMILEY_HEIGHT_INNER + 2 + TILE_BORDER_THICKNESS*2 + \
  SMILEY_PADDING*2)
#define INFO_BOX_MARGIN_BOTTOM GAME_FRAME_PADDING
#define INFO_BOX_HEIGHT_INNER 50
#define INFO_BOX_HEIGHT (INFO_BOX_HEIGHT_INNER + TILE_BORDER_THICKNESS*2)
#define INFO_BOX_DIGIT_WIDTH 20
#define INFO_BOX_DIGIT_HEIGHT 36
#define INFO_BOX_DIGIT_PADDING_Y \
  ((INFO_BOX_HEIGHT_INNER - INFO_BOX_DIGIT_HEIGHT) / 2)
#define INFO_BOX_DIGIT_PADDING_X INFO_BOX_DIGIT_PADDING_Y

/* dynamic ui element sizes */
static int num_rows;
static int num_cols;
static int board_inner_width;
static int board_inner_height;
static int board_width;
static int board_height;
static int info_box_width;
static int game_frame_width;
static int game_frame_height;

static int game_x, game_y;

static struct {
  void (*select_tile)(int row, int col);
  void (*flag_tile)(int row, int col);
  void (*press_smiley)(void);
  void (*change_difficulty)(difficulty_e selected_difficulty);
} input_callbacks;

static bool pressed_smiley;
static struct {
  int row, col;
  bool active;
} pressed_tile;


void ui_init(const int num_rows, const int num_cols)
{
  tile_init();
  init_layout_dimensions(num_rows, num_cols);

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

void ui_change_difficulty(const int num_rows, const int num_cols)
{
  init_layout_dimensions(num_rows, num_cols);
  SetWindowSize(game_frame_width, game_frame_height);
}

static void init_layout_dimensions(const int rows, const int cols)
{
  num_rows = rows;
  num_cols = cols;
  board_inner_width = BOARD_TILE_WIDTH * num_cols;
  board_inner_height = BOARD_TILE_HEIGHT * num_rows;
  board_width = board_inner_width + TILE_BORDER_THICKNESS*2;
  board_height = board_inner_height + TILE_BORDER_THICKNESS*2;
  info_box_width = board_width;
  game_frame_width = board_width + GAME_FRAME_PADDING*2 +
    TILE_BORDER_THICKNESS*2;
  game_frame_height = INFO_BOX_HEIGHT + INFO_BOX_MARGIN_BOTTOM +
    board_height + GAME_FRAME_PADDING*2 + TILE_BORDER_THICKNESS*2;
}

void ui_draw_game(const game_state_s game_state[const static 1])
{
  game_x = (GetScreenWidth() - game_frame_width)/2;
  game_y = (GetScreenHeight() - game_frame_height)/2;

  BeginDrawing();
    ClearBackground(background_grey);
    draw_game_frame();
    draw_info_box(game_state);
    draw_board(game_state->tiles);
	EndDrawing();
}

static void draw_game_frame(void)
{
  tile_draw_with_shadow(
    game_x, game_y,
    game_frame_width, game_frame_height,
    PROTRUDING
  );
}

static void draw_info_box(const game_state_s *const game_state)
{
  const int pos_x = game_x + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING;
  const int pos_y = game_y + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING;

  tile_draw_with_shadow(
    pos_x, pos_y,
    info_box_width, INFO_BOX_HEIGHT,
    INSET
  );
  draw_remaining_mines_indicator(pos_x, pos_y, game_state->remaining_flags);
  draw_time_indicator(pos_x, pos_y, game_state->elapsed_time_secs);
  draw_smiley(pos_x, pos_y, game_state);
}

static void draw_remaining_mines_indicator(
  const int info_box_x, const int info_box_y, const int remaining_mines
)
{
  const int start_x = info_box_x + TILE_BORDER_THICKNESS +
    INFO_BOX_DIGIT_PADDING_X;
  const int start_y = info_box_y + TILE_BORDER_THICKNESS +
    INFO_BOX_DIGIT_PADDING_Y;

  draw_info_box_indicator(start_x, start_y, remaining_mines);
}

static void draw_time_indicator(
  const int info_box_x, const int info_box_y, const int elapsed_secs
)
{
  const int start_x = info_box_x + info_box_width - TILE_BORDER_THICKNESS -
    INFO_BOX_DIGIT_PADDING_X - INFO_BOX_DIGIT_WIDTH*3;
  const int start_y = info_box_y + TILE_BORDER_THICKNESS +
    INFO_BOX_DIGIT_PADDING_Y;

  draw_info_box_indicator(start_x, start_y, elapsed_secs);
}

static void draw_info_box_indicator(
  const int start_x, const int start_y, int value
)
{
  int n, digit_pos;

  if(value < 0) {
    n = 10;
    digit_pos = 1;
    value = -value;
    if(value > 99)
      value = 99;

    draw_from_texture_atlas(
      textures.src_rects.digital_minus,
      start_x, start_y,
      INFO_BOX_DIGIT_WIDTH, INFO_BOX_DIGIT_HEIGHT
    );
  } else {
    n = 100;
    digit_pos = 0;
    if(value > 999)
      value = 999;
  }

  for(;n > 0; n /= 10, digit_pos++) {
    draw_from_texture_atlas(
      textures.src_rects.numbers_digital[value / n],
      start_x + INFO_BOX_DIGIT_WIDTH * digit_pos,
      start_y,
      INFO_BOX_DIGIT_WIDTH,
      INFO_BOX_DIGIT_HEIGHT
    );

    value %= n;
  }
}

static void draw_smiley(
  const int info_box_x, const int info_box_y,
  const game_state_s *const game_state
)
{
  const int pos_x = info_box_x + (info_box_width - SMILEY_WIDTH) / 2;
  const int pos_y = info_box_y + (INFO_BOX_HEIGHT - SMILEY_HEIGHT) / 2;
  int texture_x, texture_y;
  Rectangle src_rect;

  DrawRectangle(pos_x, pos_y, SMILEY_WIDTH, SMILEY_HEIGHT, shadow_grey);

  if(pressed_smiley) {
    tile_draw_revealed(
      pos_x + 1,
      pos_y + 1,
      SMILEY_WIDTH - 2,
      SMILEY_HEIGHT - 2
    );
    texture_x = pos_x + 1 + TILE_BORDER_THICKNESS*2 + SMILEY_PADDING;
    texture_y = pos_y + 1 + TILE_BORDER_THICKNESS*2 + SMILEY_PADDING;
    src_rect = textures.src_rects.smiley;
  } else {
    tile_draw_with_shadow(
      pos_x + 1,
      pos_y + 1,
      SMILEY_WIDTH - 2,
      SMILEY_HEIGHT - 2,
      PROTRUDING
    );
    texture_x = pos_x + 1 + TILE_BORDER_THICKNESS + SMILEY_PADDING;
    texture_y = pos_y + 1 + TILE_BORDER_THICKNESS + SMILEY_PADDING;
    if(game_state->won)
      src_rect = textures.src_rects.smiley_cool;
    else if(game_state->game_over)
      src_rect = textures.src_rects.smiley_dead;
    else if(pressed_tile.active)
      src_rect = textures.src_rects.smiley_excited;
    else
      src_rect = textures.src_rects.smiley;
  }

  draw_from_texture_atlas(
    src_rect,
    texture_x, texture_y,
    SMILEY_WIDTH_INNER, SMILEY_HEIGHT_INNER
  );
}

static void draw_board(
  const tile_s tiles[static const NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
)
{
  const int pos_x = game_x + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING;
  const int pos_y = game_y + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    INFO_BOX_HEIGHT + INFO_BOX_MARGIN_BOTTOM;

  tile_draw_with_shadow(
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

  for(int row = 0; row < num_rows; row++) {
    const int tile_y = board_y + BOARD_TILE_HEIGHT * row;

    for(int col = 0; col < num_cols; col++) {
      const int tile_x = board_x + BOARD_TILE_WIDTH * col;

      if(tiles[row][col].revealed) {
        draw_board_tile_revealed(
          tile_x, tile_y,
          BOARD_TILE_WIDTH, BOARD_TILE_HEIGHT,
          tiles[row][col]
        );
      } else {
        draw_board_tile_with_shadow(
          tile_x, tile_y,
          BOARD_TILE_WIDTH, BOARD_TILE_HEIGHT,
          PROTRUDING,
          tiles[row][col]
        );
      }
    }
  }

  if(pressed_tile.active) {
    const int col = pressed_tile.col;
    const int row = pressed_tile.row;

    if(!tiles[row][col].revealed && !tiles[row][col].flagged)
      tile_draw_revealed(
        board_x + BOARD_TILE_WIDTH * col,
        board_y + BOARD_TILE_HEIGHT * row,
        BOARD_TILE_WIDTH, BOARD_TILE_HEIGHT
      );
  }
}

static void draw_board_tile_with_shadow(
  const int pos_x, const int pos_y,
  const int width, const int height,
  const tile_variant_e variant,
  const tile_s tile
)
{
  tile_draw_with_shadow(pos_x, pos_y, width, height, variant);

  if(tile.flagged) {
    draw_from_texture_atlas(
      textures.src_rects.flag,
      pos_x + TILE_BORDER_THICKNESS,
      pos_y + TILE_BORDER_THICKNESS,
      width - TILE_BORDER_THICKNESS*2,
      height - TILE_BORDER_THICKNESS*2
    );
  }
}

static void draw_board_tile_revealed(
  const int pos_x, const int pos_y,
  const int width, const int height,
  const tile_s tile
)
{
  tile_draw_revealed(pos_x, pos_y, width, height);

  const Rectangle *src_rect = NULL;

  if(tile.has_mine) {
    if(tile.mine_exploded)
      src_rect = &textures.src_rects.mine_exploded;
    else
      src_rect = &textures.src_rects.mine;
  } else if(tile.flagged) {
    src_rect = &textures.src_rects.mine_false_guess;
  } else if(tile.num_adjacent_mines > 0) {
    src_rect = &textures.src_rects.numbers[tile.num_adjacent_mines - 1];
  }

  if(src_rect) {
    draw_from_texture_atlas(
      *src_rect,
      pos_x + 1, pos_y + 1,
      width - 1, height - 1
    );
  }
}

/*** INPUT LOGIC ***/

void ui_register_input_handlers(
  void select_tile(int row, int col),
  void flag_tile(int row, int col),
  void press_smiley(void),
  void change_difficulty(difficulty_e selected_difficulty)
)
{
  input_callbacks.select_tile = select_tile;
  input_callbacks.flag_tile = flag_tile;
  input_callbacks.press_smiley = press_smiley;
  input_callbacks.change_difficulty = change_difficulty;
}

void ui_handle_inputs(void)
{
  check_tile_select_input();
  check_smiley_input();
  check_difficulty_change_input();
}

static void check_tile_select_input(void)
{
  pressed_tile.active = false;

  if(
    !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
    !IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
    !IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)
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

  const int row = ((int)mouse_pos.y - board_y) / BOARD_TILE_HEIGHT;
  const int col = ((int)mouse_pos.x - board_x) / BOARD_TILE_WIDTH;

  if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    pressed_tile.row = row;
    pressed_tile.col = col;
    pressed_tile.active = true;
  } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    input_callbacks.select_tile(row, col);
  } else {
    input_callbacks.flag_tile(row, col);
  }
}

static void check_smiley_input(void)
{
  pressed_smiley = false;

  if(
    !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
    !IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
  )
    return;

  const int pos_x = game_x + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    (info_box_width - SMILEY_WIDTH) / 2;
  const int pos_y = game_y + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING +
    (INFO_BOX_HEIGHT - SMILEY_HEIGHT) / 2;

  const Rectangle smiley_rect = {
    .x = pos_x,
    .y = pos_y,
    .width = SMILEY_WIDTH,
    .height = SMILEY_HEIGHT
  };

  const Vector2 mouse_pos = GetMousePosition();

  if(!CheckCollisionPointRec(mouse_pos, smiley_rect))
    return;

  if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    pressed_smiley = true;
  else
    input_callbacks.press_smiley();
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
