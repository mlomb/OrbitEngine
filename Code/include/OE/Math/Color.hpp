#ifndef MATH_COLOR_HPP
#define MATH_COLOR_HPP

#include "OE/Math/Vec4.hpp"

namespace OrbitEngine { namespace Math {
	typedef Vec4f Color;

	// h ∈ [0, 360]
	// s ∈ [0, 1]
	// l ∈ [0, 1]
	Color FromHSL(int h, float s, float l);
} }

#endif