#include "Game_Realization.h"


Cell board[BOARD_SIZE][BOARD_SIZE];
Cell board1[BOARD_SIZE][BOARD_SIZE];

//инициализация игровой доски
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

//инициализация игровой доски сохраненной игры
void InitializeBoard2()
{
	FILE* file1;

	fopen_s(&file1, "OutputFile1.txt", "r");
	if (file1==NULL) {
		perror("Не удалось открыть файл");
		return;
	}

	char str[50];

	//Указатель, в который будет помещен адрес массива, в который считана 
	//строка, или NULL если достигнут конец файла или произошла ошибка
	char* estr;

	//Чтение (построчно) данных из файла в бесконечном цикле
	while (1)
	{
		// Чтение одной строки  из файла
		estr = fgets(str, sizeof(str), file1);

		//Проверка на конец файла или ошибку чтения
		if (estr == NULL)
		{
			// Проверяем, что именно произошло: кончился файл
			// или это ошибка чтения
			if (feof(file1) != 0)
			{
				//Если файл закончился, выводим сообщение о завершении 
				//чтения и выходим из бесконечного цикла
				printf("\nЧтение файла закончено\n");
				break;
			}
			else
			{
				//Если при чтении произошла ошибка, выводим сообщение 
				//об ошибке и выходим из бесконечного цикла
				printf("\nОшибка чтения из файла\n");
				break;
			}
		}
		
		int i, j, k, m;
		char* context = NULL;
		i=atoi(strtok_s(estr, "|", &context));
		j = atoi(strtok_s(NULL, "|", &context));
		k= atoi(strtok_s(NULL, "|", &context));
		m = atoi(strtok_s(NULL, "|", &context));
		if (k == 1)
		{
			board[i][j].type = BLACK;
		}
		else if (k == 2)
		{
			board[i][j].type = WHITE;
		}
		else if(k==0)
		{
			board[i][j].type = EMPTY;
		}
		if (m == 1)
		{
			board[i][j].king=true;
		}
		else if (m == 0)
		{
			board[i][j].king = false;
		}

	}

	fclose(file1);

}

//отрисовка игрового поля
void DrawBoard(SDL_Renderer* renderer, int SCREEN_HEIGHT) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			SDL_Color color;
			if ((i + j) % 2 == 0) {
				color = { 210, 180, 140, 255 };
			}
			else {
				color = { 139, 69, 19, 255 };
			}
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_Rect cell = { j * (SCREEN_HEIGHT/8), i * (SCREEN_HEIGHT/8), SCREEN_HEIGHT / 8, SCREEN_HEIGHT / 8 };
			SDL_RenderFillRect(renderer, &cell);
		}
	}
}

