#ifndef UI_STYLE_HPP
#define UI_STYLE_HPP

#include <yoga/YGStyle.h>

#include <OE/Math/Color.hpp>

namespace OrbitEngine { namespace UI {

	struct Style {
		Math::Color4f background = Math::Color4f(0, 0, 0 ,0); // transparent
		YGStyle yoga_style { };
	};

} }

#endif