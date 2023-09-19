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

		inline float getX() const { return x; };
		inline float getY() const { return y; };
		std::tuple<float, float> getCoordinate();
		void setX(float x);
		void setY(float y);

	public:
		float norm();

	public:
		bool operator==(const Vector2D& vec) const;
		bool operator!=(const Vector2D& vec) const;

	private:
		bool cannotDiv() const ;
		Vector2D& add(const Vector2D& vec);
		Vector2D& mul(const Vector2D& vec);
		Vector2D& sub(const Vector2D& vec);
		Vector2D& div(const Vector2D& vec);


	public:
		Vector2D operator+(const Vector2D& vec) const;
		Vector2D operator-(const Vector2D& vec) const;
		Vector2D operator*(const Vector2D& vec) const;
		Vector2D operator/(const Vector2D& vec) const;

	// scalar operand
	public:
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

		//std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
	
	public: 
		// debug function
		void printValue() const;

	};
}