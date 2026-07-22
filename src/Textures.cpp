#include "Textures.h"

#include "utils.h"
#include "constants.h"

Textures::Textures(SDL_Renderer * renderer) : renderer(renderer) {
    if (!load()) {
        exit(4);
    }
}

Textures::~Textures() {
    SDL_DestroyTexture(barSprites);
    SDL_DestroyTexture(groundSprites);
    SDL_DestroyTexture(mirrorSprites);
    SDL_DestroyTexture(nukeSprites);
    SDL_DestroyTexture(redblockSprites);
    SDL_DestroyTexture(rustySprites);
    SDL_DestroyTexture(staticSprites);
    SDL_DestroyTexture(tankSprites);
    SDL_DestroyTexture(waterSprite);

    if (LARGE_TEXTURES_SUPPORTED) {
        SDL_DestroyTexture(mainKeysSprite);
        SDL_DestroyTexture(mainBlocksSprite);
        SDL_DestroyTexture(creditsSprite);
    }

    if (DRAW_FRAME) {
        SDL_DestroyTexture(logoSprite);
    }
}

bool Textures::load() {
        barSprites = loadImage("bar.bmp");
        groundSprites = loadImage("ground.bmp");
        mirrorSprites = loadImage("mirror.bmp");
        nukeSprites= loadImage("nuke.bmp");
        redblockSprites = loadImage("redblock.bmp");
        rustySprites = loadImage("rusty.bmp");
        staticSprites = loadImage("static.bmp");
        tankSprites = loadImage("tank.bmp");
        teeSprites = loadImage("tee.bmp");
        waterSprite = loadImage("water.bmp");

        if (LARGE_TEXTURES_SUPPORTED) {
            mainKeysSprite = loadImage("maink.bmp");
            mainBlocksSprite = loadImage("mainb.bmp");
            creditsSprite = loadImage("credits.bmp");
        }

        if (DRAW_FRAME) {
            logoSprite = loadImage("logo.bmp");
            if (logoSprite == nullptr) {
                return false;
            }
        }

        return
            barSprites != nullptr &&
            groundSprites != nullptr &&
            mirrorSprites != nullptr &&
            nukeSprites != nullptr &&
            redblockSprites != nullptr &&
            rustySprites != nullptr &&
            staticSprites != nullptr &&
            tankSprites != nullptr &&
            teeSprites != nullptr &&
            waterSprite != nullptr &&
            (mainKeysSprite != nullptr || !LARGE_TEXTURES_SUPPORTED) &&
            (mainBlocksSprite != nullptr || !LARGE_TEXTURES_SUPPORTED) &&
            (creditsSprite != nullptr || !LARGE_TEXTURES_SUPPORTED);

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
    SDL_FreeSurface(surface);

    return texture;
}