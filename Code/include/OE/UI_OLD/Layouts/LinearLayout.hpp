#ifndef UI_LINEAR_LAYOUT_HPP
#define UI_LINEAR_LAYOUT_HPP

#include "OE/UI/Layouts/PaddeableLayout.hpp"
#include "OE/UI/Layouts/AlignableLayout.hpp"
#include "OE/UI/Elements/Element.hpp"

namespace OrbitEngine { namespace UI {
	class LinearLayout : public PaddeableLayout, public AlignableLayout {
	public:
		void calculateLayout(Element* element) override;
		void setLayout(Element* element) override;

		float getSpacing() { return m_Spacing; }
		Orientation getOrientation() { return m_Orientation; }

		void setSpacing(float spacing) { m_Spacing = spacing; }
		void setOrientation(Orientation orientation) { m_Orientation = orientation; }
		void setExpandChildrens(bool widthExpand, bool heightExpand) { m_ExpandChildrens[0] = widthExpand; m_ExpandChildrens[1] = heightExpand; };
		void setControlChildrensSize(bool widthControlSize, bool heightControlSize) { m_ControlChildrensSize[0] = widthControlSize; m_ControlChildrensSize[1] = heightControlSize; };
	private:
		float m_Spacing;
		Orientation m_Orientation;
		bool m_ExpandChildrens[2];
		bool m_ControlChildrensSize[2];

		void calculateLayoutAxis(Element* element, Axis axis);
		void setLayoutAxis(Element* element, Axis axis);
		static void GetChildSizes(Element* element, Axis axis, bool controlChildsSize, bool expandChilds, float& min, float& preferred, float& flexible);
	};
} }

#endif