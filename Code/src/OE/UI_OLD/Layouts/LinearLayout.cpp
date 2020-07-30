#include "OE/UI/Layouts/LinearLayout.hpp"

namespace OrbitEngine { namespace UI {
	void LinearLayout::calculateLayout(Element* element)
	{
		calculateLayoutAxis(element, Axis::HORIZONTAL);
		calculateLayoutAxis(element, Axis::VERTICAL);
	}

	void LinearLayout::setLayout(Element* element)
	{
		setLayoutAxis(element, Axis::HORIZONTAL);
		setLayoutAxis(element, Axis::VERTICAL);
	}

	void LinearLayout::calculateLayoutAxis(Element* element, Axis axis)
	{
		std::vector<Element*> childs = element->getChildrens();
		
		float padding = getTotalPaddingAxis(axis);

		float totalMin = padding;
		float totalPref = padding;
		float totalFlex = 0.0f;

		float min;
		float pref;
		float flex;

		bool flag2 = (int)m_Orientation != axis;
		for (Element* e : childs) {
			GetChildSizes(e, axis, m_ControlChildrensSize[axis], m_ExpandChildrens[axis], min, pref, flex);

			if (flag2) {
				totalMin = std::max(min + padding, totalMin);
				totalPref = std::max(pref + padding, totalPref);
				totalFlex = std::max(flex, totalFlex);
			}
			else {
				totalMin += min + m_Spacing;
				totalPref += pref + m_Spacing;
				totalFlex += flex;
			}
		}
		if (!flag2 && childs.size() > 0)
		{
			totalMin -= m_Spacing;
			totalPref -= m_Spacing;
		}
		totalPref = std::max(totalMin, totalPref);

		element->setLayoutPropertyAlongAxis(totalMin, LayoutProperty::MINIMUM_SIZE, axis);
		element->setLayoutPropertyAlongAxis(totalPref, LayoutProperty::PREFERRED_SIZE, axis);
		element->setLayoutPropertyAlongAxis(totalFlex, LayoutProperty::FLEXIBLE_SIZE, axis);
	}

	void LinearLayout::setLayoutAxis(Element* element, Axis axis)
	{
		std::vector<Element*> childs = element->getChildrens();
		float size = element->getSize()[axis];
		float paddingInAxis = getPadding()[axis];
		float totalPaddingInAxis = getTotalPaddingAxis(axis);

		bool flag2 = (int)m_Orientation != axis;
		if (flag2) {
			float value = size - totalPaddingInAxis;
			float min;
			float pref;
			float flex;
			for (Element* e : childs) {
				GetChildSizes(e, axis, m_ControlChildrensSize[axis], m_ExpandChildrens[axis], min, pref, flex);
				float num4 = Math::clamp(value, min, (flex <= 0.0f) ? pref : size);
				float startOffset = paddingInAxis;
				
				if (m_ControlChildrensSize[axis]) {
					e->setPositionAndSizeOnAxis(Math::Vec2f(startOffset, num4), axis);
				}
				else {
					float n5 = (num4 - size) * 0;
					e->setPositionAlongAxis(startOffset + n5, axis);
				}
			}
		} else {
			float parentMin = element->getLayoutPropertyAlongAxis(LayoutProperty::MINIMUM_SIZE, axis);
			float parentPref = element->getLayoutPropertyAlongAxis(LayoutProperty::PREFERRED_SIZE, axis);
			float parentFlex = element->getLayoutPropertyAlongAxis(LayoutProperty::FLEXIBLE_SIZE, axis);

			float num6 = paddingInAxis;
			if (parentFlex == 0.0f && parentPref < size)
				num6 = /*0 * (getPreferredSize()[axis] - totalPaddingInAxis) +*/ paddingInAxis;
			float t = 0.0f;
			if (parentMin != parentPref)
				t = Math::clamp((size - parentMin) / (parentPref - parentMin), 0.0f, 1.0f);
			float num7 = 0.0f;
			if (size > parentPref)
				if (parentFlex > 0.0f)
					num7 = (size - parentPref) / parentFlex;

			float min;
			float pref;
			float flex;

			for (Element* e : childs) {
				GetChildSizes(e, axis, m_ControlChildrensSize[axis], m_ExpandChildrens[axis], min, pref, flex);
				float num9 = Math::lerp(min, pref, t);
				num9 += flex * num7;

				e->setPivot(Math::Vec2f());
				e->setAnchor(Anchor::TOP_LEFT);
				e->setPositionAlongAxis(num6, axis);
				if (m_ControlChildrensSize[axis])
					e->setSizeAlongAxis(num9, axis);

				num6 += num9 + m_Spacing;
			}
		}
	}

	void LinearLayout::GetChildSizes(Element* element, Axis axis, bool controlChildsSize, bool expandChilds, float & min, float & preferred, float & flexible)
	{
		min = 0;
		preferred = 0;
		flexible = 0;

		if (!controlChildsSize) {
			min = element->getSize()[axis]; // sizeDelta
			preferred = min;
			flexible = 0.0f;
		}
		else {
			min = element->getLayoutPropertyAlongAxis(LayoutProperty::MINIMUM_SIZE, axis);
			preferred = element->getLayoutPropertyAlongAxis(LayoutProperty::PREFERRED_SIZE, axis);
			flexible = element->getLayoutPropertyAlongAxis(LayoutProperty::FLEXIBLE_SIZE, axis);
		}
		if (expandChilds)
			flexible = std::max(flexible, 1.0f);
	}
} }