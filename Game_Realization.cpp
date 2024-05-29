#include "Game_Realization.h"

Cell board[BOARD_SIZE][BOARD_SIZE];

void InitializeBoard()
{
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i + j) % 2 == 1 && i < 3) {
				board[i][j].type = BLACK;
			}
			else if ((i + j) % 2 == 1 && i > 4) {
				board[i][j].type = WHITE;
			}
			else {
				board[i][j].type = EMPTY;
			}
		}
	}
}

void drawCheckers(SDL_Renderer* renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			if (board[row][col].type != EMPTY) {
				SDL_Color color;
				if (board[row][col].type == BLACK) {
					color = { 0, 0, 0, 255 }; // Черный цвет для шашек
				}
				else if (board[row][col].type == WHITE) {
					color = { 255, 255, 255, 255 }; // Белый цвет для шашек
				}

				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

				int centerX = (SCREEN_HEIGHT / 10) + col * (SCREEN_HEIGHT / 10) + (SCREEN_HEIGHT / 10) / 2;
				int centerY = (SCREEN_HEIGHT / 10) + row * (SCREEN_HEIGHT / 10) + (SCREEN_HEIGHT / 10) / 2;
				int radius = (SCREEN_HEIGHT / 10) / 2 - 5;

				for (int w = 0; w < radius * 2; w++) {
					for (int h = 0; h < radius * 2; h++) {
						int dx = radius - w; // Горизонтальное смещение
						int dy = radius - h; // Вертикальное смещение
						if ((dx * dx + dy * dy) <= (radius * radius)) {
							SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
						}
					}
				}
			}
		}
	}
}

void loadmusic()
{
	fon = Mix_LoadMUS("Lobby-Time.mp3");
	Mix_PlayMusic(fon, -1);
}

int Game()
{
	SDL_Event event;
	int running = 1;

	InitializeBoard();
	SDL_Texture* BoardTexture = LoadBoardTexture();

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


		draw_Board(BoardTexture, windowHeight);
		drawCheckers(renderer, windowHeight, windowHeight);


		SDL_RenderPresent(renderer);
	}



	return 0; // Возврат в главное меню
}