#include "Coconut/GameMap/Map.h"

Coconut::Map::Map()
    : m_rows(20), m_columns(25) {
    m_map.resize(m_rows, std::vector<int>(m_columns, 0));
    m_dirt = Coconut::TextureManager::LoadTexture("dirt.jpg");
    m_grass = Coconut::TextureManager::LoadTexture("grass.png");
    m_water = Coconut::TextureManager::LoadTexture("water.png");
    m_sky = Coconut::TextureManager::LoadTexture("sky.png");

    loadMap();
    src.x = src.y = 0;
    // This should be change with respect to a certain size of a texture file
    // Not changed yet!
    // ToDo: make some automatic method to determin the size of a texture file 
    src.w = dest.w = src.h = dest.h = 32;

    dest.x = dest.y = 0;
}


//The function above is actually in use;
//This function will be moved in later version.
Coconut::Map::Map(int rows = 20, int cols = 25)
    : m_rows(rows), m_columns(cols) {
    m_map.resize(m_rows, std::vector<int>(m_columns, 0));
    m_dirt = Coconut::TextureManager::LoadTexture("dirt.jpg");
    m_grass = Coconut::TextureManager::LoadTexture("grass.png");
    m_water = Coconut::TextureManager::LoadTexture("water.png");
    m_sky = Coconut::TextureManager::LoadTexture("sky.png");

    loadMap();
    src.x = src.y = 0;
    // This should be change with respect to a certain size of a texture file
    // Not changed yet!
    // ToDo: make some automatic method to determin the size of a texture file 
    src.w = dest.w = src.h = dest.h = 32;

    dest.x = dest.y = 0;
}


void Coconut::Map::loadMap() {
    Coconut::MapTweaker mpt;
    mpt.fillMap(m_map);
    
}



void Coconut::Map::drawMap() {
    if (m_map.size() == 0) {
        CC_CORE_CRITICAL("Try to draw map with map size = 0!");
        return;
    }
    for (int i = 0; i < m_map.size(); i++) {
        for (int j = 0; j < m_map[0].size(); j++) {
            //use enum?
            int curType = m_map[i][j];
            
            dest.x = i * 32;
            dest.y = j * 32;
            if (i || j) break;
            switch (curType) {
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
