#include "Textures.h"

Textures::Textures(SDL_Renderer * renderer) : renderer(renderer) {

}

Textures::~Textures() {

}

bool Textures::load() {
    return
        loadImage(barSprites, "bar.bmp") &&
        loadImage(beamSprites, "beam.bmp") &&
        loadImage(groundSprites, "ground.bmp") &&
        loadImage(mirrorSprites, "mirror.bmp") &&
        loadImage(nukeSprites, "nuke.bmp") &&
        loadImage(redblockSprites, "redblock.bmp") &&
        loadImage(rustySprites, "rusty.bmp") &&
        loadImage(staticSprites, "static.bmp") &&
        loadImage(tankSprites, "tank.bmp") &&
        loadImage(teeSprites, "tee.bmp") &&

        loadImage(mainSprite, "main.bmp") &&
        loadImage(messageSprite, "message.bmp") &&
        loadImage(waterSprite, "water.bmp");
}

bool Textures::loadImage(SDL_Texture * texture, const std::string &filename) {
    std::string path = "assets/images/" + filename;

    SDL_Surface *surface = SDL_LoadBMP(path.c_str());
    if (!surface) {
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Failed to load image %s: %s", path.c_str(), SDL_GetError());
        return false;
    }

    texture = SDL_CreateTextureFromSurface(this->renderer, surface);
    if (texture == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Failed to load texture from surface for image %s: %s", path.c_str(), SDL_GetError());
        return false;
    }

    return true;
}