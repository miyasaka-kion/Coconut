#include "Render/SpriteLoader.h"

void SpriteLoader::Load(std::string path_to_dir, TextureManager* tm) {

    for(const auto& entry : std::filesystem::directory_iterator(path_to_dir)) {
        if(entry.is_regular_file()) {  
            // if the file is a hidden file, skip
            if(entry.path().filename().string().at(0) == '.') {
                continue;
            }
            auto filename  = entry.path().filename().string();
            auto stem = entry.path().stem().string();
            auto extension = entry.path().extension().string();
            if(extension == ".yaml" || extension == ".yml") {
                CC_CORE_INFO("Sprite config found, location: {}", path_to_dir + "/" + filename);
                if(tm->exists(stem)) {
                    CC_CORE_INFO("Corresponding texture found, begin parsing...");
                    Parse(entry, tm);
                }
                else {
                    CC_CORE_WARN("SpriteLoader::Load: no texture named [{}] in TextureManager!", stem);
                }
            }
        }
        else {
            CC_CORE_WARN("SpriteLoader::Load: unsupported file type: {}", entry.path().filename().string());
        }
    }
}

void SpriteLoader::Parse(const std::filesystem::directory_entry& entry, TextureManager* tm) {
    auto config = YAML::LoadFile(entry.path().generic_string());

    auto spriteSheet = config["TextureImporter"]["spriteSheet"];

    if(!spriteSheet["sprites"].IsSequence()) {
        throw std::runtime_error("The entry 'sprites' in 'TextureImporter' is not a sequence.");
    }

    
    for(auto s : spriteSheet["sprites"]) {
        SpriteInfo info;
        // info.name = s["name"].as< std::string >();

        std::string name = s["name"].as< std::string >();
        CC_CORE_INFO("Loading Sprite [{}] ...", name);

        auto rect   = s["rect"];
        info.rect.x     = rect["x"].as< int >();
        info.rect.y      = rect["y"].as< int >();
        info.rect.w  = rect["width"].as< int >();
        info.rect.h = rect["height"].as< int >();

        auto texture_name = entry.path().stem().string();

        info.texture = tm->GetTexture(texture_name);
        // std::cout << info.x << " " << info.y << " " << info.width << " " << info.height << std::endl;

        m_sprite_map[name] = info;
    }
}