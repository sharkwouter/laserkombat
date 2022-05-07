#include "Textures.h"

#include "utils.h"

Textures::Textures(SDL_Renderer * renderer) : renderer(renderer) {
    if (!load()) {
        exit(4);
    }
}

Textures::~Textures() {

}

bool Textures::load() {
        barSprites = loadImage("bar.bmp");
        beamSprites = loadImage("beam.bmp");
        groundSprites = loadImage("ground.bmp");
        mirrorSprites = loadImage("mirror.bmp");
        nukeSprites= loadImage("nuke.bmp");
        redblockSprites = loadImage("redblock.bmp");
        rustySprites = loadImage("rusty.bmp");
        staticSprites = loadImage("static.bmp");
        tankSprites = loadImage("tank.bmp");
        teeSprites = loadImage("tee.bmp");

        mainKeysSprite = loadImage("maink.bmp");
        mainBlocksSprite = loadImage("mainb.bmp");
        messageSprite = loadImage("message.bmp");
        levelInfoSprite = loadImage("levelinfo.bmp");
        waterSprite = loadImage("water.bmp");

        return
            barSprites != nullptr &&
            beamSprites != nullptr &&
            groundSprites != nullptr &&
            mirrorSprites != nullptr &&
            nukeSprites != nullptr &&
            redblockSprites != nullptr &&
            rustySprites != nullptr &&
            staticSprites != nullptr &&
            tankSprites != nullptr &&
            teeSprites != nullptr &&
            mainKeysSprite != nullptr &&
            mainBlocksSprite != nullptr &&
            messageSprite != nullptr &&
            levelInfoSprite != nullptr &&
            waterSprite != nullptr;

}

SDL_Texture * Textures::loadImage(const std::string &filename) {
    std::string path = getAssetPath("images", filename);
    uint32_t colorKey;
    SDL_Surface *surface = SDL_LoadBMP(path.c_str());
    if (!surface) {
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Failed to load image %s: %s", path.c_str(), SDL_GetError());
        return nullptr;
    }

    // Key out black if this is not static.bmp
    if (filename != "static.bmp") {
        colorKey = SDL_MapRGB(surface->format, 0, 0, 0);
        SDL_SetColorKey(surface, SDL_TRUE, colorKey);
    }

    SDL_Texture * texture = SDL_CreateTextureFromSurface(this->renderer, surface);
    if (!texture) {
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Failed to load texture from surface for image %s: %s", path.c_str(), SDL_GetError());
    }

    return texture;
}