#pragma once

#include <tuple>

#include <box2d/box2d.h>

namespace Coconut {
class Rect {
public:
    int x, y;
    int w, h;

    Rect();
    Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    Rect(std::tuple<int, int> pos_px, std::tuple<int, int> size_px);
    Rect(b2Vec2 pos_meter, b2Vec2 size_meter);

    Rect operator=(const Rect& other) {
        return Rect(x, y, w, h);
    }
};
}  // namespace Coconut