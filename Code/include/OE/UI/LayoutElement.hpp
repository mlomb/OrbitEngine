#ifndef UI_LAYOUT_ELEMENT_HPP
#define UI_LAYOUT_ELEMENT_HPP

#include "OE/UI/HierarchyElement.hpp"

// forward def to avoid include
class YGNode;

namespace OrbitEngine { namespace UI {

	class LayoutElement : public HierarchyElement {
	public:
		LayoutElement();
		virtual ~LayoutElement();

	private:
		YGNode* m_YogaNode;
	};

} }

#endif