//отрисовка шашек
void drawCheckers(SDL_Renderer* renderer, int SCREEN_HEIGHT, int selectedX, int selectedY,  bool possibleMoves[BOARD_SIZE][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (possibleMoves[i][j]) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // голубой цвет для подсветки возможных ходов
				SDL_Rect cell = { j * (SCREEN_HEIGHT / 8), i * (SCREEN_HEIGHT / 8), (SCREEN_HEIGHT / 8), (SCREEN_HEIGHT / 8) };
				SDL_RenderDrawRect(renderer, &cell);
			}

			if (board[i][j].type != EMPTY) {
				SDL_Color color;
				if (board[i][j].type == BLACK) {
					color = { 0, 0, 0, 255 }; // Черный цвет для шашек
				}
				else if (board[i][j].type == WHITE) {
					color = { 255, 255, 255, 255 }; // Белый цвет для шашек
				}

				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

				int centerX = j * (SCREEN_HEIGHT / 8) + (SCREEN_HEIGHT / 8) / 2;
				int centerY = i * (SCREEN_HEIGHT / 8) + (SCREEN_HEIGHT / 8) / 2;
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
				if (board[i][j].king) {
					SDL_SetRenderDrawColor(renderer, 255, 195, 0, 255); // желтый цвет для дамок
					SDL_Rect kingRect = { centerX - 15, centerY - 15, 30, 30 };
					SDL_RenderDrawRect(renderer, &kingRect);
				}

				// подсветка текущей шашки
				if (i == selectedY && j == selectedX) {
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // зеленый цвет для подсветки текущей шашки
					SDL_Rect highlightRect = { j * (SCREEN_HEIGHT / 8), i* (SCREEN_HEIGHT / 8), (SCREEN_HEIGHT / 8), (SCREEN_HEIGHT / 8) };
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

//перемещение шашек
void MovePiece(int startX, int startY, int endX, int endY, bool* mustContinue, CheckerType* currentPlayer, CheckerType* currentPlayer1, bool checker[BOARD_SIZE][BOARD_SIZE]) {
	
	int dx = endX - startX;
	int dy = endY - startY;
	*mustContinue = false;//продолжение хода
	CopyMas(currentPlayer1, currentPlayer);

	// Проверка, было ли это взятие
	if (abs(dx) == 2 && abs(dy) == 2) {
		int middleX = startX + dx / 2;
		int middleY = startY + dy / 2;
		board[middleY][middleX].type = EMPTY;
		board[middleY][middleX].king = false;
		*mustContinue = true;
		checker[endX][endY] = true;
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
				checker[endX][endY] = true;
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

//все возможные ходы для шашки
void GetPossibleMoves(int startX, int startY, bool possibleMoves[BOARD_SIZE][BOARD_SIZE], CheckerType currentPlayer) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			possibleMoves[i][j] = IsValidMove(startX, startY, j, i, currentPlayer);
		}
	}
}

//проверка возможности взятия шашки противника
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

//фоновая музыка
void loadmusic()
{
	fon = Mix_LoadMUS("Lobby-Time.mp3");
	Mix_PlayMusic(fon, -1);
}

//запись в файл данных сохраненной игры
void OutFail() {
	FILE* file;
	fopen_s(&file, "OutputFile1.txt", "w");
	if (!file) {
		perror("Не удалось открыть файл");
		return;
	}
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j].king) {
				fprintf(file, "%d|%d|%d|%d|\n", i, j, board[i][j].type, 1);
			}
			else {
				fprintf(file, "%d|%d|%d|%d|\n", i, j, board[i][j].type, 0);
			}
		}
			
			
	}
	fclose(file);
}

//перемещение по дополнительному меню
void GameMenu(int xMenu, int yMenu, int title1X, int title1Y, int title2Y, int title3Y, int title3Width, int title3Height, int *running, int *d)
{
	if (xMenu>=(title1X-10) && xMenu<=(title1X+title3Width+20) && yMenu>=(title1Y-10) && yMenu<=(title1Y+ title3Height+20))
	{
		Game(1);
	}
	if (xMenu >= (title1X - 10) && xMenu <= (title1X + title3Width + 20) && yMenu >= (title2Y - 10) && yMenu <= (title2Y + title3Height + 20))
	{
		OutFail();
		*d = 2;
	}
	if (xMenu >= (title1X - 10) && xMenu <= (title1X + title3Width + 20) && yMenu >= (title3Y - 10) && yMenu <= (title3Y + title3Height + 20))
	{
		*running = 0;
	}
}

//нажатие кнопок бонусов
void Bonus(int xMenu, int yMenu, int bonusX, int bonusY, int bonus2X, int bonus2Y, int bonusWidth, int bonusHeight, int bonus2Width, int bonus2Height, CheckerType* currentPlayer, CheckerType* currentPlayer1)
{
	if (xMenu >= bonusX && xMenu <= (bonusX + bonusWidth) && yMenu >= bonusY && yMenu <= (bonusY + bonusHeight))
	{
		CopyMasBack(currentPlayer, currentPlayer1);
	}
	if (xMenu >= bonus2X && xMenu <= (bonus2X + bonus2Width) && yMenu >= bonus2Y && yMenu <= (bonus2Y + bonus2Height))
	{
		if (*currentPlayer == WHITE)
		{
			*currentPlayer =BLACK;
		}
		else {
			*currentPlayer = WHITE;
		}
	}
}

