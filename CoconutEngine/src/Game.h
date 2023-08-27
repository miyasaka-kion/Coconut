#pragma once

#include "Log.h"

#pragma warning(push)
#pragma warning(disable: 26819) // Disable warning C26819
#include <SDL.h>// Include the external dependency header
#pragma warning(pop) 

#include <string>


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

		SDL_Window* m_window;
		//std::shared_ptr<SDL_Window> m_window;
		
		SDL_Renderer* m_renderer;
		//std::shared_ptr<SDL_Window> m_renderer;
	};
}