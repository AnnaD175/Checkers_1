#include "Menu.h"

//int ShowGameMenu(int* gameMode)
//{
//	SDL_Event event;
//	int running = 1;
//
//	TTF_Font* Titlefont = TTF_OpenFont("Title2.ttf", 40);
//	TTF_Font* font = TTF_OpenFont("Title2.ttf", 24);
//
//
//	SDL_Color brown = { 139, 69, 19, 255 };
//	SDL_Color brown2 = { 210, 180, 140, 255 };
//	SDL_Color white = { 255, 255, 255, 255 };
//	SDL_Color black = { 0, 0, 0, 255 };
//
//	int currentSelection = 0;
//	const char* GameMenu[] =
//	{
//		"New Game",
//		"Save the game",
//		"Finish the game",
//	};
//	int GameMenuCount = sizeof(GameMenu) / sizeof(GameMenu[0]);
//
//	while (running)
//	{
//		// Получить размеры окна
//		int windowWidth, windowHeight;
//		SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
//
//		// Определение размеров текста
//		int totalHeight = 0;
//		int textWidth = 0;
//		int maxWidth = 0;
//		for (int i = 0; i < GameMenuCount; i++)
//		{
//			int width, height;
//			TTF_SizeText(font, GameMenu[i], &width, &height);
//			totalHeight += height + 10;
//			if (width > maxWidth)
//			{
//				maxWidth = width;
//			}
//		}
//		totalHeight -= 10; // Убираем последний отступ
//
//		// Координаты для центрирования
//		int startX = (windowWidth - maxWidth)*(0.8);
//		int startY = (windowHeight - totalHeight) * (0.8);
//
//		while (SDL_PollEvent(&event))
//		{
//			if (event.type == SDL_QUIT)
//			{
//				running = 0;
//				*gameMode = -1; // Выход из игры
//			}
//			else if (event.type == SDL_KEYDOWN)
//			{
//				switch (event.key.keysym.sym)
//				{
//				case SDLK_UP:
//					currentSelection = (currentSelection - 1 + GameMenuCount) % GameMenuCount;
//					break;
//				case SDLK_DOWN:
//					currentSelection = (currentSelection + 1) % GameMenuCount;
//					break;
//				case SDLK_RETURN:
//					if (currentSelection == 0)
//					{
//						//*gameMode = 1;
//						running = 0;
//						Game();
//					}
//					else if (currentSelection == 1)
//					{
//						//*gameMode = 2;
//						//running = 0;
//					}
//					else if (currentSelection == 2)
//					{
//						*gameMode = 0; // Назад в меню
//						running = 0;
//					}
//					break;
//				}
//			}
//			else if (event.type == SDL_MOUSEMOTION)
//			{
//				int mouseX = event.motion.x;
//				int mouseY = event.motion.y;
//				int y = startY;
//				for (int i = 0; i < GameMenuCount; i++)
//				{
//					int width, height;
//					TTF_SizeText(font, GameMenu[i], &width, &height);
//					if (mouseX >= startX && mouseX <= startX + width && mouseY >= y && mouseY <= y + height)
//					{
//						currentSelection = i;
//					}
//					y += height + 10;
//				}
//			}
//			else if (event.type == SDL_MOUSEBUTTONDOWN)
//			{
//				if (event.button.button == SDL_BUTTON_LEFT)
//				{
//					if (currentSelection == 0)
//					{
//						*gameMode = 1; // Классический режим
//						running = 0;
//						Game();
//					}
//					else if (currentSelection == 1)
//					{
//						//*gameMode = 2; // Режим на время
//						//running = 0;
//					}
//					else if (currentSelection == 2)
//					{
//						*gameMode = 0; // Назад в меню
//						running = 0;
//					}
//				}
//			}
//		}
//
//		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//		SDL_RenderClear(renderer);
//
//
//		// Отрисовка Menu
//		int titleWidth, titleHeight;
//		TTF_SizeText(font, "Menu", &titleWidth, &titleHeight);
//		int titleX = (windowWidth - titleWidth -40);
//		int titleY = windowHeight-200;
//		RenderText("Menu", titleX, titleY, Titlefont, black);
//
//		int y = startY;
//		for (int i = 0; i < GameMenuCount; i++)
//		{
//			int width, height;
//			TTF_SizeText(font, GameMenu[i], &width, &height);
//			RenderText(GameMenu[i], startX, y, font, (i == currentSelection) ? brown2 : brown);
//			y += height + 10;
//		}
//		RenderButtonFrame(startX - 20, startY - 10, maxWidth + 40, totalHeight + 20, black);
//
//
//		SDL_RenderPresent(renderer);
//	}
//
//	TTF_CloseFont(font);
//
//	return *gameMode;
//}

