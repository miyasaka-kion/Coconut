#pragma once

#include "Log.h"

#include <SDL.h>

#include <string>


namespace Coconut {
	class Game {
	public:
		Game();
		~Game();
		
		void gameInit(const char* title, int x, int y, int width, int height, bool fullscreen);
		
		void handleEvents();
		void update();
		void render();
		void clean();
		
		bool running();

	private:
		bool m_isRunning;
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
	};
}