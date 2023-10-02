#pragma once

namespace Coconut {
namespace Constants {
    constexpr int WINDOW_WIDTH  = 800;
    constexpr int WINDOW_HEIGHT = 600;

    constexpr int ENTITY_INITIAL_POSITION_X = 0;
    constexpr int ENTITY_INITIAL_POSITION_Y = 0;

    constexpr int DEST_RECT_WIDTH  = 64;
    constexpr int DEST_RECT_HEIGHT = 64;

    // I think this should be automatically defered from the texture picture
    constexpr int BIRD_HIGHT = 630;
    constexpr int BIRD_WIDTH = 765;

    constexpr float defaultScale = 0.1f;
    constexpr float BIRD_SCALE   = defaultScale;

    constexpr int MAP_TILE_WIDTH = 64;
    constexpr int MAP_TILE_HEIGHT = 64;

    constexpr int MAP_WIDTH = WINDOW_WIDTH / MAP_TILE_WIDTH + 1;
    constexpr int MAP_HEIGHT = WINDOW_HEIGHT / MAP_TILE_HEIGHT + 1;

}  // namespace Constants
}  // namespace Coconut