// Функция для показа правил игры
int ShowGameRulesMenu()
{
	SDL_Event event;
	int running = 1;

	TTF_Font* font = TTF_OpenFont("Title2.ttf", 14);
	TTF_Font* TitleFont = TTF_OpenFont("Title2.ttf", 64);

	SDL_Color brown = { 139, 69, 19, 255 };
	SDL_Color brown2 = { 210, 180, 140, 255 };
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color blue = { 82, 255, 255, 255 };
	SDL_Color purple = { 252, 86, 254, 255 };
	SDL_Color red = { 252, 0, 0, 255 };
	SDL_Color black = { 0, 0, 0, 255 };

	const char* rulesText[] =
	{
		
		"1. The goal of the game is to capture all the opponent's checkers",
		" or deprive him of the opportunity to make a move",
		"2. Checkers must move forward to the adjacent field diagonally",
		"3. When a checker is on the same diagonal next to the opponent's checker, behind",
		"which there is a free cell, it must be moved there through this checker",
		"4. Taking the opponent's checkers is mandatory",
		"5. If the checker reaches the last row of the opponent, it",
		"becomes a king and moves diagonally in any direction",
		"6. Kings can only stand on fields not occupied by other checkers",
		"7. The queen cannot jump over her checkers",
		"8. The winner is the player who remains with checkers on the board",
		"after the opponent cannot make a move, either the opponent has no more checkers",
		"left, or the opponent has declared that he is giving up",
		"Press enter for exit"
	};
	int rulesTextCount = sizeof(rulesText) / sizeof(rulesText[0]);

	while (running)
	{
		// Получить размеры окна
		int windowWidth, windowHeight;
		SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = 0; // Возврат в главное меню
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					running = 0; // Возврат в главное меню при клике
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		//RenderBackground(windowWidth, windowHeight);

		// Отрисовка заголовка
		int titleWidth, titleHeight;
		TTF_SizeText(TitleFont, "Game Rules", &titleWidth, &titleHeight);
		int titleX = (windowWidth - titleWidth) / 2;
		int titleY = 60;
		RenderText("Game Rules", titleX, titleY, TitleFont, black);

		// Отрисовка текста правил
		int y = titleY + titleHeight + 20;
		for (int i = 0; i < rulesTextCount; i++)
		{
			int width, height;
			TTF_SizeText(font, rulesText[i], &width, &height);
			int x = (windowWidth - width) / 2;

			if (i == 13)
			{
				RenderText(rulesText[i], x, y, font, brown2);
			}
			else {
				RenderText(rulesText[i], x, y, font, brown);
			}
			y += height + 10;


		}


		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);
	TTF_CloseFont(TitleFont);

	return 0; // Возврат в главное меню
}

