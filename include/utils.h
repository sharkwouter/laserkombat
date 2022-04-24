#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>

#include <string>

static std::string getAssetPath(std::string type, std::string file) {
    std::string path = "";
    #if defined(NXDK) || defined(WIN32)
        std::string separator = "\\";
    #else
        std::string separator = "/";
    #endif

    #ifdef NXDK
        path += "D:\\";
    #else
    char * base_path = SDL_GetBasePath();
    if (base_path) {
        path += base_path;
        SDL_free(base_path);
    }
    #endif

    path += "assets";
    path += separator;
    path += type;
    path += separator;
    path += file;

    return path;
}

#endif // UTILS_H