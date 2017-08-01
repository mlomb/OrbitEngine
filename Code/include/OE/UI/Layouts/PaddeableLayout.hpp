#ifndef UI_PADDEABLE_LAYOUT_HPP
#define UI_PADDEABLE_LAYOUT_HPP

#include "OE/UI/Layouts/Layout.hpp"

#include "OE/Math/Vec4.hpp"

namespace OrbitEngine { namespace UI {
	class Element;

	class PaddeableLayout : virtual public Layout {
	public:

		Math::Vec4f getPadding() { return m_Padding; };
		void setPadding(Math::Vec4f padding) { m_Padding = padding; };

		Math::Vec4f getFinalRelativeRect(Element* e);
		float getTotalPaddingAxis(int axis);

	private:
		Math::Vec4f m_Padding;
	};
} }

#endif