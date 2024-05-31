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

void DrawBoard(SDL_Renderer* renderer, int SCREEN_HEIGHT) {
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			SDL_Color color;
			if ((row + col) % 2 == 0)
			{
				color = { 210, 180, 140, 255 };
			}
			else {
				color = { 139, 69, 19, 255 };
			}
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			//SDL_SetRenderDrawColor(renderer, (row + col) % 2 ? 139 : 210, 69, 19, 255); // Dark and light brown
			SDL_Rect cell = { col * (SCREEN_HEIGHT/8), row * (SCREEN_HEIGHT/8), SCREEN_HEIGHT / 8, SCREEN_HEIGHT / 8 };
			SDL_RenderFillRect(renderer, &cell);
		}
	}
}

void drawCheckers(SDL_Renderer* renderer, int SCREEN_HEIGHT, int selectedX, int selectedY,  bool possibleMoves[BOARD_SIZE][BOARD_SIZE]) {
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			if (possibleMoves[row][col]) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Red for possible moves
				SDL_Rect cell = { col * (SCREEN_HEIGHT / 8), row * (SCREEN_HEIGHT / 8), (SCREEN_HEIGHT / 8), (SCREEN_HEIGHT / 8) };
				SDL_RenderDrawRect(renderer, &cell);
			}

			if (board[row][col].type != EMPTY) {
				SDL_Color color;
				if (board[row][col].type == BLACK) {
					color = { 0, 0, 0, 255 }; // Черный цвет для шашек
				}
				else if (board[row][col].type == WHITE) {
					color = { 255, 255, 255, 255 }; // Белый цвет для шашек
				}

				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

				int centerX = col * (SCREEN_HEIGHT / 8) + (SCREEN_HEIGHT / 8) / 2;
				int centerY = row * (SCREEN_HEIGHT / 8) + (SCREEN_HEIGHT / 8) / 2;
				int radius = (SCREEN_HEIGHT / 8) / 2 - 5;

				for (int w = 0; w < radius * 2; w++) {
					for (int h = 0; h < radius * 2; h++) {
						int dx = radius - w; // Горизонтальное смещение
						int dy = radius - h; // Вертикальное смещение
						if ((dx * dx + dy * dy) <= (radius * radius)) {
							SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
						}
					}
				}
				if (board[row][col].king) {
					SDL_SetRenderDrawColor(renderer, 255, 195, 0, 255); // Gold color for kings
					SDL_Rect kingRect = { centerX - 15, centerY - 15, 30, 30 };
					SDL_RenderDrawRect(renderer, &kingRect);
				}

				// Highlight the selected piece
				if (row == selectedY && col == selectedX) {
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for highlight
					SDL_Rect highlightRect = { col * (SCREEN_HEIGHT / 8), row* (SCREEN_HEIGHT / 8), (SCREEN_HEIGHT / 8), (SCREEN_HEIGHT / 8) };
					SDL_RenderDrawRect(renderer, &highlightRect);
				}
			}
		}
	}
}

//проверка ходов шашек
bool IsValidMove(int startX, int startY, int endX, int endY, CheckerType currentPlayer) {
	if (endX < 0 || endX >= BOARD_SIZE || endY < 0 || endY >= BOARD_SIZE) return false;
	if (board[endY][endX].type != EMPTY) return false;
	int dx = endX - startX;
	int dy = endY - startY;

	if (board[startY][startX].type != currentPlayer) return false;


	// Проверка обычного хода для обычных шашек
	if (!board[startY][startX].king) {
	
		if (board[startY][startX].type == BLACK && dy == 1 && abs(dx) == 1) return true;
		if (board[startY][startX].type == WHITE && dy == -1 && abs(dx) == 1) return true;

		// Проверка взятия для обычных шашек
		if (abs(dx) == 2 && abs(dy) == 2) {
			int middleX = startX + dx / 2;
			int middleY = startY + dy / 2;
			if (board[middleY][middleX].type != EMPTY && board[middleY][middleX].type != board[startY][startX].type) {
				return true;
			}
		}
	
	}
	else {
		// Проверка хода для дамок
	
		if (abs(dx) == abs(dy)) {
			int stepX = (dx > 0) ? 1 : -1;
			int stepY = (dy > 0) ? 1 : -1;
			int x = startX + stepX;
			int y = startY + stepY;
			bool opponentPieceFound = false;

			while (x != endX && y != endY) {
				if (board[y][x].type != EMPTY) {
					if (board[y][x].type == board[startY][startX].type || opponentPieceFound) {
						return false;
					}
					opponentPieceFound = true;
				}
				x += stepX;
				y += stepY;
			}

			return true;
			
		}
	}

	return false;
}

