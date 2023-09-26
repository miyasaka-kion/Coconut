#include <cmath>

#include "Coconut/Log.h"
#include "Coconut/Vector2D.h"

Coconut::Vector2D::Vector2D() : x(0.0f), y(0.0f) {}

Coconut::Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

Coconut::Vector2D::Vector2D(std::tuple<float, float> tp) {
    std::tie(this->x, this->y) = tp;
}

std::tuple<float, float> Coconut::Vector2D::getCoordinate() {
    return std::make_tuple(x, y);
}

void Coconut::Vector2D::setX(float x) {
    this->x = x;
}

void Coconut::Vector2D::setY(float y) {
    this->y = y;
}

float Coconut::Vector2D::norm_2() {
    return std::sqrt(this->x * this->x + this->y * this->y);
}

bool Coconut::Vector2D::operator==(const Vector2D& vec) const {
    return (this->x == vec.x && this->y == vec.y);
}

bool Coconut::Vector2D::operator!=(const Vector2D& vec) const {
    return !(*this == vec);
}

bool Coconut::Vector2D::cannotDiv() const {
    return (this->x == 0.0f || this->y == 0.0f);
}

Coconut::Vector2D& Coconut::Vector2D::add(const Vector2D& vec) {
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Coconut::Vector2D& Coconut::Vector2D::sub(const Vector2D& vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Coconut::Vector2D& Coconut::Vector2D::mul(const Vector2D& vec) {
    this->x *= vec.x;
    this->y *= vec.y;
    return *this;
}

Coconut::Vector2D& Coconut::Vector2D::div(const Vector2D& vec) {
    if(vec.cannotDiv()) {
        CC_CORE_WARN("Division by zero ({}, {})! Return the dividend!", vec.getX(), vec.getY());
        return *this;
    }
    this->x /= vec.x;
    this->y /= vec.y;
    return *this;
}

Coconut::Vector2D Coconut::Vector2D::operator+(const Vector2D& vec) const {
    return Vector2D(this->x + vec.x, this->y + vec.y);
}

Coconut::Vector2D Coconut::Vector2D::operator-(const Vector2D& vec) const {
    return Vector2D(this->x - vec.x, this->y - vec.y);
}

Coconut::Vector2D Coconut::Vector2D::operator*(const Vector2D& vec) const {
    return Vector2D(this->x * vec.x, this->y * vec.y);
}

Coconut::Vector2D Coconut::Vector2D::operator/(const Vector2D& vec) const {
    if(vec.cannotDiv()) {
        CC_CORE_WARN("Dicision by zero! Return the illegal divisor~");
        return vec;
    }
    return Vector2D(this->x / vec.x, this->y / vec.y);
}

Coconut::Vector2D Coconut::Vector2D::operator*(const int& x) const {
    float fl = static_cast<float>(x);
    return *this * Vector2D(fl, fl);
}

Coconut::Vector2D Coconut::Vector2D::operator*(const float& x) const {
    return *this * Vector2D(x, x);
}

// += -= *= /=
Coconut::Vector2D& Coconut::Vector2D::operator+=(const Vector2D& vec) {
    this->add(vec);
    return *this;
}

Coconut::Vector2D& Coconut::Vector2D::operator-=(const Vector2D& vec) {
    this->sub(vec);
    return *this;
}

Coconut::Vector2D& Coconut::Vector2D::operator*=(const Vector2D& vec) {
    this->mul(vec);
    return *this;
}

Coconut::Vector2D& Coconut::Vector2D::operator/=(const Vector2D& vec) {
    this->div(vec);
    return *this;
}

Coconut::Vector2D& Coconut::Vector2D::operator/=(float scalar) {
    this->div(Vector2D(scalar, scalar));
    return *this;
}

void Coconut::Vector2D::printValue() const {
    CC_CORE_INFO("Current Vector2D pos: ({}, {})", x, y);
}

Coconut::Vector2D::RelativePosition_t Coconut::Vector2D::getRelativePosition(const Vector2D other) {
	if (other.x < x && other.y < y) return RelativePosition_t::lu;
	if (other.x > x && other.y < y) return RelativePosition_t::ld;
	if (other.x < x && other.y > y) return RelativePosition_t::ru;
	if (other.x > x && other.y > y) return RelativePosition_t::rd;
    return RelativePosition_t::same;
}

float Coconut::Vector2D::distance(const Coconut::Vector2D vecA, const Coconut::Vector2D vecB) {
    Coconut::Vector2D tmpvec{ vecA - vecB };
    return tmpvec.norm_2();
}

bool Coconut::Vector2D::operator<(const Coconut::Vector2D& other) {
    return (this->x < other.x && this->y < other.y);
}

bool Coconut::Vector2D::operator>(const Coconut::Vector2D& other) {
    return (this->x > other.x && this->y > other.y);
}