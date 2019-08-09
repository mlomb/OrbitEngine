#ifndef MATH_AABB_HPP
#define MATH_AABB_HPP

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace Math {
	/// 2D AABB boundaries
	template<typename T>
	struct Bounds {
		T left, right, top, bottom;
	};

	/**
		@brief Axis-aligned bounding box
		@note Half-size based
	*/
	template<typename T>
	struct AABB {
		AABB()
			: x(0), y(0), hw(0), hh(0) { }
		AABB(T x, T y, T halfWidth, T halfHeight)
			: x(x), y(y), hw(halfWidth), hh(halfHeight) { }
		AABB(const Vec2<T>& position, const Vec2<T>& halfSize)
			: x(position.x), y(position.y), hw(halfSize.w), hh(halfSize.h) { }

		/// X position
		T x;
		/// Y position
		T y;
		/// Half width
		T hw;
		/// Half height
		T hh;

		/// Get the boundaries
		Bounds<T> bounds() const {
			return {
				x - hw,
				x + hw,
				y - hh,
				y + hh,
			};
		}

		/// Check if a point is inside the bounding box
		bool contains(const Vec2<T>& p) const {
			const Bounds<T>& bounds_ = bounds();
			return bounds_.top <= p.y  && p.y <= bounds_.bottom &&
				   bounds_.left <= p.x && p.x <= bounds_.right;
		}

		/// Check if the bounding box intersects another
		bool intersects(const AABB& other) const {
			bool xx = std::abs(x - other.x) <= (hw + other.hw);
			bool yy = std::abs(y - other.y) <= (hh + other.hh);

			return xx && yy;
		}
	};

	/// Float AABB
	typedef AABB<float> AABBf;
} }

#endif