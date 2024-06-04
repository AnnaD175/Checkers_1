#include"Render.h"


void RenderButtonFrame(int x, int y, int w, int h, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderDrawRect(renderer, &rect);
}

void draw_Board(SDL_Texture* texture, int SCREEN_HEIGHT)
{
	SDL_Rect rect = { 0, 0, SCREEN_HEIGHT, SCREEN_HEIGHT };
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void RenderText(const char* text, int x, int y, TTF_Font* font, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect dstrect = { x, y, surface->w, surface->h };

	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
}