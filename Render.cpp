#include"Render.h"


void RenderButtonFrame(int x, int y, int w, int h, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderDrawRect(renderer, &rect);
}

SDL_Texture* LoadBoardTexture()
{
	SDL_Surface* BoardSurface = IMG_Load("22.jpeg");
	SDL_Texture* BoardTexture = SDL_CreateTextureFromSurface(renderer, BoardSurface);

	SDL_FreeSurface(BoardSurface);

	return BoardTexture;
}
void draw_Board(SDL_Texture* texture, int SCREEN_HEIGHT)
{
	SDL_Rect rect = { 0, 0, SCREEN_HEIGHT, SCREEN_HEIGHT };
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void draw_BackGround(SDL_Texture* texture, int x, int y, int w, int h)
{
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

SDL_Texture* LoadBackGroundTexture()
{
	SDL_Surface* BackgroundSurface = IMG_Load("Background5.jpg");
	SDL_Texture* BackgroundTexture = SDL_CreateTextureFromSurface(renderer, BackgroundSurface);
	SDL_FreeSurface(BackgroundSurface);
	return BackgroundTexture;
}