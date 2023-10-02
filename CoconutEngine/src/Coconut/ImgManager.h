#pragma once

#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <tuple>

namespace Coconut {
class ImageObject {
public:
    SDL_Texture* texture;
    int          height;
    int          width;

    ImageObject() : texture(nullptr), height(0), width(0) {}
    ImageObject(SDL_Texture* texture_p, int height_p, int width_p) : texture(texture_p), height(height_p), width(width_p) {}
    ImageObject(std::tuple<SDL_Texture*, int, int> tp) {
        std::tie(texture, width, height) = tp;
    }
};
}  // namespace Coconut