#ifndef ASSETS_H
#define ASSETS_H

#include <string>
#include <SDL.h>

class Textures {

public:

    Textures(SDL_Renderer * renderer);
    ~Textures();

    bool load();

    SDL_Texture * getBarSprites() { return barSprites; };
    SDL_Texture * getBeamSprites() { return beamSprites; };
    SDL_Texture * getGroundSprites() { return groundSprites; };
    SDL_Texture * getMirrorSprites() { return mirrorSprites; };
    SDL_Texture * getNukeSprites() { return nukeSprites; };
    SDL_Texture * getRedblockSprites() { return redblockSprites; };
    SDL_Texture * getRustySprites() { return rustySprites; };
    SDL_Texture * getStaticSprites() { return staticSprites; };
    SDL_Texture * getTankSprites() { return tankSprites; };
    SDL_Texture * getTeeSprites() { return teeSprites; };

    SDL_Texture * getMainSprite() { return mainSprite; };
    SDL_Texture * getMessageSprite() { return messageSprite; };
    SDL_Texture * getWaterSprite() { return waterSprite; };

private:

    SDL_Renderer * renderer;

    SDL_Texture * barSprites;
    SDL_Texture * beamSprites;
    SDL_Texture * groundSprites;
    SDL_Texture * mirrorSprites;
    SDL_Texture * nukeSprites;
    SDL_Texture * redblockSprites;
    SDL_Texture * rustySprites;
    SDL_Texture * staticSprites;
    SDL_Texture * tankSprites;
    SDL_Texture * teeSprites;

    SDL_Texture * mainSprite;
    SDL_Texture * messageSprite;
    SDL_Texture * waterSprite;

    SDL_Texture * loadImage(const std::string &filename);

};

#endif // ASSETS_H