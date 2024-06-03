#pragma once

#include "Init_DeInit.h"
#include"Render.h"
#include "Text.h"
#include "Menu.h"
//#define _CRT_SECURE_NO_WARNINGS


#define BOARD_SIZE 8
#define SQUARE_SIZE 60

enum CheckerType {
	EMPTY,
	BLACK,
	WHITE
};

struct Cell {
	CheckerType type;
	bool king;
};


extern Cell board[BOARD_SIZE][BOARD_SIZE];
extern Cell board1[BOARD_SIZE][BOARD_SIZE];


void InitializeBoard();
void InitializeBoard2();
void DrawBoard(SDL_Renderer* renderer, int SCREEN_HEIGHT);
void drawCheckers(SDL_Renderer* renderer, int SCREEN_HEIGHT, int selectedX, int selectedY,  bool possibleMoves[BOARD_SIZE][BOARD_SIZE]);
bool IsValidMove(int startX, int startY, int endX, int endY, CheckerType currentPlayer);
void MovePiece(int startX, int startY, int endX, int endY, bool* mustContinue, CheckerType* currentPlayer, CheckerType* currentPlayer1);
void GetPossibleMoves(int startX, int startY, bool possibleMoves[BOARD_SIZE][BOARD_SIZE], CheckerType currentPlayer);
bool HasCaptureMoves(CheckerType currentPlayer);
void loadmusic();
void OutFail();
void Bonus(int xMenu, int yMenu, int bonusX, int bonusY, int bonus2X, int bonus2Y, int bonusWidth, int bonusHeight, int bonus2Width, int bonus2Height, CheckerType* currentPlayer, CheckerType* currentPlayer1);
void GameMenu(int xMenu, int yMenu, int title1X, int title1Y, int title2Y, int title3Y, int title3Width, int title3Height, int* running, int* d);
void CopyMas(CheckerType* currentPlayer1, CheckerType* currentPlayer);
void CopyMasBack(CheckerType* currentPlayer, CheckerType* currentPlayer1);
int Game(int currentSelection);
