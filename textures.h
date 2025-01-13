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
    Rectangle smiley;
    Rectangle smiley_excited;
    Rectangle smiley_cool;
    Rectangle smiley_dead;
    Rectangle bomb;
    Rectangle bomb_exploded;
    Rectangle bomb_false_guess;
    Rectangle flag;
  } src_rects;
} textures_s;

extern textures_s textures;

#endif
