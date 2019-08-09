#ifndef MATH_UV_HPP
#define MATH_UV_HPP

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace Math {
	/// Describes the uv coordinates of a single quad
	struct UV {
		UV() = default;
		UV(const float u0, const float v0, const float u1, const float v1)
			: _1(Vec2f(u0, v0)), _2(Vec2f(u0, v1)), _3(Vec2f(u1, v1)), _4(Vec2f(u1, v0)) { };
		UV(const Vec2f& _1, const Vec2f& _2, const Vec2f& _3, const Vec2f& _4)
			: _1(_1), _2(_2), _3(_3), _4(_4) { };

		Vec2f _1, _2, _3, _4;

		/// Get UV coordinates by index
		Vec2f getUV(int pos) const {
			pos %= 4;
			switch (pos)
			{
			case 0:
				return _1;
			case 1:
				return _2;
			case 2:
				return _3;
			case 3:
				return _4;
			}
			return Math::Vec2f();
		}

	};

	/// Default UVs for 2D quads
	static const UV UV_Default = UV(
		Math::Vec2f(0, 1),
		Math::Vec2f(0, 0),
		Math::Vec2f(1, 0),
		Math::Vec2f(1, 1)
	);
} }

#endif