int ShowGameModeMenu(int* gameMode)
{
	SDL_Event event;
	int running = 1;

	TTF_Font* Titlefont = TTF_OpenFont("Title2.ttf", 70);
	TTF_Font* font = TTF_OpenFont("Title2.ttf", 24);
	

	SDL_Color brown = { 139, 69, 19, 255 };
	SDL_Color brown2 = { 210, 180, 140, 255 };
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color black = { 0, 0, 0, 255 };

	int currentSelection = 0;
	const char* menuItems[] =
	{
		"Player vs Player",
		"Player vs Computer",
		"Back"
	};
	int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);

	while (running)
	{
		// Получить размеры окна
		int windowWidth, windowHeight;
		SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

		// Определение размеров текста
		int totalHeight = 0;
		int textWidth = 0;
		int maxWidth = 0;
		for (int i = 0; i < menuItemCount; i++)
		{
			int width, height;
			TTF_SizeText(font, menuItems[i], &width, &height);
			totalHeight += height + 10;
			if (width > maxWidth)
			{
				maxWidth = width;
			}
		}
		totalHeight -= 10; // Убираем последний отступ

		// Координаты для центрирования
		int startX = (windowWidth - maxWidth) / 2;
		int startY = (windowHeight - totalHeight) / 2;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
				*gameMode = -1; // Выход из игры
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					currentSelection = (currentSelection - 1 + menuItemCount) % menuItemCount;
					break;
				case SDLK_DOWN:
					currentSelection = (currentSelection + 1) % menuItemCount;
					break;
				case SDLK_RETURN:
					if (currentSelection == 0)
					{
						*gameMode = 1;
						running = 0;
					}
					else if (currentSelection == 1)
					{
						*gameMode = 2;
						running = 0;
					}
					else if (currentSelection == 2)
					{
						*gameMode = 0; // Назад в меню
						running = 0;
					}
					break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				int mouseX = event.motion.x;
				int mouseY = event.motion.y;
				int y = startY;
				for (int i = 0; i < menuItemCount; i++)
				{
					int width, height;
					TTF_SizeText(font, menuItems[i], &width, &height);
					if (mouseX >= startX && mouseX <= startX + width && mouseY >= y && mouseY <= y + height)
					{
						currentSelection = i;
					}
					y += height + 10;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (currentSelection == 0)
					{
						*gameMode = 1; // Классический режим
						running = 0;
					}
					else if (currentSelection == 1)
					{
						*gameMode = 2; // Режим на время
						running = 0;
					}
					else if (currentSelection == 2)
					{
						*gameMode = 0; // Назад в меню
						running = 0;
					}
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);


		// Отрисовка Game Mode
		int titleWidth, titleHeight;
		TTF_SizeText(font, "Game Mode", &titleWidth, &titleHeight);
		int titleX = (windowWidth - titleWidth) / 4;
		int titleY = 110;
		RenderText("Game Mode", titleX, titleY, Titlefont, black);

		// Отображение текущего режима
		const char* currentModeText = (*gameMode == 1) ? "Current Mode: Player vs Player" : "Current Mode: Player vs Computer";
		int modeWidth, modeHeight;
		TTF_SizeText(font, currentModeText, &modeWidth, &modeHeight);
		int modeX = (windowWidth - modeWidth) / 2;
		int modeY = (1.5)*titleY + titleHeight;
		RenderText(currentModeText, modeX, modeY, font, black);

		int y = startY;
		for (int i = 0; i < menuItemCount; i++)
		{
			int width, height;
			TTF_SizeText(font, menuItems[i], &width, &height);
			RenderText(menuItems[i], startX, y, font, (i == currentSelection) ? brown2 : brown);
			y += height + 10;
		}
		RenderButtonFrame(startX - 20, startY - 10, maxWidth + 40, totalHeight + 20, black);


		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);

	return *gameMode;
}


