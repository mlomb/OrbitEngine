#ifndef VEC2_INL
#define VEC2_INL

#include "OE/Math/Math.hpp"

#include "Vec2.hpp"

namespace OrbitEngine { namespace Math {
	template<typename T> inline Vec2<T>::Vec2(const T& _x, const T& _y) : x(_x), y(_y) { }
	template<typename T> template<typename U> inline Vec2<T>::Vec2(const Vec2<U> b) : x((T)b.x), y((T)b.y) { }
	template<typename T> inline Vec2<T>::Vec2() : x(0), y(0) { }

	template<typename T> inline T& Vec2<T>::operator[](int i)
	{
		return data[i];
	}

	template<typename T> inline bool Vec2<T>::operator==(const Vec2& b) const
	{
		return x == b.x && y == b.y;
	}

	template<typename T> inline bool Vec2<T>::operator!=(const Vec2& b) const
	{
		return x != b.x || y != b.y;
	}

	template<typename T> inline Vec2<T>& Vec2<T>::operator+() const
	{
		return *this;
	}

	template<typename T> inline Vec2<T>& Vec2<T>::operator-() const
	{
		return Vec2<T>(-x, -y);
	}

	template<typename T> inline Vec2<T>& Vec2<T>::operator+=(const Vec2& b)
	{
		this->x += b.x;
		this->y += b.y;
		return *this;
	}

	template<typename T> inline Vec2<T>& Vec2<T>::operator-=(const Vec2& b)
	{
		this->x -= b.x;
		this->y -= b.y;
		return *this;
	}

	template<typename T> inline Vec2<T>& Vec2<T>::operator*=(const Vec2& b)
	{
		this->x *= b.x;
		this->y *= b.y;
		return *this;
	}

	template<typename T> inline Vec2<T>& Vec2<T>::operator/=(const Vec2& b)
	{
		this->x /= b.x;
		this->y /= b.y;
		return *this;
	}

	template<typename T> inline Vec2<T>& Vec2<T>::operator+=(const T& b)
	{
		this->x += b;
		this->y += b;
		return *this;
	}

	template<typename T> inline Vec2<T>& Vec2<T>::operator-=(const T& b)
	{
		this->x -= b;
		this->y -= b;
		return *this;
	}

	template<typename T> inline Vec2<T>& Vec2<T>::operator*=(const T& b)
	{
		this->x *= b;
		this->y *= b;
		return *this;
	}

	template<typename T> inline Vec2<T>& Vec2<T>::operator/=(const T& b)
	{
		this->x /= b;
		this->y /= b;
		return *this;
	}
	
	template<typename T> inline Vec2<T> operator+(const Vec2<T>& a, const Vec2<T>& b)
	{
		return Vec2<T>((T)(a.x + b.x),
					   (T)(a.y + b.y));
	}

	template<typename T, typename U> inline Vec2<T> operator-(const Vec2<T>& a, const Vec2<U>& b)
	{
		return Vec2<T>((T)(a.x - b.x),
					   (T)(a.y - b.y));
	}

	template<typename T> inline Vec2<T> operator*(const Vec2<T>& a, const Vec2<T>& b)
	{
		return Vec2<T>((T)(a.x * b.x),
					   (T)(a.y * b.y));
	}

	template<typename T> inline Vec2<T> operator/(const Vec2<T>& a, const Vec2<T>& b)
	{
		return Vec2<T>((T)(a.x / b.x),
					   (T)(a.y / b.y));
	}

	template<typename T> inline Vec2<T> operator+(const Vec2<T>& a, const T& b)
	{
		return Vec2<T>((T)(a.x + b),
					   (T)(a.y + b));
	}

	template<typename T> inline Vec2<T> operator+(const T& a, const Vec2<T>& b)
	{
		return b + a;
	}

	template<typename T> inline Vec2<T> operator*(const Vec2<T>& a, const T& b)
	{
		return Vec2<T>((T)(a.x * b),
					   (T)(a.y * b));
	}

	template<typename T> inline Vec2<T> operator*(const T& a, const Vec2<T>& b)
	{
		return b * a;
	}

	template<typename T> inline Vec2<T> operator-(const Vec2<T>& a, const T& b)
	{
		return Vec2<T>((T)(a.x - b),
					   (T)(a.y - b));
	}

	template<typename T> inline Vec2<T> operator-(const T& a, const Vec2<T>& b)
	{
		return Vec2<T>((T)(a - b.x),
					   (T)(a - b.y));
	}

	template<typename T> inline Vec2<T> operator/(const Vec2<T>& a, const T& b)
	{
		return Vec2<T>((T)(a.x / b),
					   (T)(a.y / b));
	}

	template<typename T> inline Vec2<T> operator/(const T& a, const Vec2<T>& b)
	{
		return Vec2<T>((T)(a / b.x),
					   (T)(a / b.y));
	}

	// --

	template<typename T> inline float Vec2<T>::normalize()
	{
		float length = sqrt(x * x + y * y);
		if (length == 0) return 0;
		x /= length;
		y /= length;
		return length;
	}

	template<typename T> inline Vec2<T> Vec2<T>::Min(const Vec2<T>& a, const Vec2<T>& b)
	{
		return Vec2<T>(std::min(a.x, b.x),
					   std::min(a.y, b.y));
	}

	template<typename T> inline Vec2<T> Vec2<T>::Max(const Vec2<T>& a, const Vec2<T>& b)
	{
		return Vec2<T>(std::max(a.x, b.x),
					   std::max(a.y, b.y));
	}

	template<typename T> inline Vec2<T> Vec2<T>::Lerp(const Vec2<T>& a, const Vec2<T>& b, T t)
	{
		return Vec2<T>(lerp(a.x, b.x, t),
					   lerp(a.y, b.y, t));
	}

	template<typename T> inline Vec2<T> Vec2<T>::Normalize(const Vec2<T>& a)
	{
		Vec2<T> v(a);
		v.normalize();
		return v;
	}

	template<typename T> inline Vec2<T> Vec2<T>::FromAngle(double angle)
	{
		return Vec2<T>(cos(angle), -sin(angle));
	}
} }

#endif