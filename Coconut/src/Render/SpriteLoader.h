#pragma once

#include <algorithm>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include <SDL.h>
#include <SDL_image.h>
#include <yaml-cpp/yaml.h>

#include "Core/Log.h"
#include "Core/TextureManager.h"
#include "Util/sdl_delete.h"

struct SpriteInfo {
    SDL_Texture* texture;
    SDL_Rect     rect;
};

class SpriteLoader {
public:
    [[nodiscard]] const SpriteInfo& GetSpriteInfo(const std::string& name) const {
        auto it = m_sprite_map.find(name);
        if(it != m_sprite_map.end()) {
            return it->second;
        }
        else {
            CC_CORE_WARN("SpriteLoader: Sprite name [{}] not found in m_sprite_map: ", name);
            throw std::runtime_error("Sprite not found."); // TODO: should not throw 
        }
    }

    void Load(std::string path_to_dir, TextureManager* tm);

private:
    void Parse(const std::filesystem::directory_entry& entry, TextureManager* tm);

private:
    std::unordered_map< std::string, SpriteInfo > m_sprite_map;
};
