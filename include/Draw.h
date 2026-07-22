#ifndef DRAW_H
#define DRAW_H

#include <SDL.h>
#include <SDL_ttf.h>

class Draw {

public:
    Draw(SDL_Renderer * renderer);
    ~Draw();

    void BlitSquare(SDL_Texture * texture, int x, int y, int dx, int dy);
    void BlitWater(SDL_Texture * texture, int rotation, int dx, int dy);
    void BlitOther(SDL_Texture * texture, int x, int y, int dx, int dy, int w, int h);
    void BlackSquare(int x, int y);
    void BlitBeam(int rotation, int x, int y);
    void BlitMessage(const char * title, const char ** lines, int line_count);
    void BlitMessageBox(SDL_Rect * box);
    void BlitLevelInfo(int level, char * description, char * author);
    
    // This draws a frame around the rendered game, only applies to certain platforms
    void BlitFrame(SDL_Texture * logoSprite);

    void BlitText(char * text, TTF_Font * font, int x, int y, SDL_Color color={0, 0, 0, 255});
    void Flip();

    int animation;

private:
    SDL_Renderer * renderer;
    TTF_Font * font_custom;
    TTF_Font * font_text;
    TTF_Font * font_title;
    TTF_Font * font_title_italic;

    int last_level = 0;
    SDL_Texture * level_text = NULL;
    SDL_Texture * author_text = NULL;
    SDL_Texture * author_info_text = NULL;
    SDL_Texture * description_text = NULL;

    SDL_Texture * stringToTexture(SDL_Renderer * renderer, char * text, TTF_Font * font, SDL_Color color);
};

#endif // DRAW_H