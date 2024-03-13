#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include <SDL.h>

#include "Util/sdl_delete.h"


class TextureManager {
public:
    void Load(const std::string& path_to_dir, SDL_Renderer* renderer);

    [[nodiscard]]  SDL_Texture* GetTexture(const std::string& name) const {
        auto it = m_texture_map.find(name);
        if (it == m_texture_map.end()) {
            CC_CORE_ERROR("TextureManager::GetTexture: no texture named [{}] in TextureManager!", name);
            return nullptr;
        }
        return it->second.get();
    }

    [[nodiscard]] const bool exists(const std::string& name) const  {
        return m_texture_map.find(name) != m_texture_map.end();
    }

    [[nodiscard]] const SDL_Texture* operator[](const std::string& name) const  {
        return GetTexture(name);
    }

private:
    std::unordered_map< std::string, SDL::Texture > m_texture_map;
};