#ifndef MATH_VEC4_HPP
#define MATH_VEC4_HPP

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace Math {
	template<typename T>
	struct Vec4 {
	public:
		union {
			float data[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct {
				Vec2<T> xy;
				Vec2<T> zw;
			};
		};

		Vec4(const T& _x, const T& _y, const T& _z, const T& _w);
		Vec4(const Vec2<T> _xy, const Vec2<T> _zw);
		Vec4();

		T& operator[] (int i);

		template<typename U>
		bool operator==(const Vec4<U>& b) const;
		template<typename U>
		bool operator!=(const Vec4<U> &b) const;

		Vec4<T>& operator+() const;
		Vec4<T>& operator-() const;
		
		Vec4<T>& operator+=(const Vec4& b);
		Vec4<T>& operator-=(const Vec4& b);
		Vec4<T>& operator*=(const Vec4& b);
		Vec4<T>& operator/=(const Vec4& b);
		
		Vec4<T>& operator+=(const T& b);
		Vec4<T>& operator-=(const T& b);
		Vec4<T>& operator*=(const T& b);
		Vec4<T>& operator/=(const T& b);

		void normalize();
		template<typename U>
		bool isInside(Vec2<U> point);

		Vec2<T> xz();
		Vec2<T> yw();

		static Vec4<T> Min(const Vec4<T>& a, const Vec4<T>& b);
		static Vec4<T> Max(const Vec4<T>& a, const Vec4<T>& b);
		static Vec4<T> Lerp(const Vec4<T>& a, const Vec4<T>& b, T t);
		static Vec4<T> Normalize(const Vec4<T>& a);
	};

	typedef Vec4<int>    Vec4i;
	typedef Vec4<float>  Vec4f;
	typedef Vec4<double> Vec4d;
} }

#include "OE/Math/Vec4.inl"

#endif