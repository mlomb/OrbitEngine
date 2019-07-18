#ifndef MATH_VEC2_HPP
#define MATH_VEC2_HPP

namespace OrbitEngine { namespace Math {

	template<typename T>
	struct Vec2 {
	public:
		union {
			T data[2];
			struct { T x, y; };
			struct { T w, h; };
			struct { T r, g; };
		};

		Vec2();
		Vec2(const T& _x, const T& _y);
		template<typename U> Vec2(const Vec2<U> b);

		T& operator[](int i);

		bool operator==(const Vec2& b) const;
		bool operator!=(const Vec2& b) const;

		Vec2<T>& operator+() const;
		Vec2<T>& operator-() const;

		Vec2<T>& operator+=(const Vec2& b);
		Vec2<T>& operator-=(const Vec2& b);
		Vec2<T>& operator*=(const Vec2& b);
		Vec2<T>& operator/=(const Vec2& b);

		Vec2<T>& operator+=(const T& b);
		Vec2<T>& operator-=(const T& b);
		Vec2<T>& operator*=(const T& b);
		Vec2<T>& operator/=(const T& b);

		float normalize();
		float distanceTo(const Vec2<T>& b) const;

		static Vec2<T> Min(const Vec2<T>& a, const Vec2<T>& b);
		static Vec2<T> Max(const Vec2<T>& a, const Vec2<T>& b);
		static Vec2<T> Lerp(const Vec2<T>& a, const Vec2<T>& b, T t);
		static Vec2<T> Normalize(const Vec2<T>& a);
		static Vec2<T> FromAngle(double angle);
		static float Distance(const Vec2<T>& a, const Vec2<T>& b);
	};

	typedef Vec2<int>    Vec2i;
	typedef Vec2<float>  Vec2f;
	typedef Vec2<double> Vec2d;
} }

#include "OE/Math/Vec2.inl"

#endif