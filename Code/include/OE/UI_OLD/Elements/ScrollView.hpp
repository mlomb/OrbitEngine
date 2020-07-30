#ifndef UI_SCROLL_VIEW_HPP
#define UI_SCROLL_VIEW_HPP

#include "OE/UI/Elements/Panel.hpp"
#include "OE/UI/Elements/DraggableElement.hpp"

namespace OrbitEngine {	namespace UI {
	class ScrollView : public Panel, private DraggableElement {
	public:
		ScrollView();

		void setViewport(Element* element);
		void setContent(Element* element);

	private:
		Math::Vec2f m_ScrollPosition;

		Element* m_Viewport;
		Element* m_Content;

		void drag(const DragEvent& dragEvent);
		void wheelDelta(const WheelEvent& wheelEvent);

		void onValidate() override;
		void updateScrolling();
	};
} }

#endif