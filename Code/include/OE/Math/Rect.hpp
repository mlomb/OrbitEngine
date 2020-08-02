#ifndef MATH_RECT_HPP
#define MATH_RECT_HPP

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec4.hpp"

namespace OrbitEngine { namespace Math {
	// 2D rect (x, y, width, height)
	template<typename T>
	struct Rect {
		union {
			struct { T x, y, width, height; };
			struct {
				Vec2<T> position;
				Vec2<T> size;
			};
			Vec4<T> v4;
		};

		Rect(const T& _x, const T& _y, const T& _width, const T& _height);
		Rect(const Vec2<T> _position, const Vec2<T> _size);
		Rect();
	};

	/// integer rect
	typedef Rect<int>    Recti;
	/// float rect
	typedef Rect<float>  Rectf;
} }

#include "Rect.inl"

#endif