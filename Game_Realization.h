#pragma once

#include "Init_DeInit.h"
#include"Render.h"


#define BOARD_SIZE 8
#define SQUARE_SIZE 60

enum CheckerType {
	EMPTY,
	BLACK,
	WHITE
};

struct Cell {
	CheckerType type;
};


extern Cell board[BOARD_SIZE][BOARD_SIZE];


void InitializeBoard();
void drawCheckers(SDL_Renderer* renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT);
void loadmusic();
int Game();
