#pragma once

#include "Init_DeInit.h"

void RenderButtonFrame(int x, int y, int w, int h, SDL_Color color);
void RenderBack(int x, int y, int w, int h, SDL_Color color);
SDL_Texture* LoadBoardTexture();
void draw_Board(SDL_Texture* texture, int SCREEN_HEIGH);
void draw_BackGround(SDL_Texture* texture, int x, int y, int w, int h);
SDL_Texture* LoadBackGroundTexture();
