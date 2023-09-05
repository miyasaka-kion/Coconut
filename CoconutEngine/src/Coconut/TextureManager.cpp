#include "TextureManager.h"

namespace Coconut {
	std::filesystem::path TextureManager::m_binPath = std::filesystem::current_path();
	std::filesystem::path TextureManager::m_projectPath = m_binPath.parent_path();
	std::filesystem::path TextureManager::m_assetPath = m_binPath / "assets";

	TextureManager::TextureManager() {

	}

	TextureManager::~TextureManager() {
	}

	SDL_Texture* TextureManager::LoadTexture_withFullPath(std::string fileFullPath, SDL_Renderer* renderer) {

		SDL_Surface* tmpSurface = IMG_Load(fileFullPath.c_str());
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

		if (tmpSurface) {
			CC_CORE_INFO("Image loaded: " + fileFullPath + " successfully loaded.");
		}
		else {
			CC_CORE_ERROR("Failed to load image: " + fileFullPath);
		}

		SDL_FreeSurface(tmpSurface);

		return texture;
	}

	SDL_Texture* TextureManager::LoadTexture(std::string fileName, SDL_Renderer* renderer) {
		std::filesystem::path fullPath = m_assetPath / fileName;
		std::string fileFullPath = fullPath.string();

		SDL_Surface* tmpSurface = IMG_Load(fileFullPath.c_str());
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

		if (tmpSurface) {
			CC_CORE_INFO("TextureManager: Image loaded: " + fileFullPath + " successfully loaded.");
		}
		else {
			CC_CORE_ERROR("Failed to load image: " + fileFullPath);
		}

		SDL_FreeSurface(tmpSurface);

		return texture;
	}

	void TextureManager::showFileInfo() {
		CC_CORE_INFO("TextManager Initialization: Current executable directory is: " + m_binPath.string());

		CC_CORE_INFO("TextManager Initialization: Current project directory is: " + m_projectPath.string());

		CC_CORE_INFO("TextManager Initialization: Current asset directory is: " + m_assetPath.string());
	}
}


