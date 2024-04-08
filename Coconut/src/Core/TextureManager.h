#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include <SDL.h>

#include "UI/Layer.h"
#include "Util/sdl_delete.h"
#include "imgui.h"

class TextureManager {
public:
    void Load(const std::string& path_to_dir, SDL_Renderer* renderer);

    [[nodiscard]] SDL_Texture* GetTexture(const std::string& name) const {
        auto it = m_texture_map.find(name);
        if(it == m_texture_map.end()) {
            CC_CORE_ERROR("TextureManager::GetTexture: No texture named [{}] in TextureManager!", name);
            return nullptr;
        }
        return it->second.get();
    }

    [[nodiscard]] const bool exists(const std::string& name) const {
        return m_texture_map.find(name) != m_texture_map.end();
    }

    [[nodiscard]] const SDL_Texture* operator[](const std::string& name) const {
        return GetTexture(name);
    }

private:
    std::unordered_map< std::string, SDL::Texture > m_texture_map;

    friend class TextureManagerDebugLayer;
};

class TextureManagerDebugLayer : public Layer {
public:
    TextureManagerDebugLayer(const TextureManager& texture_manager) : m_texture_manager(texture_manager) {}
    void GuiRender() override {
        ImGui::Begin("Texture Manager");

        for(const auto& [key, value] : m_texture_manager.m_texture_map) {
            ImGui::Text("%s: %p", key.c_str(), value.get());
        }
        ImGui::End();
    }

private:
    const TextureManager& m_texture_manager;
};