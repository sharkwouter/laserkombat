#include "Draw.h"

#include "utils.h"
#include "constants.h"

Draw::Draw(SDL_Renderer * renderer) : renderer(renderer) {
    animation = 0;
    renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_TARGET, IMAGE_WIDTH*COLUMNS, IMAGE_WIDTH*ROWS);
    font = TTF_OpenFont(getAssetPath("fonts", "FreeSans.ttf").c_str(), 14);
}

Draw::~Draw() {
    SDL_DestroyTexture(renderTarget);
    TTF_CloseFont(font);
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

void Draw::BlitText(char * text, int x, int y) {
    SDL_Surface * surface = TTF_RenderText_Solid(font, text, {0, 255, 0, 255});
    if (surface == NULL) {
        SDL_Log("Couldn't create surface for text %s: %s", text, TTF_GetError());
        return;
    }

    SDL_Texture * texture = NULL;
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    if (texture) {
        SDL_Rect rect = {x, y, 0, 0};
        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
        rect.x -= rect.w/2;
        rect.y -= rect.h/2;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    }
}

void Draw::Flip() {
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, renderTarget, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, renderTarget);
}