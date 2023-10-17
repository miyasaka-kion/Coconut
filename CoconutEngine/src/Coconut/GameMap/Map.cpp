#include "Coconut/GameMap/Map.h"
#include "Coconut/ConstantSpec.h"

Coconut::Map::Map() : m_rows(Coconut::Constants::MAP_WIDTH), m_columns(Coconut::Constants::MAP_HEIGHT) {
    m_map.resize(m_rows, std::vector<int>(m_columns, 0));
    CC_CORE_INFO("Begin to load map sprites:");
    m_dirt  = Coconut::TextureManager::LoadTexture("dirt.jpg");
    m_grass = Coconut::TextureManager::LoadTexture("grass.png");
    m_water = Coconut::TextureManager::LoadTexture("water.png");
    m_sky   = Coconut::TextureManager::LoadTexture("sky.png");
    CC_CORE_INFO("CC load map sprites finished!");

    loadMap();
    src.x = src.y = 0;
    // This should be change with respect to a certain size of a texture file
    // Not changed yet!
    // ToDo: make some automatic method to determin the size of a texture file
    src.w = dest.w = Coconut::Constants::MAP_TILE_WIDTH;
    src.h = dest.h = Coconut::Constants::MAP_TILE_HEIGHT;

    dest.x = dest.y = 0;
}

Coconut::Map::~Map() {
    SDL_DestroyTexture(m_dirt);
    SDL_DestroyTexture(m_sky);
    SDL_DestroyTexture(m_water);
    SDL_DestroyTexture(m_grass);
    CC_CORE_INFO("Map resource cleaned");
}

void Coconut::Map::loadMap() {
    Coconut::MapTweaker mpt;
    mpt.fillMap(m_map);
}

void Coconut::Map::drawMap() {
    if(m_map.size() == 0) {
        CC_CORE_CRITICAL("Try to draw map with map size = 0!");
        return;
    }
    for(int i = 0; i < m_map.size(); i++) {
        for(int j = 0; j < m_map[0].size(); j++) {
            // use enum?
            int curType = m_map[i][j];

            dest.x = i * Coconut::Constants::DEST_RECT_WIDTH;
            dest.y = j * Coconut::Constants::DEST_RECT_HEIGHT;

            switch(curType) {
            case 0:
                Coconut::TextureManager::DrawTexture(m_water, src, dest);
                break;
            case 1:
                Coconut::TextureManager::DrawTexture(m_water, src, dest);
                break;
                // This will be rewrite with enum method; not implement this tmply.
            default:
                break;
            }
        }
    }
}
