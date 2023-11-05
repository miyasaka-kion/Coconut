// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#pragma once

#include <filesystem>

#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include "Coconut/Game.h"
#include "Coconut/ImgManager.h"
#include "Coconut/Log.h"

namespace Coconut {
class TextureManager {
public:
    static SDL_Texture* LoadTexture_withFullPath(std::string fileName);
    // This is a test function, will be remove in release.

    // @return texture, width, height
    static std::tuple<SDL_Texture*, int, int> LoadTexture_tuple(const std::string& fileName);
    static SDL_Texture*                       LoadTexture(const std::string& fileName);
    static Coconut::ImageObject               LoadTextureImageObject(const std::string& fileName);

    static void DrawTexture(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest);
    static void DrawTexture(SDL_Texture* texture, SDL_Rect src);
    static void showFileInfo();

private:
    static std::filesystem::path m_binPath;
    static std::filesystem::path m_projectPath;
    static std::filesystem::path m_assetPath;
};

}  // namespace Coconut
