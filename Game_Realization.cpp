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
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // blue for possible moves
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


void GameMenu(int xMenu, int yMenu, int title1X, int title1Y, int title2Y, int title3Y, int title3Width, int title3Height, int *running)
{
	if (xMenu>=(title1X-10) && xMenu<=(title1X+title3Width+20) && yMenu>=(title1Y-10) && yMenu<=(title1Y+ title3Height+20))
	{
		Game();
	}
	if (xMenu >= (title1X - 10) && xMenu <= (title1X + title3Width + 20) && yMenu >= (title2Y - 10) && yMenu <= (title2Y + title3Height + 20))
	{
		
	}
	if (xMenu >= (title1X - 10) && xMenu <= (title1X + title3Width + 20) && yMenu >= (title3Y - 10) && yMenu <= (title3Y + title3Height + 20))
	{
		*running = 0;
	}
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

	SDL_Color black = { 0, 0, 0, 255 };
	SDL_Color brown = { 139, 69, 19, 255 };
	SDL_Color brown2 = { 210, 180, 140, 255 };
	TTF_Font* font = TTF_OpenFont("Title2.ttf", 16);

	while (running)
	{
		// Получить размеры окна
		int windowWidth, windowHeight;
		SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

		int title3Width, title3Height;
		TTF_SizeText(font, "Finish the game", &title3Width, &title3Height);
		int title3X = windowHeight + 30;
		int title3Y = windowHeight - title3Height - 30;


		int title2Width, title2Height;
		TTF_SizeText(font, "Save the game", &title2Width, &title2Height);
		int title2X = windowHeight + 30;
		int title2Y = windowHeight - title3Height - title2Height - 60;

		int title1Width, title1Height;
		TTF_SizeText(font, "New game", &title1Width, &title1Height);
		int title1X = windowHeight + 30;
		int title1Y = windowHeight - title3Height - title2Height - title1Height - 90;



		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
				for (int row = 0; row < BOARD_SIZE; row++)
					for (int col = 0; col < BOARD_SIZE; col++)
						board[row][col].king = false;
			}

			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					//running = 0; // Возврат в главное меню при клике
					int xMenu = event.button.x;
					int yMenu = event.button.y;

					GameMenu(xMenu, yMenu, title1X, title1Y, title2Y, title3Y, title3Width, title3Height, &running);

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

								if (board[startY][startX].king && mustContinue) {
									int directions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
									for (int d = 0; d < 4; ++d) {
										int stepX = directions[d][0];
										int stepY = directions[d][1];
										int x = startX + stepX;
										int y = startY + stepY;
										bool opponentPieceFound = false;

										while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
											if (board[y][x].type != EMPTY) {
												if (board[y][x].type == board[startY][startX].type || opponentPieceFound) {
													break;
												}
												opponentPieceFound = true;
											}
											else if (opponentPieceFound && board[y][x].type == EMPTY) {
												hasFurtherCapture = true;
												break;
											}
											x += stepX;
											y += stepY;
										}

										if (hasFurtherCapture) {
											break;
										}
									}
								}
								else if (!board[startY][startX].king){
									for (int i = 0; i < BOARD_SIZE; ++i) {
										for (int j = 0; j < BOARD_SIZE; ++j) {
											if (possibleMoves[i][j] && abs(i - startY) == 2 && abs(j - startX) == 2) {
												hasFurtherCapture = true;
												break;
											}
										}
										if (hasFurtherCapture) break;
									}
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


		// Отрисовка Game Mode
		int titleWidth, titleHeight;
		if (currentPlayer == WHITE) {
			TTF_SizeText(font, "Move: white", &titleWidth, &titleHeight);
		}
		if (currentPlayer == BLACK) {
			TTF_SizeText(font, "Move: black", &titleWidth, &titleHeight);
		}
		int titleX = windowHeight + 30;
		int titleY = 30;
		if (currentPlayer == WHITE) {
			RenderText("Move: white", titleX, titleY, font, brown);
		}
		if (currentPlayer == BLACK) {
			RenderText("Move: black", titleX, titleY, font, brown);
		}
		RenderButtonFrame(titleX - 10, titleY - 10, titleWidth + 20, titleHeight + 20, brown2);



		RenderText("Finish the game", title3X, title3Y, font, black);

		RenderText("Save the game", title2X, title2Y, font, black);


		RenderText("New game", title1X, title1Y, font, black);

		RenderButtonFrame(title1X - 10, title1Y - 10, title3Width + 20, title3Height + 20, brown);
		RenderButtonFrame(title2X - 10, title2Y - 10, title3Width + 20, title3Height + 20, brown);
		RenderButtonFrame(title3X - 10, title3Y - 10, title3Width + 20, title3Height + 20, brown);

		DrawBoard(renderer, windowHeight);
		//draw_Board(BoardTexture, windowHeight);
		drawCheckers(renderer, windowHeight, piece_selected ? startX : -1, piece_selected ? startY : -1, possibleMoves);


		SDL_RenderPresent(renderer);
	}



	return 0; // Возврат в главное меню
}