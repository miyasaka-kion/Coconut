#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Coconut/TextureManager.h"
#include "Coconut/Log.h"
#include "Coconut/Object.h"
#include "Coconut/GameMap/MapTweaker.h"
#include "Coconut/GameMap/Map.h"

namespace Coconut {
	class Game {
	public:
		Game();
		~Game();
		
		void gameInit(std::string title, int x, int y, int width, int height, bool fullscreen);
		
		void handleEvents();
		void update();
		void render();
		void clean();
		
		inline bool running() const {
			return m_isRunning;
		}
		
	static SDL_Renderer* renderer;

	private:
		bool m_isRunning;
		int m_gameCounter;
		SDL_Window* m_window;
	};
}