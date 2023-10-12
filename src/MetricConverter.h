#pragma once
#include <tuple>

#include <box2d/box2d.h>

class MetricConverter {
private:
    static constexpr int PixelsPerMeter = 100;  // pix/meter, his is the scaling factor

    static constexpr int WIDTH  = 1000;  // pix, Screen width
    static constexpr int HEIGHT = 700;   // pix, Screen height

    static constexpr float SCREEN_WIDTH_IN_METER  = static_cast<float>(WIDTH) / PixelsPerMeter;  // screen width in meter
    static constexpr float SCREEN_HEIGHT_IN_METER = static_cast<float>(HEIGHT) / PixelsPerMeter;

    // 1 rad × 180/π = 57,296°
    static constexpr float DegreePerRad = 180 / M_PI;

public:
    /// @param mt the value (meter) which need to convert to pixel
    static int toPix(float mt) {
        return mt * PixelsPerMeter;
    }
    static float toMeter(int px) {
        return static_cast<int>(px) / static_cast<float>(PixelsPerMeter);
    }

    static float toDegree(float angleInRad) {
        return angleInRad * DegreePerRad;
    }

    static float toRad(float angleInDegree) {
        return angleInDegree / DegreePerRad;
    }

    /// @param x position x in meter, change of axis included
    static int toPixX(float x) {
        return toPix( SCREEN_WIDTH_IN_METER / 2.0f + x);
    }

    /// @param y position y in mete, change of axis includedr
    static int toPixY(float y) {
        return toPix(SCREEN_HEIGHT_IN_METER / 2.0f - y);
    }

    static std::tuple<int, int> toPixCoordinate(b2Vec2 pos) {
        return std::make_tuple(
            toPixX(pos.x),
            toPixY(pos.y)
        );
    }

    static std::tuple<int, int> toPixCoordinate(float x, float y) {
        b2Vec2 pos{x, y};
        return std::make_tuple(
            toPixX(pos.x),
            toPixY(pos.y)
        );
    }

};