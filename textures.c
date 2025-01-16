#include "textures.h"

textures_s textures = {
  .src_rects.numbers[0] = { 1, 69, 15, 15 },
  .src_rects.numbers[1] = { 18, 69, 15, 15 },
  .src_rects.numbers[2] = { 35, 69, 15, 15 },
  .src_rects.numbers[3] = { 52, 69, 15, 15 },
  .src_rects.numbers[4] = { 69, 69, 15, 15 },
  .src_rects.numbers[5] = { 86, 69, 15, 15 },
  .src_rects.numbers[6] = { 103, 69, 15, 15 },
  .src_rects.numbers[7] = { 120, 69, 15, 15 },
  .src_rects.mine = { 86, 52, 15, 15 },
  .src_rects.mine_exploded = { 103, 52, 15, 15 },
  .src_rects.mine_false_guess = { 120, 52, 15, 15 },
  .src_rects.flag = { 36, 53, 12, 12 },
  .src_rects.smiley = { 4, 28, 19, 19 },
  .src_rects.smiley_excited = { 58, 28, 19, 19 },
  .src_rects.smiley_cool = { 85, 28, 19, 19 },
  .src_rects.smiley_dead = { 112, 28, 19, 19 }
};

void textures_load(void)
{
  textures.sprite_atlas = LoadTexture("assets/sprite_atlas.png");
}

void textures_unload(void)
{
  UnloadTexture(textures.sprite_atlas);
}
