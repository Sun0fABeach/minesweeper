#ifndef DIGIT_H
#define DIGIT_H

#define INFO_BOX_DIGIT_WIDTH 20
#define INFO_BOX_DIGIT_HEIGHT 36

extern int digital_digits_remaining_flags_x;
extern int digital_digits_remaining_flags_y;
extern int digital_digits_time_x;
extern int digital_digits_time_y;

void digital_digits_draw(int start_x, int start_y, int value);

static inline void digital_digits_draw_remaining_flags(const int value)
{
  digital_digits_draw(
    digital_digits_remaining_flags_x,
    digital_digits_remaining_flags_y,
    value
  );
}
static inline void digital_digits_draw_time(const int value)
{
  digital_digits_draw(
    digital_digits_time_x,
    digital_digits_time_y,
    value
  );
}

#endif

