#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include <vector>

// The yoga layout engine
// https://yogalayout.com
#include <yoga/Yoga.h>

#include "OE/Math/Rect.hpp"
#include "Style.hpp"

namespace OrbitEngine { namespace UI {
	class Painter;

	/// The base of all UI elements
	class Element {
	public:
		Element();
		virtual ~Element();

		void setOwner(Element* parent);
		void addElement(Element* child, uint32_t index);

	protected:
		friend class Composer;

		Element* m_Parent;
		std::vector<Element*> m_Childrens;
		
		void setAsTextType();
		void enableMeasurement();
		void markAsDirty();
		void layout(const Math::Vec2f& availableSpace);

		virtual Math::Vec2f measureContent(float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
		virtual void generateContent(Painter* painter);

	public: // TODO: set as private
		YGNodeRef m_Node;

		Math::Rectf m_LayoutRect;
		Math::Rectf m_BoundingBox;
		ResolvedStyle m_ResolvedStyle;

		// called when the layout has changed
		void layoutSubtree();

		static YGSize YogaMeasureCallback(YGNode* yogaNode, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
	};
} }

#endif