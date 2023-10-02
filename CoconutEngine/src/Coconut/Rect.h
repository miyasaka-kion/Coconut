#pragma once

#include <tuple>

#include <Coconut/Vector2D.h>
namespace Coconut {

class Rect {
public:
    int x, y;
    int w, h;

    Rect();
    Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    Rect(std::tuple<int, int> pos_arr, std::tuple<int, int> size_arr);
    Rect operator=(const Rect& other) {
        return Rect(x, y, w, h);
    }

    // Good bindings to get Rect info
    std::tuple<int, int> getPosition();
    std::tuple<int, int> getSize();

    // When treating Rect as a circle:
    Coconut::Vector2D getCentral();
    float             getRadius();
    std::tuple<Coconut::Vector2D, float> getCircleInfo();
};
}  // namespace Coconut