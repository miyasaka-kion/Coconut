#pragma once
#include <box2d/box2d.h>

class MetricConverter {
private:
    static constexpr int PixelsPerMeter = 100;  // pix/meter, his is the scaling factor

    static constexpr int WIDTH  = 1000;  // pix, Screen width
    static constexpr int HEIGHT = 700;   // pix, Screen height

    static constexpr int SCALED_WIDTH  = WIDTH / PixelsPerMeter;  // screen width in meter
    static constexpr int SCALED_HEIGHT = HEIGHT / PixelsPerMeter;

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

};