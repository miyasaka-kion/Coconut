#include "Coconut/GameMap/MapTweaker.h"
#include "Coconut/Log.h"

Coconut::MapTweaker::MapTweaker() {
    m_row         = 20;
    m_column      = 25;
    s_fillMapType = FillMapType::zeros;
}

Coconut::MapTweaker::MapTweaker(int row, int col) {
    m_row         = row;
    m_column      = col;
    s_fillMapType = FillMapType::zeros;
}

Coconut::MapTweaker::MapTweaker(int row, int col, Coconut::MapTweaker::FillMapType fillMapType) {
    m_row         = row;
    m_column      = col;
    s_fillMapType = fillMapType;
}

void Coconut::MapTweaker::fillMap(std::vector<std::vector<int>>& map) {
    fitSize(map);
    CC_CORE_INFO("Map size auto fitted, with row = {} and column = {}", m_row, m_column);
    switch(s_fillMapType) {
    case FillMapType::zeros:
        fillZeros(map);
        CC_CORE_INFO("Map filled with zeros method.");
        break;
    case FillMapType::random:
        // not done
        CC_CORE_INFO("Map filled with random method.");
        break;
    default:
        // not done
        break;
    }
}

void Coconut::MapTweaker::fillZeros(std::vector<std::vector<int>>& map) {
    map.resize(m_row, std::vector<int>(m_column, 0));
}

void Coconut::MapTweaker::fitSize(std::vector<std::vector<int>>& map) {
    m_row    = static_cast<int>(map.size());
    m_column = static_cast<int>((m_row > 0) ? map[0].size() : 0);
}
