#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include "Coconut/TextureManager.h"
#include "Coconut/Vector2D.h"

std::filesystem::path Coconut::TextureManager::m_binPath = std::filesystem::current_path();

std::filesystem::path Coconut::TextureManager::m_projectPath = m_binPath.parent_path();

std::filesystem::path Coconut::TextureManager::m_assetPath = m_binPath / "assets";

SDL_Texture* Coconut::TextureManager::LoadTexture_withFullPath(std::string fileFullPath) {

    SDL_Surface* tmpSurface = IMG_Load(fileFullPath.c_str());
    SDL_Texture* texture    = SDL_CreateTextureFromSurface(Coconut::Game::renderer, tmpSurface);

    if(tmpSurface) {
        CC_CORE_INFO("Image loaded: " + fileFullPath + " successfully loaded.");
    }
    else {
        CC_CORE_ERROR("Failed to load image: " + fileFullPath);
    }

    SDL_FreeSurface(tmpSurface);

    return texture;
}

std::tuple<SDL_Texture*, int, int> Coconut::TextureManager::LoadTexture_tuple(const std::string& fileName) {
    // Load texture in the asset folder directly

    std::filesystem::path fullPath     = m_assetPath / fileName;
    std::string           fileFullPath = fullPath.string();

    SDL_Surface* tmpSurface = IMG_Load(fileFullPath.c_str());

    if(tmpSurface == nullptr) {
        CC_CORE_CRITICAL("tmpSurface is a nullptr! The specified file or path may not correct.");
        CC_CORE_CRITICAL("ERROR loading {}", fileFullPath);
        throw std::runtime_error("IMG_Load returns a nullptr!");
    }
    int imgWidth, imgHeight;
    imgWidth  = tmpSurface->w;
    imgHeight = tmpSurface->h;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Coconut::Game::renderer, tmpSurface);

    if(tmpSurface) {
        CC_CORE_INFO("TextureManager: Image loaded: " + fileFullPath + " successfully loaded.");
        CC_CORE_INFO("Image size = ({}, {}).", imgWidth, imgHeight);
    }
    else {
        CC_CORE_ERROR("Failed to load image: " + fileFullPath);
    }

    SDL_FreeSurface(tmpSurface);

    return std::tie(texture, imgWidth, imgHeight);
}

SDL_Texture* Coconut::TextureManager::LoadTexture(const std::string& fileName) {
    SDL_Texture* texture;
    std::tie(texture, std::ignore, std::ignore) = Coconut::TextureManager::LoadTexture_tuple(fileName);
    return texture;
}

Coconut::ImageObject Coconut::TextureManager::LoadTextureImageObject(const std::string& fileName) {
    // ToDo...
    return Coconut::ImageObject();
}

void Coconut::TextureManager::DrawTexture(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest) {
    SDL_RenderCopy(Coconut::Game::renderer, texture, &src, &dest);
}

void Coconut::TextureManager::showFileInfo() {

    CC_CORE_INFO("TextManager Initialization: Current executable directory is: " + m_binPath.string());

    CC_CORE_INFO("TextManager Initialization: Current project directory is: " + m_projectPath.string());

    CC_CORE_INFO("TextManager Initialization: Current asset directory is: " + m_assetPath.string());
}
