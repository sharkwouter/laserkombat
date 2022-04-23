#include "Draw.h"

#include "constants.h"

Draw::Draw(SDL_Renderer * renderer) : renderer(renderer) {
    animation = 0;
    renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Draw::BlitSquare(SDL_Texture * texture, int x, int y, int dx, int dy) {
    x *= IMAGE_WIDTH;
    y *= IMAGE_WIDTH;
    dx *= IMAGE_WIDTH;
    dy *= IMAGE_WIDTH;
    SDL_Rect sr={x,y,IMAGE_WIDTH, IMAGE_WIDTH};
    SDL_Rect dr={dx,dy,IMAGE_WIDTH, IMAGE_WIDTH};
    SDL_RenderCopy(renderer, texture, &sr, &dr);
}

void Draw::BlitOther(SDL_Texture * texture, int x, int y, int dx, int dy, int w, int h) {
    SDL_Rect sr={x,y,w,h};
    SDL_Rect dr={dx,dy,w, h};
    SDL_RenderCopy(renderer, texture, &sr, &dr);
}

void Draw::BlackSquare(int x, int y) {
    x *= IMAGE_WIDTH;
    y *= IMAGE_WIDTH;
    SDL_Rect r={x,y,IMAGE_WIDTH,IMAGE_WIDTH};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
}

void Draw::Flip() {
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, renderTarget, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, renderTarget);
}