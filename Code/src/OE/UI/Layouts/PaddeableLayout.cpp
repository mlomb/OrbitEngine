#include "OE/UI/Layouts/PaddeableLayout.hpp"

#include "OE/UI/Elements/Element.hpp"

namespace OrbitEngine { namespace UI {
	Math::Vec4f PaddeableLayout::getFinalRelativeRect(Element* e)
	{
		return Math::Vec4f(m_Padding.xy, e->getSize() - (m_Padding.xy + m_Padding.zw));
	}

	float PaddeableLayout::getTotalPaddingAxis(int axis)
	{
		if (axis == 0)
			return m_Padding.x + m_Padding.z;
		else
			return m_Padding.y + m_Padding.w;
	}
} }