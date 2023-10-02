#pragma once

#include <iostream>

namespace Coconut {

class Vector2D {
public:
    float x;
    float y;

public:
    Vector2D();
    Vector2D(float x, float y);
    Vector2D(std::tuple<float, float> tp);

    // may be remove in the funture
    inline float getX() const {
        return x;
    };
    inline float getY() const {
        return y;
    };

    void setX(float x);
    void setY(float y);

    std::tuple<float, float> getCoordinate();

    enum class RelativePosition_t {
        same = 0,
        // left up
        lu,
        // left down
        ld,
        // right up
        ru,
        // right down
        rd
    };

    RelativePosition_t getRelativePosition(const Vector2D other);

    // 2 norm of the vector
    float norm_2();
    // get the Euclidean distance of two Vector2D
    static float distance(const Vector2D vecA, const Vector2D vecB);

public:
    bool operator==(const Vector2D& vec) const;
    bool operator!=(const Vector2D& vec) const;

private:
    bool      cannotDiv() const;
    Vector2D& add(const Vector2D& vec);
    Vector2D& mul(const Vector2D& vec);
    Vector2D& sub(const Vector2D& vec);
    Vector2D& div(const Vector2D& vec);

public:
    Vector2D operator+(const Vector2D& vec) const;
    Vector2D operator-(const Vector2D& vec) const;
    Vector2D operator*(const Vector2D& vec) const;
    Vector2D operator/(const Vector2D& vec) const;

public:
    // scalar operands
    Vector2D operator*(const int& x) const;
    Vector2D operator*(const float& x) const;

public:
    Vector2D& operator+=(const Vector2D& vec);
    Vector2D& operator-=(const Vector2D& vec);
    Vector2D& operator*=(const Vector2D& vec);
    Vector2D& operator/=(const Vector2D& vec);

public:
    // ToDo: the choose of pass by value or pass by reference should be unified in this class
    Vector2D& operator/=(float scalar);

    bool operator<(const Vector2D& vec);
    bool operator>(const Vector2D& vec);

public:
    // debug function
    void printValue() const;
};
}  // namespace Coconut