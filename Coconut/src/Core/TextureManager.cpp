#include "Core/TextureManager.h"

#include <SDL.h>
#include <SDL_image.h>

#include "Util/sdl_check.h"

void TextureManager::Load(const std::string& path_to_dir, SDL_Renderer* renderer) {
    for(const auto& entry : std::filesystem::directory_iterator(path_to_dir)) {
        if(entry.is_regular_file()) {  // Check if the entry is a regular file
            auto filename  = entry.path().filename().string();
            auto extension = entry.path().extension().string();
            auto stem      = entry.path().stem().string();
            if(extension == ".png") {
                CC_CORE_INFO("LoadTexture(): Loading texture, texture path: {}", path_to_dir);
                CC_CORE_INFO("LoadTexture(): Loading texture, texture name: {}", filename);
                IMG_Init(IMG_INIT_PNG);

                SDL::Surface tmp_sprites{ SDL_CHECK(IMG_Load(entry.path().c_str())) };

                auto texture        = SDL_CHECK(SDL_CreateTextureFromSurface(renderer, tmp_sprites.get()));
                m_textureMap[stem] = SDL::Texture(texture);
                CC_CORE_INFO("LoadTexture(): Texture Loaded, texture name: {}", filename);
            }
        }
    }
}