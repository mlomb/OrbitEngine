#ifndef MATH_SCISSOR_HPP
#define MATH_SCISSOR_HPP

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec4.hpp"

namespace OrbitEngine { namespace Math {
	struct Scissor {
	public:
		//Scissor(Math::Vec2i position, Math::Vec2i size)
		//	: position(position), size(size) {}
		Scissor(Math::Vec4i _rect)
			: rect(_rect) {}
		Scissor(int x, int y, int w, int h)
			: Scissor(Math::Vec4i(x, y, w, h)) {}
		Scissor()
			: Scissor(Math::Vec4i(0, 0, 32000, 32000)) {};

		union {
			/*
			struct {
				Math::Vec2i position;
				Math::Vec2i size;
			};
			*/
			Math::Vec4i rect;
		};
	};
} }

#endif