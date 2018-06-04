#ifndef MATH_HPP
#define MATH_HPP

#include <algorithm>
#include <cmath>
#include <math.h>

#define PI 3.14159265358979323846f
#define TAU 6.28318530717958647692f
#define KAPPA90 0.5522847493f

// KAPPA90 = (4/3 * (sqrt(2) - 1))
// https://de.wikipedia.org/wiki/Datei:Circle-cbezier-method2.svg

#define OE_FLOAT_INFINITY std::numeric_limits<float>::infinity()

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(0[arr]))

namespace OrbitEngine { namespace Math {

	inline double deg2rad(double deg)
	{
		return deg * PI / 180.f;
	}

	inline double rad2deg(double rad)
	{
		return rad * 180.f / PI;
	}
	
	inline unsigned int nextPowerOfTwo(unsigned int x)
	{
		x--;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		x++;
		return x;
	}

	template<typename T>
	inline T approximatelyEqual(T a, T b, T epsilon)
	{
		return std::abs(a - b) <= ((std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
	}

	template<typename T>
	inline T lerp(T v0, T v1, float t) {
		return (1.f - t) * v0 + t * v1;
	}

	template<typename T>
	inline T clamp(T input, T min_, T max_) {
		return (std::max)(min_, (std::min)(input, max_));
	}

	template<typename T>
	inline T sign(T input)
	{
		return input >= (T)0 ? (T)1 : (T)-1;
	};

	template<typename A, typename B>
	A castMinMax(B in)
	{
		// only integer types

		A retVal = 0;

		if (in > 0)
			retVal = static_cast<A>(in & std::numeric_limits<A>::max());
		else if (in < 0)
			retVal = static_cast<A>(in | std::numeric_limits<A>::min());

		return retVal;
	}
} }

#endif