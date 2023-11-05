#include "Coconut/TextureManager.h"
#include "Coconut/Log.h"
#include <SDL_error.h>

#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

std::filesystem::path Coconut::TextureManager::m_binPath = std::filesystem::current_path();
std::filesystem::path Coconut::TextureManager::m_projectPath = m_binPath.parent_path();
std::filesystem::path Coconut::TextureManager::m_assetPath = m_binPath / "assets";

std::tuple<SDL_Texture*, int, int> Coconut::TextureManager::LoadTexture_tuple(const std::string& fileName) {
    // Load texture in the asset folder directly

    std::filesystem::path full_path     = m_assetPath / fileName;
    std::string           full_path_str = full_path.string();

    SDL_Surface* tmpSurface = IMG_Load(full_path_str.c_str());

    if(tmpSurface == nullptr) {
        CC_CORE_CRITICAL("tmpSurface is a nullptr! The specified file or path may not correct.");
        CC_CORE_CRITICAL("ERROR loading {}", full_path_str);
        throw std::runtime_error("IMG_Load returns a nullptr!");
    }
    int imgWidth, imgHeight;
    imgWidth  = tmpSurface->w;
    imgHeight = tmpSurface->h;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Coconut::Game::renderer, tmpSurface);
    if (texture == NULL) {
        CC_CORE_ERROR("SDL_CreateTextureFromSurface Error: {}", SDL_GetError());
    }

    if(tmpSurface) {
        CC_CORE_INFO("TextureManager: Image loaded: " + full_path_str + " successfully loaded.");
        CC_CORE_INFO("Image size = ({}, {}).", imgWidth, imgHeight);
    }
    else {
        CC_CORE_ERROR("Failed to load image: " + full_path_str);
    }

    SDL_FreeSurface(tmpSurface);

    return std::tie(texture, imgWidth, imgHeight);
}

SDL_Texture* Coconut::TextureManager::LoadTexture(const std::string& fileName) {
    SDL_Texture* texture;
    std::tie(texture, std::ignore, std::ignore) = Coconut::TextureManager::LoadTexture_tuple(fileName);
    return texture;
}

// Coconut::ImageObject Coconut::TextureManager::LoadTextureImageObject(const std::string& fileName) {
//     // ToDo...
//     return Coconut::ImageObject();
// }

void Coconut::TextureManager::DrawTexture(SDL_Texture* texture, SDL_Rect src_rect, SDL_Rect dest_rect) {
    if(SDL_RenderCopy(Coconut::Game::renderer, texture, &src_rect, &dest_rect)) {
        CC_CORE_ERROR("DrawTexture Error: {}", SDL_GetError());
    }
}

// render full texture from source
void Coconut::TextureManager::DrawTexture(SDL_Texture* texture, SDL_Rect dest_rect) {
    if(SDL_RenderCopy(Coconut::Game::renderer, texture, NULL, &dest_rect)) {
        CC_CORE_ERROR("DrawTexture Error: {}", SDL_GetError());
    }
}

void Coconut::TextureManager::showFileInfo() {
    CC_CORE_INFO("TextManager Initialization: Current executable directory is: " + m_binPath.string());
    CC_CORE_INFO("TextManager Initialization: Current project directory is: " + m_projectPath.string());
    CC_CORE_INFO("TextManager Initialization: Current asset directory is: " + m_assetPath.string());
}
