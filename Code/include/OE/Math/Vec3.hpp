#ifndef MATH_VEC3_H
#define MATH_VEC3_H

namespace OrbitEngine { namespace Math {

	template<typename T>
	struct Vec3 {
	public:
		union {
			T data[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
		};

		explicit Vec3(const T& _x, const T& _y, const T& _z);
		Vec3();

		T& operator[] (int i);

		bool operator==(const Vec3& b) const;
		bool operator!=(const Vec3& b) const;

		Vec3<T>& operator+() const;
		Vec3<T>& operator-() const;
		
		Vec3<T>& operator+=(const Vec3& b);
		Vec3<T>& operator-=(const Vec3& b);
		Vec3<T>& operator*=(const Vec3& b);
		Vec3<T>& operator/=(const Vec3& b);
		
		Vec3<T>& operator+=(const T& b);
		Vec3<T>& operator-=(const T& b);
		Vec3<T>& operator*=(const T& b);
		Vec3<T>& operator/=(const T& b);

		Vec3<T> normalize();

		inline static Vec3<T> Up() { return Vec3<T>(0, 1, 0); }
		inline static Vec3<T> Down() { return Vec3<T>(0, -1, 0); }
		inline static Vec3<T> Left() { return Vec3<T>(-1, 0, 0); }
		inline static Vec3<T> Right() { return Vec3<T>(1, 0, 0); }
		inline static Vec3<T> Forward() { return Vec3<T>(0, 0, 1); }
		inline static Vec3<T> Back() { return Vec3<T>(0, 0, -1); }

		static Vec3<T> Min(const Vec3<T>& a, const Vec3<T>& b);
		static Vec3<T> Max(const Vec3<T>& a, const Vec3<T>& b);
		static Vec3<T> Lerp(const Vec3<T>& a, const Vec3<T>& b, T t);
		static Vec3<T> Normalize(const Vec3<T>& a);
		static Vec3<T> Cross(const Vec3<T>& a, const Vec3<T>& b);
	};

	typedef Vec3<int>    Vec3i;
	typedef Vec3<float>  Vec3f;
	typedef Vec3<double> Vec3d;
} }

#include "OE/Math/Vec3.inl"

#endif