void MovePiece(int startX, int startY, int endX, int endY, bool* mustContinue) {
	
	int dx = endX - startX;
	int dy = endY - startY;
	*mustContinue = false;

	// Проверка, было ли это взятие
	if (abs(dx) == 2 && abs(dy) == 2) {
		int middleX = startX + dx / 2;
		int middleY = startY + dy / 2;
		board[middleY][middleX].type = EMPTY;
		board[middleY][middleX].king = false;
		*mustContinue = true;
	}
	else if (abs(dx) > 1 && abs(dy) > 1 && board[startY][startX].king) {
		// Взятие для дамок
		int stepX = (dx > 0) ? 1 : -1;
		int stepY = (dy > 0) ? 1 : -1;
		int x = startX + stepX;
		int y = startY + stepY;

		while (x != endX && y != endY) {
			if (board[y][x].type != EMPTY) {
				board[y][x].type = EMPTY;
				board[y][x].king = false;
				*mustContinue = true;
				break;
			}
			x += stepX;
			y += stepY;
		}
	}
	
	board[endY][endX] = board[startY][startX];
	board[startY][startX].type = EMPTY;
	board[startY][startX].king = false;

	if (endY == 0 && board[endY][endX].type == WHITE) {
		board[endY][endX].king = true;
	}
	if (endY == BOARD_SIZE - 1 && board[endY][endX].type == BLACK) {
		board[endY][endX].king = true;
	}
}

void GetPossibleMoves(int startX, int startY, bool possibleMoves[BOARD_SIZE][BOARD_SIZE], CheckerType currentPlayer) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			possibleMoves[i][j] = IsValidMove(startX, startY, j, i, currentPlayer);
		}
	}
}

bool HasCaptureMoves(CheckerType currentPlayer) {
	for (int y = 0; y < BOARD_SIZE; ++y) {
		for (int x = 0; x < BOARD_SIZE; ++x) {
			if (board[y][x].type == currentPlayer) {
				bool possibleMoves[BOARD_SIZE][BOARD_SIZE];
				GetPossibleMoves(x, y, possibleMoves, currentPlayer);
				for (int i = 0; i < BOARD_SIZE; ++i) {
					for (int j = 0; j < BOARD_SIZE; ++j) {
						if (possibleMoves[i][j] && abs(i - y) == 2 && abs(j - x) == 2) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
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
	int piece_selected = 0;
	int startX, startY;

	InitializeBoard();
	SDL_Texture* BoardTexture = LoadBoardTexture();

	bool possibleMoves[BOARD_SIZE][BOARD_SIZE] = { false };
	CheckerType currentPlayer = WHITE;

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
				for (int row = 0; row < BOARD_SIZE; row++)
					for (int col = 0; col < BOARD_SIZE; col++)
						board[row][col].king = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = 0; // Возврат в главное меню
					for (int row = 0; row < BOARD_SIZE; row++)
						for (int col = 0; col < BOARD_SIZE; col++)
							board[row][col].king = false;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					//running = 0; // Возврат в главное меню при клике
					//int x = event.button.x;
					//int y = event.button.y;

					int x = event.button.x / (windowHeight / BOARD_SIZE);
					int y = event.button.y / (windowHeight / BOARD_SIZE);

					if (!piece_selected && board[y][x].type != EMPTY) {
						startX = x;
						startY = y;
						piece_selected = true;
						GetPossibleMoves(startX, startY, possibleMoves, currentPlayer);
					}
					else if (piece_selected) {
						bool mustContinue = false;
						if (IsValidMove(startX, startY, x, y, currentPlayer)) {
							MovePiece(startX, startY, x, y, &mustContinue);

							if (mustContinue) {
								startX = x;
								startY = y;
								GetPossibleMoves(startX, startY, possibleMoves, currentPlayer);

								// Проверяем, есть ли еще возможные взятия
								bool hasFurtherCapture = false;
								for (int i = 0; i < BOARD_SIZE; ++i) {
									for (int j = 0; j < BOARD_SIZE; ++j) {
										if (possibleMoves[i][j] && abs(i - startY) == 2 && abs(j - startX) == 2) {
											hasFurtherCapture = true;
											break;
										}
									}
									if (hasFurtherCapture) break;
								}

								if (!hasFurtherCapture) {
									piece_selected = false;
									currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
								}
							}
							else {
								piece_selected = false;
								currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
							}
						}
						else {
							piece_selected = false;
						}

							//currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE; // Переключение хода игрока
						//}
						//piece_selected = false;
						for (int i = 0; i < BOARD_SIZE; ++i) {
							for (int j = 0; j < BOARD_SIZE; ++j) {
								possibleMoves[i][j] = false;
							}
						}
					}
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		DrawBoard(renderer, windowHeight);
		//draw_Board(BoardTexture, windowHeight);
		drawCheckers(renderer, windowHeight, piece_selected ? startX : -1, piece_selected ? startY : -1, possibleMoves);


		SDL_RenderPresent(renderer);
	}



	return 0; // Возврат в главное меню
}