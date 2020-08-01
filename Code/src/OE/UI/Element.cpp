#include "OE/UI/Element.hpp"

#include <yoga/YGNode.h>

#include "OE/Misc/Log.hpp"
#include "OE/UI/Painter.hpp"

namespace OrbitEngine { namespace UI {
	Element::Element()
		: m_Parent(nullptr)
	{
		m_Node = YGNodeNew();
		YGNodeSetContext(m_Node, this);
	}

	Element::~Element()
	{
		// TODO: not like this

		for (Element* elem : m_Childrens)
			delete elem;
		YGNodeFreeRecursive(m_Node);
	}

	void Element::setStyle(Style style)
	{
		m_Style = style;
		m_Node->setStyle(style.yoga_style);
	}

	void Element::setOwner(Element* parent)
	{
		m_Parent = parent;
	}

	void Element::addElement(Element* child, uint32_t index)
	{
		// this function sets the correct owner in yoga land
		YGNodeInsertChild(m_Node, child->m_Node, index);

		child->setOwner(this);
		m_Childrens.insert(m_Childrens.begin() + index, child);
	}

	void Element::generateContent(Painter* painter)
	{
		painter->drawRectangle(m_BoundingBox.xy, m_BoundingBox.zw, m_Style.background);
	}

	YGSize Element::YogaMeasureCallback(YGNode* yogaNode, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
	{
		Element* element_ptr = static_cast<Element*>(yogaNode->getContext());
		auto size = element_ptr->measureContent(width, widthMode, height, heightMode);
		return { size.x, size.y };
	}

	void Element::setAsTextType()
	{
		YGNodeSetNodeType(m_Node, YGNodeType::YGNodeTypeText);
	}

	void Element::enableMeasurement()
	{
		YGNodeSetMeasureFunc(m_Node, Element::YogaMeasureCallback);
	}

	void Element::layout(const Math::Vec2f& availableSpace)
	{
		YGNodeCalculateLayout(m_Node, availableSpace.w, availableSpace.h, YGDirectionLTR);
		layoutSubtree();
	}

	void Element::layoutSubtree()
	{
		m_LayoutRect = Math::Vec4f(
			YGNodeLayoutGetLeft(m_Node), YGNodeLayoutGetTop(m_Node),
			YGNodeLayoutGetWidth(m_Node), YGNodeLayoutGetHeight(m_Node)
		);
		m_BoundingBox.xy = m_Parent ? m_Parent->m_BoundingBox.xy : Math::Vec2f(0, 0);
		m_BoundingBox.xy += m_LayoutRect.xy;
		m_BoundingBox.zw = m_LayoutRect.zw;

		// TODO: for now, update the whole tree anyway
		//if (YGNodeGetHasNewLayout(m_Node)) {
			for (Element* e : m_Childrens)
				e->layoutSubtree();

		//	YGNodeSetHasNewLayout(m_Node, false);
		//}
	}

	void Element::markAsDirty()
	{
		if (!YGNodeHasMeasureFunc(m_Node)) {
			OE_ASSERT_MSG(false, "Only leaf elements with custom measure functions should mark themselves as dirty");
			return;
		}
		YGNodeMarkDirty(m_Node);
	}

	Math::Vec2f Element::measureContent(float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
	{
		OE_ASSERT_MSG(false, "measureContent is not overrided");
		return { };
	}
} }
