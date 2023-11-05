#include "Rect.h"

#include <Coconut/MetricConverter.h>

Coconut::Rect::Rect(std::tuple<int, int> pos_arr, std::tuple<int, int> size_arr) {
    std::tie(x, y) = pos_arr;
    std::tie(w, h) = size_arr;
}

Coconut::Rect::Rect(b2Vec2 pos_meter, b2Vec2 size_meter) {
    x = MetricConverter::toPixX(pos_meter.x);
    y = MetricConverter::toPixY(pos_meter.y);
    
    w = MetricConverter::toPix(size_meter.x);
    h = MetricConverter::toPix(size_meter.y);
}

