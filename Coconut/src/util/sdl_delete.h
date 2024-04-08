#pragma once

#include <memory>

#include <SDL2/SDL.h>

#include "Core/Log.h"

namespace SDL {

struct DeleteWindow {
    void operator()(SDL_Window* ptr) const noexcept {
        CC_CORE_INFO("SDL window [{}] destroyed.", static_cast<void *>(ptr));
        SDL_DestroyWindow(ptr);
    }
};

struct DeleteTexture {
    void operator()(SDL_Texture* ptr) const noexcept {
    CC_CORE_INFO("SDL texture [{}] destroyed", static_cast<void *>(ptr));
        SDL_DestroyTexture(ptr);
    }
};

struct DeleteSurface {
    void operator()(SDL_Surface* ptr) const noexcept {
        SDL_FreeSurface(ptr);
    }
};

struct DeleteRenderer {
    void operator()(SDL_Renderer* ptr) const noexcept {
        CC_CORE_INFO("SDL renderer [{}] destroyed.", static_cast<void *>(ptr));
        SDL_DestroyRenderer(ptr);
    }
};

using Window   = std::unique_ptr< SDL_Window, DeleteWindow >;
using Texture  = std::unique_ptr< SDL_Texture, DeleteTexture >;
using Surface  = std::unique_ptr< SDL_Surface, DeleteSurface >;
using Renderer = std::unique_ptr< SDL_Renderer, DeleteRenderer >;

}  // namespace SDL