int ShowMainMenu(int* inGame, int* gameMode)
{
    SDL_Event event;
    int running = 1;

	SDL_Texture* BackgroundTexture = LoadBackGroundTexture();

    TTF_Font* font = TTF_OpenFont("Title2.ttf", 24);
	TTF_Font* Titlefont = TTF_OpenFont("Title2.ttf", 90);

	SDL_Color brown = { 139, 69, 19, 255 };
	SDL_Color brown2 = { 210, 180, 140, 255 };
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color black = { 0, 0, 0, 255 };

    int currentSelection = 0;
	bool soundEnabled = true; //звук

    const char* menuItems[] =
    {
        "Rules",
        "Select Game Mode",
        "Start New Game",
        "Saved Games",
        "Sound: ON",
        "Quit"
    };
    int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);

    int menuselection = -1;
	loadmusic();

	while (running)
	{

		int windowWidth, windowHeight;
		SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

		int totalHeight = 0;
		int textWidth = 0;
		int maxWidth = 0;

		for (int i = 0; i < menuItemCount; i++)
		{
			int width, height;
			TTF_SizeText(font, menuItems[i], &width, &height);
			totalHeight += height + 10;
			if (width > maxWidth)
			{
				maxWidth = width;
			}
		}
		totalHeight -= 10;

		int startX = (windowWidth - maxWidth) / 2;
		int startY = (windowHeight - totalHeight) / 2;

		int checkersWidth, checkersHeight;
		TTF_SizeText(font, "Checkers", &checkersWidth, &checkersHeight);
		int checkersdX = (windowWidth - checkersWidth) / 4;
		int checkersdY = 40;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					currentSelection = (currentSelection - 1 + menuItemCount) % menuItemCount;
					break;
				case SDLK_DOWN:
					currentSelection = (currentSelection + 1) % menuItemCount;
					break;
				case SDLK_RETURN:
					menuselection = currentSelection;
					if (currentSelection == 0)
					{
						ShowGameRulesMenu();
						*inGame = 1;
						//running = 0;
					}
					else if (currentSelection == 1)
					{
						*gameMode = ShowGameModeMenu(gameMode);
						if (*gameMode == -1)
						{
							running = 0;
						}
					}
					else if (currentSelection == 2)
					{
						menuselection = currentSelection;
						if (*gameMode == 1) {
							Game(currentSelection);
							//ShowGameMenu(gameMode);
						}
					}
					else if (currentSelection == 3)
					{
						menuselection = currentSelection;
						if (*gameMode == 1) {
							Game(currentSelection);
							//ShowGameMenu(gameMode);
						}
					}
					else if (currentSelection == 4)
					{
						// Переключение звука
						soundEnabled = !soundEnabled;
						menuItems[4] = (menuItems[4] == "Sound: ON") ? "Sound: OFF" : "Sound: ON";
						if (soundEnabled)
						{
							loadmusic();
						}
						else if (!soundEnabled)
						{
							Mix_PauseMusic();
						}
					}
					else if (currentSelection == 5)
					{
						running = 0;
					}
					break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				int mouseX = event.motion.x;
				int mouseY = event.motion.y;
				int y = startY;
				for (int i = 0; i < menuItemCount; i++)
				{
					int width, height;
					TTF_SizeText(font, menuItems[i], &width, &height);
					if (mouseX >= startX && mouseX <= startX + width && mouseY >= y && mouseY <= y + height)
					{
						currentSelection = i;
					}
					y += height + 10;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (currentSelection == 0)
					{
						//*inGame = 1;
						running = 0;
					}
					else if (currentSelection == 1)
					{
						//*gameMode = ShowGameModeMenu(gameMode);
						/*if (*gameMode == -1)
						{
							running = 0;
						}*/
					}
					else if (currentSelection == 2)
					{
						//ShowGameRulesMenu();
					}
					else if (currentSelection == 4)
					{
						// Переключение звука
						soundEnabled = !soundEnabled;
						menuItems[4] = (menuItems[4] == "Sound: ON") ? "Sound: OFF" : "Sound: ON";
						if (soundEnabled)
						{
							loadmusic();
						}
						else if (!soundEnabled)
						{
							Mix_PauseMusic();
						}
					}
					else if (currentSelection == 5)
					{
						running = 0;
					}
				}
			}
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
				}
			}

		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);




		//draw_BackGround(BackgroundTexture, 0, 0, windowWidth, windowHeight);
		RenderText("Checkers", checkersdX, checkersdY, Titlefont, black);

		int y = startY;
		for (int i = 0; i < menuItemCount; i++)
		{
			int width, height;
			TTF_SizeText(font, menuItems[i], &width, &height);
			RenderText(menuItems[i], startX, y, font, (i == currentSelection) ? brown2 : brown);
			y += height + 10;
		}
		RenderButtonFrame(startX - 20, startY - 10, maxWidth + 40, totalHeight + 20, black);

		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);
	return running;
}


void MainLoop()
{
	setlocale(LC_ALL, "Russian");
    SDL_Surface* icon = IMG_Load("2.png");

    if (icon != NULL)
    {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }
    else
    {
        SDL_Log("Unable to load icon: %s", IMG_GetError());
    }

    int running = 1;
    int inGame = 0;
    int gameMode = 1;

    while (running)
    {
        if (!inGame)
        {
            running = ShowMainMenu(&inGame, &gameMode);
        }
        else
        {
            //running = GameLoop();
            inGame = 0;
        }
    }
}