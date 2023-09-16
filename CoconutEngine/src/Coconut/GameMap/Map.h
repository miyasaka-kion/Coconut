#pragma once
#include <vector>

#pragma warning(push, 0)
#include <SDL.h>
#include <SDL_image.h>
#pragma push(pop)

#include "Coconut/TextureManager.h"
#include "Coconut/GameMap/MapTweaker.h"

namespace Coconut {
	class Map {
	public:
		//enum class 
		Map();
		Map(int rows, int cols);
		//~Map();

		void loadMap();

		void drawMap();
	private:
		SDL_Rect src, dest;
		SDL_Texture* m_dirt;
		SDL_Texture* m_grass;
		SDL_Texture* m_water;
		SDL_Texture* m_sky;

		const int m_rows;
		const int m_columns;
		std::vector<std::vector<int>> m_map;


	};
}
