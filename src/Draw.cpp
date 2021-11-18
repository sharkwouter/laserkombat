#include "Draw.h"

#include "constants.h"

int Draw::animation = 0;

void Draw::BlitSquare(SDL_Renderer * renderer, SDL_Texture * texture, int x, int y, int dx, int dy) {
    x *= IMAGE_WIDTH;
    y *= IMAGE_WIDTH;
    dx *= IMAGE_WIDTH;
    dy *= IMAGE_WIDTH;
    SDL_Rect sr={x,y,IMAGE_WIDTH, IMAGE_WIDTH};
    SDL_Rect dr={dx,dy,IMAGE_WIDTH, IMAGE_WIDTH};
    SDL_RenderCopy(renderer, texture, &sr, &dr);
}

void Draw::Blit(SDL_Renderer * renderer, SDL_Texture * texture, int x, int y, int dx, int dy) {

}

void Draw::BlitOther(SDL_Renderer * renderer, SDL_Texture * texture, int x, int y, int dx, int dy, int w, int h) {
    SDL_Rect sr={x,y,w,h};
    SDL_Rect dr={dx,dy,w, h};
    SDL_RenderCopy(renderer, texture, &sr, &dr);
}

void Draw::BlackSquare(SDL_Renderer * renderer, int x, int y) {
    x *= IMAGE_WIDTH;
    y *= IMAGE_WIDTH;
    SDL_Rect r={x,y,IMAGE_WIDTH,IMAGE_WIDTH};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
}

void Draw::Flip(SDL_Renderer * renderer) {
    SDL_RenderPresent(renderer);
}