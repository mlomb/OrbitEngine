#ifndef MATH_HPP
#define MATH_HPP

#include <algorithm>
#include <cmath>
#include <math.h>

/// PI value
#define PI 3.14159265358979323846f
/// TAU value (2 * PI)
#define TAU 6.28318530717958647692f
/// @brief KAPPA90 = (4/3 * (sqrt(2) - 1))
/// @see https://de.wikipedia.org/wiki/Datei:Circle-cbezier-method2.svg
#define KAPPA90 0.5522847493f


/// Max float value
#define OE_FLOAT_INFINITY std::numeric_limits<float>::infinity()

/// Count the number of elements in a C array
#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(0[arr]))

namespace OrbitEngine { namespace Math {

	/// Convert degrees to radians
	inline double deg2rad(double deg)
	{
		return deg * PI / 180.f;
	}

	/// Convert radians to degrees
	inline double rad2deg(double rad)
	{
		return rad * 180.f / PI;
	}
	
	/// Get the next power of two
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

	/// Check if a number is a power of two
	inline bool isPowerOfTwo(unsigned int x) {
		return x == 0 || (x & (x - 1)) == 0;
	}

	/// Check if two values are 'equal' by at most \p epsilon
	template<typename T>
	inline T approximatelyEqual(T a, T b, T epsilon)
	{
		return std::abs(a - b) <= ((std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
	}

	/// Linearly interpolates two elements
	template<typename T>
	inline T lerp(T v0, T v1, float t) {
		return v0 + (v1 - v0) * t;
	}

	/// Constrain a value between two others
	template<typename T>
	inline T clamp(T input, T min_, T max_) {
		return (std::max)(min_, (std::min)(input, max_));
	}

	/// Extract the sign of the parameter
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