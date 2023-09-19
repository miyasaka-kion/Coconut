#include <cmath>

#include "Coconut/Vector2D.h"
#include "Coconut/Log.h"

Coconut::Vector2D::Vector2D() : x(0.0f), y(0.0f) {}

Coconut::Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

std::tuple<float, float> Coconut::Vector2D::getCoordinate()
{
	return std::make_tuple(x, y);
}

void Coconut::Vector2D::setX(float x) {
	this->x = x;
}

void Coconut::Vector2D::setY(float y) {
	this->y = y;
}

float Coconut::Vector2D::norm()
{
	return std::sqrt(this->x * this->x + this->y * this->y);
}

bool Coconut::Vector2D::operator==(const Vector2D& vec) const
{
	return (this->x == vec.x && this->y == vec.y);
}

bool Coconut::Vector2D::operator!=(const Vector2D& vec) const
{
	return !(*this == vec);
}

bool Coconut::Vector2D::cannotDiv() const 
{
	return (this->x == 0.0f || this->y == 0.0f);
}



Coconut::Vector2D& Coconut::Vector2D::add(const Vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}

Coconut::Vector2D& Coconut::Vector2D::sub(const Vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}

Coconut::Vector2D& Coconut::Vector2D::mul(const Vector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	return *this;
}

Coconut::Vector2D& Coconut::Vector2D::div(const Vector2D& vec)
{
	if (vec.cannotDiv()) {
		CC_CORE_WARN("Division by zero ({}, {})! Return the dividend!", vec.getX(), vec.getY());
		return *this;
	}
	this->x /= vec.x;
	this->y /= vec.y;
	return *this;
}

Coconut::Vector2D Coconut::Vector2D::operator+(const Vector2D& vec) const 
{
	return Vector2D(this->x + vec.x, this->y + vec.y);
}

Coconut::Vector2D Coconut::Vector2D::operator-(const Vector2D& vec) const 
{
	return Vector2D(this->x - vec.x, this->y - vec.y);
}

Coconut::Vector2D Coconut::Vector2D::operator*(const Vector2D& vec) const 
{
	return Vector2D(this->x * vec.x, this->y * vec.y);
}

Coconut::Vector2D Coconut::Vector2D::operator/(const Vector2D& vec) const
{
	if (vec.cannotDiv()) {
		CC_CORE_WARN("Dicision by zero! Return the illegal divisor~");
		return vec;
	}
	return Vector2D(this->x / vec.x, this->y / vec.y);
}

Coconut::Vector2D Coconut::Vector2D::operator*(const int& x) const
{
	float fl = static_cast<float>(x);
	return *this * Vector2D(fl, fl);
}

Coconut::Vector2D Coconut::Vector2D::operator*(const float& x) const
{
	return *this * Vector2D(x, x);
}




// += -= *= /=
Coconut::Vector2D& Coconut::Vector2D::operator+=(const Vector2D& vec)
{
	this->add(vec);
	return *this;
}

Coconut::Vector2D& Coconut::Vector2D::operator-=(const Vector2D& vec)
{
	this->sub(vec);
	return *this;
}

Coconut::Vector2D& Coconut::Vector2D::operator*=(const Vector2D& vec)
{
	this->mul(vec);
	return *this;
}

Coconut::Vector2D& Coconut::Vector2D::operator/=(const Vector2D& vec)
{
	this->div(vec);
	return *this;
}

Coconut::Vector2D& Coconut::Vector2D::operator/=(float scalar) {
	this->div(Vector2D(scalar, scalar));
	return *this;
}

//std::ostream& Coconut::Vector2D::operator<<(std::ostream& stream, const Coconut::Vector2D& vec) {
//	stream << '(' << vec.x << ',' << ' ' << vec.y;
//	return stream;
//}




void Coconut::Vector2D::printValue() const {
	CC_CORE_INFO("Current Vector2D pos: ({}, {})", x, y);
}

