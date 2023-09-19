#include "Coconut/Vector2D.h"
#include "Coconut/Log.h"

Coconut::Vector2D::Vector2D() : x(0.0f), y(0.0f) {}

Coconut::Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

std::tuple<float, float> Coconut::Vector2D::getCoordinate()
{
	return std::make_tuple(x, y);
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
		CC_CORE_WARN("Division by zero!");
		this->x = 0.0f;
		this->y = 0.0f;
		return *this;
	}
	this->x /= vec.x;
	this->y /= vec.y;
	return *this;
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

//std::ostream& Coconut::Vector2D::operator<<(std::ostream& stream, const Coconut::Vector2D& vec) {
//	stream << '(' << vec.x << ',' << ' ' << vec.y;
//	return stream;
//}



// + - * /
Coconut::Vector2D& Coconut::operator+(Vector2D& v1, const Vector2D& v2) {
	return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

Coconut::Vector2D& Coconut::operator-(Vector2D& v1, const Vector2D& v2) {
	return Vector2D(v1.x - v2.x, v1.y - v2.y);
}

Coconut::Vector2D& Coconut::operator*(Vector2D& v1, const Vector2D& v2) {
	return Vector2D(v1.x * v2.x, v1.y * v2.y);
}

Coconut::Vector2D& Coconut::operator/(Vector2D& v1, const Vector2D& v2) {
	if (v2.cannotDiv()) {
		CC_CORE_WARN("Division by zero!");
		return v1;
	}
	return Vector2D(v1.x / v2.x, v1.y / v2.y);
}


void Coconut::Vector2D::printValue() const {
	CC_CORE_INFO("Current Vector2D pos: ({}, {})", x, y);
}