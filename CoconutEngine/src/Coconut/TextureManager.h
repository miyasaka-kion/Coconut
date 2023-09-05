#pragma once

#include "Coconut/Log.h"
#include "Coconut/Game.h"

#include <SDL.h>
#include <SDL_image.h>
#include <filesystem>

namespace Coconut {
	class TextureManager {
	public:
		TextureManager();
		~TextureManager();

	public:
		static SDL_Texture* LoadTexture_withFullPath(std::string fileName);

		static SDL_Texture* LoadTexture(std::string fileName);

		static void showFileInfo();
	private:
		static std::filesystem::path m_binPath;
		static std::filesystem::path m_projectPath;
		static std::filesystem::path m_assetPath;
	};

}


