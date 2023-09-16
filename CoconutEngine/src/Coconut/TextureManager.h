#pragma once

#include <filesystem>

#pragma warning(push, 0)
#include <SDL.h>
#include <SDL_image.h>
#pragma warning(pop)

#include "Coconut/Log.h"
#include "Coconut/Game.h"



namespace Coconut {
	class TextureManager {
	public:
		TextureManager();
		~TextureManager();

	public:
		static SDL_Texture* LoadTexture_withFullPath(std::string fileName);
		// This is a test function, will be remove in release.

		static SDL_Texture* LoadTexture(std::string fileName);
		static void DrawTexture(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest);

		static void showFileInfo();
	private:
		static std::filesystem::path m_binPath;
		static std::filesystem::path m_projectPath;
		static std::filesystem::path m_assetPath;
	};

}


