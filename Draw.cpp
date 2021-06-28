// Draw.cpp

#include <SDL2/SDL.h>

#include "constants.h"
#include "Exception.h"
#include "Draw.h"

void Draw::SetWindow()
{
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WOLD_MAX_X, WOLD_MAX_Y, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("Couldn't create window: %s", SDL_GetError());
        exit(2);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        printf("Couldn't create renderer: %s", SDL_GetError());
        exit(3);
    }
}

void Draw::SetDDraw()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) < 0){
        printf("Couldn't init SDL: %s", SDL_GetError());
        exit(1);
    }
}

void Draw::CleanUp()
{

}

SDL_Texture * Draw::GetSurface(const char* name)
{
	SDL_Surface * surface = SDL_LoadBMP(name);
	if( surface == nullptr )
		throw Exception("Error loading bitmap.");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to load texture for image %s! SDL Error: %s\n", name, SDL_GetError());
        exit(4);
    }
    SDL_FreeSurface(surface);
	return texture;
}
void Draw::BlitSquare(SDL_Texture * surface, int x, int y, int dx, int dy)
{
	x*=IMAGE_WIDTH;
	y*=IMAGE_WIDTH;
	dx*=IMAGE_WIDTH;
	dy*=IMAGE_WIDTH;
	SDL_Rect sr={x,y,x+IMAGE_WIDTH, y+IMAGE_WIDTH};
	SDL_Rect r={dx,dy,dx+IMAGE_WIDTH, dy+IMAGE_WIDTH};
    SDL_RenderCopy(renderer, surface, &sr, &r);
}

void Draw::Blit(SDL_Texture * surface, int x, int y, int dx, int dy)
{
	SDL_Rect r={x,y,x+IMAGE_WIDTH, y+IMAGE_WIDTH};
    SDL_RenderCopy(renderer, surface, NULL, &r);
}

void Draw::BlitOther(SDL_Texture * surface, int x, int y, int dx, int dy, int w, int h)
{
	SDL_Rect r={x,y,x+w, y+h};
    SDL_RenderCopy(renderer, surface, NULL, &r);
}

void Draw::BlackSquare(int dx, int dy)
{
	dx*=IMAGE_WIDTH;
	dy*=IMAGE_WIDTH;

	SDL_Rect rc = {dx, dy, dx+IMAGE_WIDTH, dy+IMAGE_WIDTH};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderFillRect(renderer, &rc);
}


void Draw::Flip()
{
	SDL_RenderClear(renderer);
}
