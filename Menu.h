#pragma once

#ifndef MENU_H
#define MENU_H

#include "Init_DeInit.h"
#include "Text.h"
#include"Render.h"
#include "Game_Realization.h"

//int ShowGameMenu(int* gameMode);
int ShowGameRulesMenu();
int ShowGameModeMenu(int* gameMode);
int ShowMainMenu(int* inGame, int* gameMode);
void MainLoop();


#endif