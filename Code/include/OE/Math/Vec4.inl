#ifndef VEC4_INL
#define VEC4_INL

#include "OE/Math/Math.hpp"

#include "Vec4.hpp"

namespace OrbitEngine { namespace Math {
	template<typename T> inline Vec4<T>::Vec4(const T& _x, const T& _y, const T& _z, const T& _w) : x(_x), y(_y), z(_z), w(_w) { }
	template<typename T> inline Vec4<T>::Vec4(const Vec2<T> _xy, const Vec2<T> _zw) : xy(_xy), zw(_zw) { }
	template<typename T> inline Vec4<T>::Vec4() : Vec4(0, 0, 0, 0) { }

	template<typename T> inline T& Vec4<T>::operator[](int i)
	{
		return data[i];
	}

	template<typename T> template<typename U> inline bool Vec4<T>::operator==(const Vec4<U> &b) const
	{
		return x == b.x && y == b.y && z == b.z && w == b.w;
	}

	template<typename T> template<typename U> inline bool Vec4<T>::operator!=(const Vec4<U> &b) const
	{
		return x != b.x || y != b.y || z != b.z || w != b.w;
	}

	template<typename T> inline Vec4<T>& Vec4<T>::operator+() const
	{
		return *this;
	}

	template<typename T> inline Vec4<T>& Vec4<T>::operator-() const
	{
		return Vec4<T>(-x, -y, -z, -w);
	}

	template<typename T> inline Vec4<T>& Vec4<T>::operator+=(const Vec4& b)
	{
		this->x += b.x;
		this->y += b.y;
		this->z += b.z;
		this->w += b.w;
		return *this;
	}

	template<typename T> inline Vec4<T>& Vec4<T>::operator-=(const Vec4& b)
	{
		this->x -= b.x;
		this->y -= b.y;
		this->z -= b.z;
		this->w -= b.w;
		return *this;
	}

	template<typename T> inline Vec4<T>& Vec4<T>::operator*=(const Vec4& b)
	{
		this->x *= b.x;
		this->y *= b.y;
		this->z *= b.z;
		this->w *= b.w;
		return *this;
	}

	template<typename T> inline Vec4<T>& Vec4<T>::operator/=(const Vec4& b)
	{
		this->x /= b.x;
		this->y /= b.y;
		this->z /= b.z;
		this->w /= b.w;
		return *this;
	}

	template<typename T> inline Vec4<T>& Vec4<T>::operator+=(const T& b)
	{
		this->x += b;
		this->y += b;
		this->z += b;
		this->w += b;
		return *this;
	}

	template<typename T> inline Vec4<T>& Vec4<T>::operator-=(const T& b)
	{
		this->x -= b;
		this->y -= b;
		this->z -= b;
		this->w -= b;
		return *this;
	}

	template<typename T> inline Vec4<T>& Vec4<T>::operator*=(const T& b)
	{
		this->x *= b;
		this->y *= b;
		this->z *= b;
		this->w *= b;
		return *this;
	}

	template<typename T> inline Vec4<T>& Vec4<T>::operator/=(const T& b)
	{
		this->x /= b;
		this->y /= b;
		this->z /= b;
		this->w /= b;
		return *this;
	}
	
	template<typename T> inline Vec4<T> operator+(const Vec4<T>& a, const Vec4<T>& b)
	{
		return Vec4<T>(a.x + b.x,
					   a.y + b.y,
					   a.z + b.z,
					   a.w + b.w);
	}

	template<typename T> inline Vec4<T> operator-(const Vec4<T>& a, const Vec4<T>& b)
	{
		return Vec4<T>(a.x - b.x,
					   a.y - b.y,
					   a.z - b.z,
					   a.w - b.w);
	}

	template<typename T> inline Vec4<T> operator*(const Vec4<T>& a, const Vec4<T>& b)
	{
		return Vec4<T>(a.x * b.x,
					   a.y * b.y,
					   a.z * b.z,
					   a.w * b.w);
	}

	template<typename T> inline Vec4<T> operator/(const Vec4<T>& a, const Vec4<T>& b)
	{
		return Vec4<T>(a.x / b.x,
					   a.y / b.y,
					   a.z / b.z,
					   a.w / b.w);
	}

	template<typename T> inline Vec4<T> operator+(const Vec4<T>& a, const T& b)
	{
		return Vec4<T>(a.x + b,
					   a.y + b,
					   a.z + b,
					   a.w + b);
	}

	template<typename T> inline Vec4<T> operator+(const T& a, const Vec4<T>& b)
	{
		return b + a;
	}

	template<typename T> inline Vec4<T> operator*(const Vec4<T>& a, const T& b)
	{
		return Vec4<T>(a.x * b,
					   a.y * b,
					   a.z * b,
					   a.w * b);
	}

	template<typename T> inline Vec4<T> operator*(const T& a, const Vec4<T>& b)
	{
		return b * a;
	}

	template<typename T> inline Vec4<T> operator-(const Vec4<T>& a, const T& b)
	{
		return Vec4<T>(a.x - b,
					   a.y - b,
					   a.z - b,
					   a.w - b);
	}

	template<typename T> inline Vec4<T> operator-(const T& a, const Vec4<T>& b)
	{
		return Vec4<T>(a - b.x,
					   a - b.y,
					   a - b.z,
					   a - b.w);
	}

	template<typename T> inline Vec4<T> operator/(const Vec4<T>& a, const T& b)
	{
		return Vec4<T>(a.x / b,
					   a.y / b,
					   a.z / b,
					   a.w / b);
	}

	template<typename T> inline Vec4<T> operator/(const T& a, const Vec4<T>& b)
	{
		return Vec4<T>(a / b.x,
					   a / b.y,
					   a / b.z,
					   a / b.w);
	}

	// --

	template<typename T> inline void Vec4<T>::normalize()
	{
		float length = sqrt(x * x + y * y + z * z + w * w);
		if (length == 0) return;
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	template<typename T> template<typename U> inline bool Vec4<T>::isInside(Vec2<U> point)
	{
		return point.x >= x &&
			point.x <= x + z &&
			point.y >= y &&
			point.y <= y + w;
	}

	template<typename T> inline Vec2<T> Vec4<T>::xz()
	{
		return Vec2<T>(x, z);
	}

	template<typename T> inline Vec2<T> Vec4<T>::yw()
	{
		return Vec2<T>(y, w);
	}

	template<typename T> inline Vec4<T> Vec4<T>::Min(const Vec4<T>& a, const Vec4<T>& b)
	{
		return Vec4<T>(std::min(a.x, b.x),
					   std::min(a.y, b.y),
					   std::min(a.z, b.z),
					   std::min(a.w, b.w));
	}

	template<typename T> inline Vec4<T> Vec4<T>::Max(const Vec4<T>& a, const Vec4<T>& b)
	{
		return Vec4<T>(std::max(a.x, b.x),
					   std::max(a.y, b.y),
					   std::max(a.z, b.z),
					   std::max(a.w, b.w));
	}

	template<typename T> inline Vec4<T> Vec4<T>::Lerp(const Vec4<T>& a, const Vec4<T>& b, T t)
	{
		return Vec4<T>(lerp(a.x, b.x, t),
					   lerp(a.y, b.y, t),
					   lerp(a.z, b.z, t),
					   lerp(a.w, b.w, t));
	}

	template<typename T> inline Vec4<T> Vec4<T>::Normalize(const Vec4<T>& a)
	{
		Vec4<T> v(a);
		v.normalize();
		return v;
	}
} }

#endif