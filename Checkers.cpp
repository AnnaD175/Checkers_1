﻿#include"Init_DeInit.h"
#include"Menu.h"


int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

Mix_Music* fon = NULL;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;





int main (int argc, char** argv)
{
    Init();

    MainLoop();

    DeInit(0);
    return 0;
}
