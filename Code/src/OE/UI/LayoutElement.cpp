#include "OE/UI/LayoutElement.hpp"

#include "OE/Misc/Log.hpp"

// The yoga layout engine
// https://yogalayout.com
#include <yoga/Yoga.h>

namespace OrbitEngine { namespace UI {

	LayoutElement::LayoutElement()
	{
		m_YogaNode = YGNodeNew();
		YGNodeSetContext(m_YogaNode, this);
	}

	LayoutElement::~LayoutElement()
	{
		YGNodeFreeRecursive(m_YogaNode);
	}
} }
