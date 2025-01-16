#include "textures.h"

textures_s textures = {
  .src_rects.numbers[0] = { 25, 36, 23, 23 },
  .src_rects.numbers[1] = { 49, 36, 23, 23 },
  .src_rects.numbers[2] = { 73, 36, 23, 23 },
  .src_rects.numbers[3] = { 97, 36, 23, 23 },
  .src_rects.numbers[4] = { 121, 36, 23, 23 },
  .src_rects.numbers[5] = { 145, 36, 23, 23 },
  .src_rects.numbers[6] = { 169, 36, 23, 23 },
  .src_rects.numbers[7] = { 193, 36, 23, 23 },
  .src_rects.mine = { 97, 60, 23, 23 },
  .src_rects.mine_exploded = { 49, 60, 23, 23 },
  .src_rects.mine_false_guess = { 73, 60, 23, 23 },
  .src_rects.flag = { 27, 62, 18, 18 },
  .src_rects.smiley = { 6, 89, 28, 28 },
  .src_rects.smiley_excited = { 84, 89, 28, 28 },
  .src_rects.smiley_cool = { 162, 89, 28, 28 },
  .src_rects.smiley_dead = { 123, 89, 28, 28 }
};

void textures_load(void)
{
  textures.sprite_atlas = LoadTexture("assets/sprite_atlas.png");
  SetTextureFilter(textures.sprite_atlas, TEXTURE_FILTER_BILINEAR);
}

void textures_unload(void)
{
  UnloadTexture(textures.sprite_atlas);
}
