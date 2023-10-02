#pragma once

#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "Coconut/ECS/ECS.h"
#include "Coconut/ECS/SpriteComponent.h"
#include "Coconut/ECS/TransformComponent.h"
#include "Coconut/Rect.h"

// I actually dont think treating tile as entity is a good practice...
namespace Coconut {

class TileComponent : public Coconut::Component {
public:
    enum class TileType { water = 0, dirt, grass, sky };

    TransformComponent* transform;
    SpriteComponent*    sprite;

    Coconut::Rect tileRect;
    TileType      tiletype;
    std::string   filename;

    TileComponent() = default;

    TileComponent(int x, int y, int w, int h, TileType tiletype_p) {
        tileRect = Coconut::Rect(x, y, w, h);
        tiletype = tiletype_p;

        switch(tiletype) {
        case TileType::water:
            filename = "water.png";
        case TileType::dirt:
            filename = "dirt.png";
        case TileType::grass:
            filename = "grass.png";
        case TileType::sky:
            filename = "sky.png";
        default:
            break;
        }
    }

    void init() override {
        
    }
};
}  // namespace Coconut