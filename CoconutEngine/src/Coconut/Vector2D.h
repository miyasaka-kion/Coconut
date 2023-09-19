#pragma once


#include <iostream>


namespace Coconut {
	class Vector2D {
	private:
		float x;
		float y;

	public:
		Vector2D();
		Vector2D(float x, float y);

	public:
		bool operator==(const Vector2D& vec) const;
		bool operator!=(const Vector2D& vec) const;

	private:
		bool cannotDiv() const ;
		
		Vector2D& add(const Vector2D& vec);
		Vector2D& mul(const Vector2D& vec);
		Vector2D& sub(const Vector2D& vec);
		Vector2D& div(const Vector2D& vec);

		friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
		friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
		friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
		friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

		Vector2D& operator+=(const Vector2D& vec);
		Vector2D& operator-=(const Vector2D& vec);
		Vector2D& operator*=(const Vector2D& vec);
		Vector2D& operator/=(const Vector2D& vec);

		//std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
	public: 
		void printValue() const;

	};
}