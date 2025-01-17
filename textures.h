#ifndef TEXTURES_H
#define TEXTURES_H

#include "raylib.h"

void textures_load(void);
void textures_unload(void);

typedef struct textures {
  Texture2D sprite_atlas;
  const struct {
    Rectangle numbers[8];
    Rectangle numbers_digital[10];
    Rectangle digital_minus;
    Rectangle mine;
    Rectangle mine_exploded;
    Rectangle mine_false_guess;
    Rectangle flag;
    Rectangle smiley;
    Rectangle smiley_excited;
    Rectangle smiley_cool;
    Rectangle smiley_dead;
  } src_rects;
} textures_s;

extern textures_s textures;

#endif
