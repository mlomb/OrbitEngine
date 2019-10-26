#ifndef MATH_SCISSOR_HPP
#define MATH_SCISSOR_HPP

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec4.hpp"

namespace OrbitEngine { namespace Math {
	const float DEFAULT_SCISSOR_EXTENT = 32000.0f;

	struct Scissor {
	public:
		//Scissor(Math::Vec2i position, Math::Vec2i size)
		//	: position(position), size(size) {}
		Scissor(Math::Vec4f _rect)
			: rect(_rect) {}
		Scissor(float x, float y, float w, float h)
			: Scissor(Math::Vec4f(x, y, w, h)) {}
		Scissor()
			: Scissor(Math::Vec4f(-DEFAULT_SCISSOR_EXTENT, -DEFAULT_SCISSOR_EXTENT, 2 * DEFAULT_SCISSOR_EXTENT, 2 * DEFAULT_SCISSOR_EXTENT)) {};

		union {
			/*struct Rect {
				Math::Vec2f position;
				Math::Vec2f size;
			};*/
			Math::Vec4f rect;
		};
	};
} }

#endif