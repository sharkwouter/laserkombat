#ifndef DRAW_H
#define DRAW_H

#include <SDL.h>
#include <SDL_ttf.h>

class Draw {

public:
    Draw(SDL_Renderer * renderer);
    ~Draw();

    void BlitSquare(SDL_Texture * texture, int x, int y, int dx, int dy);
    void BlitOther(SDL_Texture * texture, int x, int y, int dx, int dy, int w, int h);
    void BlackSquare(int x, int y);
    void BlitText(char * text, int x, int y);
    void Flip();

    int animation;

private:
    SDL_Renderer * renderer;
    SDL_Texture * renderTarget;
    TTF_Font * font;
};

#endif // DRAW_H