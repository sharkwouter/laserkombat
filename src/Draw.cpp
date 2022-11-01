#include "Draw.h"

#include "utils.h"
#include "constants.h"

Draw::Draw(SDL_Renderer * renderer) : renderer(renderer) {
    animation = 0;
    renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_TARGET, BLOCK_SIZE*COLUMNS, BLOCK_SIZE*ROWS);
    font = TTF_OpenFont(getAssetPath("fonts", "FreeSans.ttf").c_str(), 14);
}

Draw::~Draw() {
    SDL_DestroyTexture(renderTarget);
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
    SDL_Rect sr, dr;
    dx *= BLOCK_SIZE;
    dy *= BLOCK_SIZE;
    for (int i = 0; i < 4; i++){
        switch (i)
        {
        case 0:
            sr = {0,0,rotation, rotation};
            dr = {dx, dy, rotation, rotation};
            break;
        case 1:
            sr = {rotation, 0,BLOCK_SIZE-rotation, rotation};
            dr = {dx+rotation, dy, BLOCK_SIZE-rotation, rotation};
            break;
        case 2:
            sr = {0,rotation,rotation, BLOCK_SIZE-rotation};
            dr = {dx, dy+rotation, rotation, BLOCK_SIZE-rotation};
            break;
        case 3:
            sr = {rotation,rotation,BLOCK_SIZE-rotation, BLOCK_SIZE-rotation};
            dr = {dx+rotation, dy+rotation, BLOCK_SIZE-rotation, BLOCK_SIZE-rotation};
            break;
        default:
            break;
        }
        SDL_RenderCopy(renderer, texture, &sr, &dr);
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