//копирование данных последнего хода в новый массив
void CopyMas(CheckerType *currentPlayer1, CheckerType* currentPlayer)
{

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board1[i][j].type = board[i][j].type;
			board1[i][j].king = board[i][j].king;
		}
	}
	*currentPlayer1 = *currentPlayer;
}

//возврат последнего хода
void CopyMasBack(CheckerType* currentPlayer, CheckerType* currentPlayer1)
{

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j].type = board1[i][j].type;
			board[i][j].king = board1[i][j].king;
		}
	}
	*currentPlayer = *currentPlayer1;
}

//обнуление массива дополнительного хода шашки
void CheckerMasNull(bool checker[BOARD_SIZE][BOARD_SIZE])
{
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			checker[i][j] = false;
		}
	}
}

//проверка выбранной шашки
bool isChecker(int x, int y, bool checker[BOARD_SIZE][BOARD_SIZE])
{    
	bool z = true;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (checker[i][j] == true)
				if (i==x && j==y) { z = true; }
				else { z = false; }
			return z;
		}
	}
	return z;
}


int Game(int currentSelection)
{
	SDL_Event event;
	int running = 1;
	int piece_selected = 0;
	int startX, startY;

	//выбор игрового поля(новая игра/сохраненная игра)
	if (currentSelection == 2) {
		InitializeBoard2();
	}
	else {
		InitializeBoard();
	}

	bool possibleMoves[BOARD_SIZE][BOARD_SIZE] = { false };//возможные ходы

	CheckerType currentPlayer = WHITE;
	CheckerType currentPlayer1 = WHITE;

	bool checker[BOARD_SIZE][BOARD_SIZE];
	CheckerMasNull(checker);

	SDL_Color black = { 0, 0, 0, 255 };
	SDL_Color brown = { 139, 69, 19, 255 };
	SDL_Color brown2 = { 210, 180, 140, 255 };
	SDL_Color white = { 255, 255, 255, 255 };
	TTF_Font* font = TTF_OpenFont("Title2.ttf", 16);
	TTF_Font* font1 = TTF_OpenFont("Title2.ttf", 23);

	int d=0;
	int white_count=-1, black_count=-1;

	while (running)
	{
		// Получить размеры окна
		int windowWidth, windowHeight;
		SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

		//размеры кнопок дополнительного меню
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

		//размеры кнопок бонусов
		int bonusWidth, bonusHeight;
		TTF_SizeText(font, "Return", &bonusWidth, &bonusHeight);
		int bonusX = windowHeight + 30;
		int bonusY = windowHeight / 2-60;

		int bonus2Width, bonus2Height;
		TTF_SizeText(font, "+1 move", &bonus2Width, &bonus2Height);
		int bonus2X = windowHeight + 30;
		int bonus2Y = bonusY+ bonus2Height+30;

		// отрисовка завершения игры
		int endWidth, endHeight;
		int endX = windowHeight+30;
		int endY = windowHeight / 4;

		while (SDL_PollEvent(&event))
		{

			if (event.type == SDL_QUIT)
			{
				running = 0;
				for (int i = 0; i < BOARD_SIZE; i++)
					for (int j = 0; j < BOARD_SIZE; j++)
						board[i][j].king = false;
			}

			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{

					int xMenu = event.button.x;
					int yMenu = event.button.y;


					GameMenu(xMenu, yMenu, title1X, title1Y, title2Y, title3Y, title3Width, title3Height, &running, &d);
					Bonus(xMenu, yMenu, bonusX, bonusY, bonus2X, bonus2Y, bonusWidth, bonusHeight, bonus2Width, bonus2Height, &currentPlayer, &currentPlayer1);

					int x = event.button.x / (windowHeight / BOARD_SIZE);
					int y = event.button.y / (windowHeight / BOARD_SIZE);

					if (!piece_selected && board[y][x].type != EMPTY) {
						d = 0;

						startX = x;
						startY = y;
						piece_selected = true;

						GetPossibleMoves(startX, startY, possibleMoves, currentPlayer);

					}
					else if (piece_selected) {
						d = 0;
						bool mustContinue = false;
						//CheckerMasNull(checker);
						if (IsValidMove(startX, startY, x, y, currentPlayer)) {
							MovePiece(startX, startY, x, y, &mustContinue, &currentPlayer, &currentPlayer1, checker);

							if (mustContinue) //&& isChecker(x, y, checker)) {
							{

								if (!isChecker(x, y, checker)) { break; }
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
									CheckerMasNull(checker);

								}
							}
							else {
								piece_selected = false;
								currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
								CheckerMasNull(checker);

							}
						}
						else {
							piece_selected = false;
						}

						for (int i = 0; i < BOARD_SIZE; ++i) {
							for (int j = 0; j < BOARD_SIZE; ++j) {
								possibleMoves[i][j] = false;


							}
						}
					}
				}

				white_count = 0;
				black_count = 0;
				for (int i = 0; i < BOARD_SIZE; ++i) {
					for (int j = 0; j < BOARD_SIZE; ++j) {
						//possibleMoves[i][j] = false;
						if (board[i][j].type == WHITE)
						{
							white_count = 1;
						}
						if (board[i][j].type == BLACK)
						{
							black_count = 1;
						}

					}
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);


		//отрисовка текущего хода
		int titleWidth, titleHeight;
		if (currentPlayer == WHITE) {
			TTF_SizeText(font, "Move: white", &titleWidth, &titleHeight);
		}
		if (currentPlayer == BLACK) {
			TTF_SizeText(font, "Move: black", &titleWidth, &titleHeight);
		}
		int titleX = windowHeight +30;
		int titleY = 30;
		if (currentPlayer == WHITE) {
			RenderText("Move: white", titleX, titleY, font, black);
		}
		if (currentPlayer == BLACK) {
			RenderText("Move: black", titleX, titleY, font, black);
		}
		RenderButtonFrame(titleX - 10, titleY - 10, titleWidth + 20, titleHeight + 20, brown);

		//отрисовка бонусов
		RenderText("Return", bonusX, bonusY, font, black);
		RenderButtonFrame(bonusX - 10, bonusY - 10, bonus2Width + 20, bonusHeight + 20, brown);

		RenderText("+1 move", bonus2X, bonus2Y, font, black);
		RenderButtonFrame(bonus2X - 10, bonus2Y - 10, bonus2Width + 20, bonus2Height + 20, brown);

		//отрисовка дополнительного меню
		RenderText("Finish the game", title3X, title3Y, font, brown);
		RenderText("New game", title1X, title1Y, font, brown);
		if (d == 2)
		{
			RenderText("Save the game", title2X, title2Y, font, brown2);
			
		}
		else {
			RenderText("Save the game", title2X, title2Y, font, brown);
		}
		RenderButtonFrame(title1X - 10, title1Y - 10, title3Width + 20, title3Height + 20, black);
		RenderButtonFrame(title2X - 10, title2Y - 10, title3Width + 20, title3Height + 20, black);
		RenderButtonFrame(title3X - 10, title3Y - 10, title3Width + 20, title3Height + 20, black);

		//вывод на экран победившего игрока
		if (white_count == 0) {
			TTF_SizeText(font, "black wins!", &endWidth, &endHeight);
			RenderText("black wins!", endX, endY, font1, black);
		}
		if (black_count == 0) {
			TTF_SizeText(font, "white wins!", &endWidth, &endHeight);
			RenderText("white wins!", endX, endY, font1, black);
		}

		//отрисовка доски и шашек
		DrawBoard(renderer, windowHeight);
		drawCheckers(renderer, windowHeight, piece_selected ? startX : -1, piece_selected ? startY : -1, possibleMoves);


		SDL_RenderPresent(renderer);
	}



	return 0; // Возврат в главное меню
}