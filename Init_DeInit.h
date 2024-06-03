#pragma once

#ifndef INIT_DEINIT_H
#define INIT_DEINIT_H

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string.h>
#include <stdlib.h>
#include<locale.h>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern Mix_Music* fon;

void DeInit(int error);
void Init();

#endif