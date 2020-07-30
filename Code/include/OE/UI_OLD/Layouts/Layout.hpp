#ifndef UI_LAYOUT_HPP
#define UI_LAYOUT_HPP

#include "OE/UI/Layouts/LayoutElement.hpp"

namespace OrbitEngine { namespace UI {
	class Element;

	class Layout {
	public:
		virtual void calculateLayout(Element* element) = 0;
		virtual void setLayout(Element* element) = 0;
	};
} }

#endif