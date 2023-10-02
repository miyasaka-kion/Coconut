#include "Rect.h"



Coconut::Rect::Rect(std::tuple<int, int> pos_arr, std::tuple<int, int> size_arr) {
    std::tie(x, y) = pos_arr;
    std::tie(w, h) = size_arr;
}

std::tuple<int, int> Coconut::Rect::getPosition() {
    return std::tie(x, y);
}

std::tuple<int, int> Coconut::Rect::getSize() {
    return std::tie(w, h);
}
