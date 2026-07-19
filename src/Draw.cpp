#include "Draw.h"

#include "utils.h"
#include "constants.h"

Draw::Draw(SDL_Renderer * renderer) : renderer(renderer) {
    animation = 0;
    font = TTF_OpenFont(getAssetPath("fonts", "FreeSans.ttf").c_str(), 14);
}

Draw::~Draw() {
    TTF_CloseFont(font);
}

void Draw::BlitSquare(SDL_Texture * texture, int x, int y, int dx, int dy) {
    x *= BLOCK_SIZE;
    y *= BLOCK_SIZE;
    dx *= BLOCK_SIZE;
    dy *= BLOCK_SIZE;
    SDL_Rect sr={x,y,BLOCK_SIZE, BLOCK_SIZE};
    SDL_Rect dr={dx,dy,BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderCopy(renderer, texture, &sr, &dr);
}

void Draw::BlitWater(SDL_Texture * texture, int rotation, int dx, int dy) {
    // Slow down the water by half for small block sizes
    if (BLOCK_SIZE < 20) {
        rotation /= 2;
    }
    rotation = rotation % BLOCK_SIZE;
    dx *= BLOCK_SIZE;
    dy *= BLOCK_SIZE;
    SDL_Rect dr={dx + rotation, dy, BLOCK_SIZE - rotation, BLOCK_SIZE};
    SDL_Rect sr={0, 0, BLOCK_SIZE - rotation, BLOCK_SIZE};
    SDL_RenderCopy(renderer, texture, &sr, &dr);
    if (rotation > 0) {
        SDL_Rect dr2={dx, dy, rotation, BLOCK_SIZE};
        SDL_Rect sr2={BLOCK_SIZE - rotation, 0, rotation, BLOCK_SIZE};
        SDL_RenderCopy(renderer, texture, &sr2, &dr2);
    }
}

void Draw::BlitOther(SDL_Texture * texture, int x, int y, int dx, int dy, int w, int h) {
    SDL_Rect sr={x,y,w,h};
    SDL_Rect dr={dx,dy,w, h};
    SDL_RenderCopy(renderer, texture, &sr, &dr);
}

void Draw::BlackSquare(int x, int y) {
    x *= BLOCK_SIZE;
    y *= BLOCK_SIZE;
    SDL_Rect r={x,y,BLOCK_SIZE,BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
}

void Draw::BlitBeam(int rotation, int x, int y) {
    x *= BLOCK_SIZE;
    y *= BLOCK_SIZE;
    SDL_Rect r={0, 0, 0, 0};
    switch(rotation) {
        case 0:
            r.x = x;
            r.y = y + (BLOCK_SIZE / 2) - 1;
            r.w = BLOCK_SIZE / 2 ;
            r.h = 2;
            break;
        case 1:
            r.x = x + (BLOCK_SIZE / 2) - 1;
            r.y = y;
            r.w = 2;
            r.h = BLOCK_SIZE / 2;
            break;
        case 2:
            r.x = x + BLOCK_SIZE / 2;
            r.y = y + (BLOCK_SIZE / 2) - 1;
            r.w = BLOCK_SIZE / 2;
            r.h = 2;
            break;
        case 3:
            r.x = x + (BLOCK_SIZE / 2) - 1;
            r.y = y + (BLOCK_SIZE / 2);
            r.w = 2;
            r.h = BLOCK_SIZE;
            break;
        default:
            return;
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}