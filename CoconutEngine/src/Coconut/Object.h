#pragma once
#include "Coconut/Game.h"
#include "Coconut/TextureManager.h"

namespace Coconut {
	class Object {
	public:
		Object(std::string textureSheet, SDL_Renderer* renderer);
		Object(std::string textureSheet, SDL_Renderer* renderer, int ix, int iy);
		//(ix, iy) is the initial position of the object
		~Object();

		void objUpdate();
		void objRender();
	private:
		int m_xpos;
		int m_ypos;
		
		SDL_Texture* m_objectTexture;
		SDL_Rect m_srcRect;
		SDL_Rect m_destRect;
		SDL_Renderer* m_renderer;
	};
}