#pragma once

#ifndef MENU_H
#define MENU_H

#include "Init_DeInit.h"
#include"Render.h"
#include "Game_Realization.h"

int ShowGameRulesMenu();
int ShowMainMenu(int* inGame, int* gameMode);
void MainLoop();


#endif