#pragma once

#include "Init_DeInit.h"

void RenderButtonFrame(int x, int y, int w, int h, SDL_Color color);
void draw_Board(SDL_Texture* texture, int SCREEN_HEIGH);
void RenderText(const char* text, int x, int y, TTF_Font* font, SDL_Color color);
