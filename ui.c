#include "ui.h"
#include "tile.h"
#include "smiley.h"
#include "options.h"
#include "digital_digits.h"
#include "board.h"
#include "board_tile.h"
#include "textures.h"

static void set_layout_dimensions(int rows, int cols);
static void set_ui_element_offsets(void);
static inline void draw_game_frame(void);
static inline void draw_info_box(const game_state_s *game_state);
static inline void draw_remaining_flags_indicator(int remaining_flags);
static inline void draw_time_indicator(int elapsed_secs);
static inline void draw_smiley(const game_state_s *game_state);
static inline void draw_options(void);
static inline void draw_board(
  const board_tile_s board[static NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
);

static bool check_options_open_input(void);
static bool check_options_dropdown_input(void);
static bool check_smiley_input(void);
static bool check_board_input(void);


/* fixed ui element sizes */
#define GAME_FRAME_PADDING 8
#define INFO_BOX_MARGIN_BOTTOM GAME_FRAME_PADDING
#define INFO_BOX_HEIGHT_INNER 50
#define INFO_BOX_HEIGHT (INFO_BOX_HEIGHT_INNER + TILE_BORDER_THICKNESS*2)
#define INFO_BOX_OPTIONS_TOGGLE_MARGIN_LEFT 2
#define INFO_BOX_OPTIONS_TOGGLE_MARGIN_TOP 4
#define INFO_BOX_DIGIT_PADDING_Y \
  ((INFO_BOX_HEIGHT_INNER - INFO_BOX_DIGIT_HEIGHT) / 2)
#define INFO_BOX_DIGIT_PADDING_X INFO_BOX_DIGIT_PADDING_Y
/* dynamic ui element sizes */
static int info_box_width;
static int game_frame_width;
static int game_frame_height;

static int game_x, game_y;
static int info_box_x, info_box_y;

static struct {
  void (*select_board_tile)(int row, int col);
  void (*flag_board_tile)(int row, int col);
  void (*push_smiley)(void);
  void (*change_difficulty)(difficulty_e selected_difficulty);
} input_callbacks;


void ui_init(const int num_rows, const int num_cols)
{
  tile_init();
  set_layout_dimensions(num_rows, num_cols);

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
  set_layout_dimensions(num_rows, num_cols);
  SetWindowSize(game_frame_width, game_frame_height);
}

static void set_layout_dimensions(const int rows, const int cols)
{
  board_num_rows = rows;
  board_num_cols = cols;
  board_inner_width = BOARD_TILE_WIDTH * board_num_cols;
  board_inner_height = BOARD_TILE_HEIGHT * board_num_rows;
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
  set_ui_element_offsets();

  BeginDrawing();
    ClearBackground(background_grey);
    draw_game_frame();
    draw_board(game_state->board);
    draw_info_box(game_state); // needs to be last to draw
	EndDrawing();
}

void set_ui_element_offsets(void)
{
  game_x = (GetScreenWidth() - game_frame_width)/2;
  game_y = (GetScreenHeight() - game_frame_height)/2;
  info_box_x = game_x + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING;
  info_box_y = game_y + TILE_BORDER_THICKNESS + GAME_FRAME_PADDING;
  smiley_x = info_box_x + (info_box_width - SMILEY_WIDTH) / 2;
  smiley_y = info_box_y + (INFO_BOX_HEIGHT - SMILEY_HEIGHT) / 2;
  options_toggle_x = smiley_x + SMILEY_WIDTH +
    INFO_BOX_OPTIONS_TOGGLE_MARGIN_LEFT;
  options_toggle_y = smiley_y + INFO_BOX_OPTIONS_TOGGLE_MARGIN_TOP;
  options_dropdown_x = options_toggle_x + OPTIONS_DROPDOWN_MARGIN_LEFT;
  options_dropdown_y = options_toggle_y + OPTIONS_TOGGLE_HEIGHT +
    OPTIONS_DROPDOWN_MARGIN_TOP;
  digital_digits_remaining_flags_x = info_box_x + TILE_BORDER_THICKNESS +
    INFO_BOX_DIGIT_PADDING_X;
  digital_digits_remaining_flags_y = info_box_y + TILE_BORDER_THICKNESS +
    INFO_BOX_DIGIT_PADDING_Y;
  digital_digits_time_x = info_box_x + info_box_width - TILE_BORDER_THICKNESS -
    INFO_BOX_DIGIT_PADDING_X - INFO_BOX_DIGIT_WIDTH*3;
  digital_digits_time_y = info_box_y + TILE_BORDER_THICKNESS +
    INFO_BOX_DIGIT_PADDING_Y;
  board_x = info_box_x;
  board_y = info_box_y + INFO_BOX_HEIGHT + INFO_BOX_MARGIN_BOTTOM;

}

static inline void draw_game_frame(void)
{
  tile_draw_with_shadow(
    game_x, game_y,
    game_frame_width, game_frame_height,
    PROTRUDING
  );
}

static void draw_info_box(const game_state_s *const game_state)
{
  tile_draw_with_shadow(
    info_box_x, info_box_y,
    info_box_width, INFO_BOX_HEIGHT,
    INSET
  );
  draw_remaining_flags_indicator(game_state->remaining_flags);
  draw_time_indicator(game_state->elapsed_time_secs);
  draw_smiley(game_state);
  draw_options();
}

static inline void draw_remaining_flags_indicator(const int remaining_flags)
{
  digital_digits_draw_remaining_flags(remaining_flags);
}

static inline void draw_time_indicator(const int elapsed_secs)
{
  digital_digits_draw_time(elapsed_secs);
}

static inline void draw_smiley(const game_state_s *const game_state)
{
  smiley_type_e type = SMILEY_HAPPY;
  if(game_state->won)
    type = SMILEY_COOL;
  else if(game_state->game_over)
    type = SMILEY_DEAD;
  else if(board_pressed_tile.active)
    type = SMILEY_EXCITED;

  smiley_draw(type);
}

static inline void draw_options(void)
{
  options_draw();
}

static inline void draw_board(
  const board_tile_s board[static const NUM_TILES_Y_EXPERT][NUM_TILES_X_EXPERT]
)
{
  board_draw(board);
}

/*** INPUT LOGIC ***/

void ui_register_input_handlers(
  void select_board_tile(int row, int col),
  void flag_board_tile(int row, int col),
  void push_smiley(void),
  void change_difficulty(difficulty_e selected_difficulty)
)
{
  input_callbacks.select_board_tile = select_board_tile;
  input_callbacks.flag_board_tile = flag_board_tile;
  input_callbacks.push_smiley = push_smiley;
  input_callbacks.change_difficulty = change_difficulty;
}

void ui_handle_inputs(void)
{
  if(options_capture_inputs) {
    check_options_dropdown_input();
  } else {
    if(!check_board_input())
      if(!check_smiley_input())
        check_options_open_input();
  }
}

static bool check_options_open_input(void)
{
  if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    return false;

  if(!options_toggle_has_mouse_collision(GetMousePosition()))
    return false;

  options_open = true;
  options_capture_inputs = true;
  options_pressed_difficulty = DIFFICULTY_NONE;
  return true;
}

static bool check_options_dropdown_input(void)
{
  if(
    !IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
    !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
    !IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
  )
    return false;

  // behavior: left-click outside dropdown closes it visually, but only a
  // subsequent mouse button release opens the remaining ui for inputs again
  if(!options_open) {
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
      options_capture_inputs = false;
    return false;
  }

  const Vector2 mouse_pos = GetMousePosition();

  if(options_dropdown_has_mouse_collision(mouse_pos)) {
    const difficulty_e selected_difficulty =
      options_get_selected_difficulty(mouse_pos.y);

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      options_pressed_difficulty = selected_difficulty;
    } else { // can only be released mouse button from here
      input_callbacks.change_difficulty(selected_difficulty);
      options_open = false;
      options_capture_inputs = false;
      options_pressed_difficulty = DIFFICULTY_NONE;
    }
  } else {
    options_pressed_difficulty = DIFFICULTY_NONE;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      options_open = false;
    return false;
  }

  return true;
}

static bool check_smiley_input(void)
{
  smiley_is_pressed = false;

  if(
    !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
    !IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
  )
    return false;

  if(!smiley_has_mouse_collision(GetMousePosition()))
    return false;

  if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    smiley_is_pressed = true;
  else
    input_callbacks.push_smiley();

  return true;
}

static bool check_board_input(void)
{
  board_pressed_tile.active = false;

  if(
    !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
    !IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
    !IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)
  )
    return false;

  const Vector2 mouse_pos = GetMousePosition();

  if(!board_has_mouse_collision(mouse_pos))
    return false;

  const int row = board_get_row(mouse_pos.y);
  const int col = board_get_col(mouse_pos.x);

  if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    board_pressed_tile.row = row;
    board_pressed_tile.col = col;
    board_pressed_tile.active = true;
  } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    input_callbacks.select_board_tile(row, col);
  } else {
    input_callbacks.flag_board_tile(row, col);
  }

  return true;
}

