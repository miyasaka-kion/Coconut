#pragma once

#include "Coconut/Game.h"
#include "Coconut/TextureManager.h"
#include "Coconut/Log.h"
#include "Coconut/Object.h"

#include <SDL.h>
#include <SDL_image.h>



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

	private:
		bool m_isRunning;

		int m_gameCounter;

		SDL_Window* m_window;
		//std::shared_ptr<SDL_Window> m_window;
		
		SDL_Renderer* m_renderer;
		//std::shared_ptr<SDL_Window> m_renderer;
	};
}