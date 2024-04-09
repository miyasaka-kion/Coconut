#include "Tile.h"

#include "Render/QuadWrite.h"

EdgeTile::EdgeTile(SDL_Renderer* renderer,  SpriteInfo& sprite_info, b2Vec2 start, b2Vec2 end, float width) : m_renderer(renderer), m_sprite_info(sprite_info), m_start(start), m_end(end) {
    float h_over_w = static_cast<float > (sprite_info.rect.h) / static_cast<float>(sprite_info.rect.w);
    m_size.x      = width;
    m_size.y      = width * h_over_w;
}

void EdgeTile::Render() {
    QuadWrite   writer{ m_renderer };
    const auto& dx    = m_size.x;
    const auto& cur_y = m_start.y;
    for(auto cur_x = m_start.x; cur_x <= m_end.x; cur_x += dx) {
        
        writer.Render(&m_sprite_info, m_size, b2Vec2(cur_x, cur_y), b2Vec2_zero, 0.0f);
    }
}