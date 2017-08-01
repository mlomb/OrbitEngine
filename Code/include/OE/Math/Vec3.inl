#ifndef VEC3_INL
#define VEC3_INL

#include "OE/Math/Math.hpp"

#include "Vec3.hpp"

namespace OrbitEngine { namespace Math {
	template<typename T> inline Vec3<T>::Vec3(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) { }
	template<typename T> inline Vec3<T>::Vec3() : x(0), y(0), z(0) { }

	template<typename T> inline T& Vec3<T>::operator[](int i)
	{
		return data[i];
	}

	template<typename T> inline bool Vec3<T>::operator==(const Vec3& b) const
	{
		return x == b.x && y == b.y && z == b.z;
	}

	template<typename T> inline bool Vec3<T>::operator!=(const Vec3& b) const
	{
		return x != b.x || y != b.y || z != b.z;
	}

	template<typename T> inline Vec3<T>& Vec3<T>::operator+() const
	{
		return *this;
	}

	template<typename T> inline Vec3<T>& Vec3<T>::operator-() const
	{
		return Vec3<T>(-x, -y, -z);
	}

	template<typename T> inline Vec3<T>& Vec3<T>::operator+=(const Vec3& b)
	{
		this->x += b.x;
		this->y += b.y;
		this->z += b.z;
		return *this;
	}

	template<typename T> inline Vec3<T>& Vec3<T>::operator-=(const Vec3& b)
	{
		this->x -= b.x;
		this->y -= b.y;
		this->z -= b.z;
		return *this;
	}

	template<typename T> inline Vec3<T>& Vec3<T>::operator*=(const Vec3& b)
	{
		this->x *= b.x;
		this->y *= b.y;
		this->z *= b.z;
		return *this;
	}

	template<typename T> inline Vec3<T>& Vec3<T>::operator/=(const Vec3& b)
	{
		this->x /= b.x;
		this->y /= b.y;
		this->z /= b.z;
		return *this;
	}

	template<typename T> inline Vec3<T>& Vec3<T>::operator+=(const T& b)
	{
		this->x += b;
		this->y += b;
		this->z += b;
		return *this;
	}

	template<typename T> inline Vec3<T>& Vec3<T>::operator-=(const T& b)
	{
		this->x -= b;
		this->y -= b;
		this->z -= b;
		return *this;
	}

	template<typename T> inline Vec3<T>& Vec3<T>::operator*=(const T& b)
	{
		this->x *= b;
		this->y *= b;
		this->z *= b;
		return *this;
	}

	template<typename T> inline Vec3<T>& Vec3<T>::operator/=(const T& b)
	{
		this->x /= b;
		this->y /= b;
		this->z /= b;
		return *this;
	}
	
	template<typename T> inline Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3<T>(a.x + b.x,
					   a.y + b.y,
					   a.z + b.z);
	}

	template<typename T> inline Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3<T>(a.x - b.x,
					   a.y - b.y,
					   a.z - b.z);
	}

	template<typename T> inline Vec3<T> operator*(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3<T>(a.x * b.x,
					   a.y * b.y,
					   a.z * b.z);
	}

	template<typename T> inline Vec3<T> operator/(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3<T>(a.x / b.x,
					   a.y / b.y,
					   a.z / b.z);
	}

	template<typename T> inline Vec3<T> operator+(const Vec3<T>& a, const T& b)
	{
		return Vec3<T>(a.x + b,
					   a.y + b,
					   a.z + b);
	}

	template<typename T> inline Vec3<T> operator+(const T& a, const Vec3<T>& b)
	{
		return b + a;
	}

	template<typename T> inline Vec3<T> operator*(const Vec3<T>& a, const T& b)
	{
		return Vec3<T>(a.x * b,
					   a.y * b,
					   a.z * b);
	}

	template<typename T> inline Vec3<T> operator*(const T& a, const Vec3<T>& b)
	{
		return b * a;
	}

	template<typename T> inline Vec3<T> operator-(const Vec3<T>& a, const T& b)
	{
		return Vec3<T>(a.x - b,
					   a.y - b,
					   a.z - b);
	}

	template<typename T> inline Vec3<T> operator-(const T& a, const Vec3<T>& b)
	{
		return Vec3<T>(a - b.x,
					   a - b.y,
					   a - b.z);
	}

	template<typename T> inline Vec3<T> operator/(const Vec3<T>& a, const T& b)
	{
		return Vec3<T>(a.x / b,
					   a.y / b,
					   a.z / b);
	}

	template<typename T> inline Vec3<T> operator/(const T& a, const Vec3<T>& b)
	{
		return Vec3<T>(a / b.x,
					   a / b.y,
					   a / b.z);
	}

	// --

	template<typename T> inline Vec3<T> Vec3<T>::normalize()
	{
		float length = sqrt(x * x + y * y + z * z);
		if (length == 0) return *this;
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}

	template<typename T> inline Vec3<T> Vec3<T>::Min(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3<T>(std::min(a.x, b.x),
					   std::min(a.y, b.y),
					   std::min(a.z, b.z));
	}

	template<typename T> inline Vec3<T> Vec3<T>::Max(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3<T>(std::max(a.x, b.x),
					   std::max(a.y, b.y),
					   std::max(a.z, b.z));
	}

	template<typename T> inline Vec3<T> Vec3<T>::Lerp(const Vec3<T>& a, const Vec3<T>& b, T t)
	{
		return Vec3<T>(lerp(a.x, b.x, t),
					   lerp(a.y, b.y, t),
					   lerp(a.z, b.z, t));
	}

	template<typename T> inline Vec3<T> Vec3<T>::Normalize(const Vec3<T>& a)
	{
		Vec3<T> v(a);
		v.normalize();
		return v;
	}

	template<typename T> inline Vec3<T> Vec3<T>::Cross(const Vec3<T>& a, const Vec3<T>& b)
	{
		Vec3 r;
		r.x = (a.y * b.z) - (a.z * b.y);
		r.y = (a.z * b.x) - (a.x * b.z);
		r.z = (a.x * b.y) - (a.y * b.x);
		return r;
	}
} }

#endif