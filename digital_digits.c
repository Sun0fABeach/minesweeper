#include "digital_digits.h"
#include "textures.h"

void digital_digits_draw(const int start_x, const int start_y, int value)
{
  int n, digit_pos;

  if(value < 0) {
    n = 10;
    digit_pos = 1;
    value = -value;
    if(value > 99)
      value = 99;

    textures_draw_from_atlas(
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
    textures_draw_from_atlas(
      textures.src_rects.numbers_digital[value / n],
      start_x + INFO_BOX_DIGIT_WIDTH * digit_pos,
      start_y,
      INFO_BOX_DIGIT_WIDTH,
      INFO_BOX_DIGIT_HEIGHT
    );

    value %= n;
  }
}
