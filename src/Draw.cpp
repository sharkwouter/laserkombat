    #include "Draw.h"

    #include "constants.h"
    
    static void Draw::BlitSquare(SDL_Renderer * renderer, SDL_Texture * texture, int x, int y, int dx, int dy) {
        x *= IMAGE_WIDTH;
        y *= IMAGE_WIDTH;
        dx *= IMAGE_WIDTH;
        dy *= IMAGE_WIDTH;
        SDL_Rect sr={x,y,IMAGE_WIDTH, IMAGE_WIDTH};
        SDL_Rect dr={dx,dy,IMAGE_WIDTH, IMAGE_WIDTH};
        SDL_RenderCopy(renderer, texture, &sr, &dr);
    }

    static void Draw::Blit(SDL_Renderer * renderer, SDL_Texture * texture, int x, int y, int dx, int dy) {

    }

    static void Draw::BlitOther(SDL_Renderer * renderer, SDL_Texture * texture, int x, int y, int dx, int dy, int w, int h) {
        SDL_Rect sr={x,y,w,h};
        SDL_Rect dr={dx,dy,w, h};
        SDL_RenderCopy(renderer, texture, &sr, &dr);
    }

    static void Draw::BlackSquare(SDL_Renderer * renderer, SDL_Texture * texture, int dx, int dy) {

    }
