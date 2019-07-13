#ifndef MATH_AABB_HPP
#define MATH_AABB_HPP

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace Math {
	struct Bounds {
		float left, right, top, bottom;
	};

	struct AABB {
	public:
		float x, y;
		float hw, hh; // half sizes

		Bounds bounds() const {
			return {
				x - hw,
				x + hw,
				y - hh,
				y + hh,
			};
		}

		bool contains(const Vec2f& p) const {
			const Bounds& bounds_ = bounds();
			return bounds_.top <= p.y  && p.y <= bounds_.bottom &&
				   bounds_.left <= p.x && p.x <= bounds_.right;
		}

		bool intersects(const AABB& other) const {
			bool xx = std::abs(x - other.x) <= (hw + other.hw);
			bool yy = std::abs(y - other.y) <= (hh + other.hh);

			return xx && yy;
		}
	};
} }

#endif