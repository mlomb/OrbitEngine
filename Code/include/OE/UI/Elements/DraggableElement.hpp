#ifndef UI_DRAGGABLE_ELEMENT_HPP
#define UI_DRAGGABLE_ELEMENT_HPP

#include "OE/UI/Elements/Element.hpp"
#include "OE/Application/EventHandler.hpp"

namespace OrbitEngine {	namespace UI {
	class DragEvent : public Event {
	public:
		DragEvent(const MouseMoveEvent& baseEvent, const Math::Vec2f dragStart, const Math::Vec2f relativeDragStart, const Math::Vec2f totalDelta) : baseEvent(baseEvent), dragStart(dragStart), relativeDragStart(relativeDragStart), totalDelta(totalDelta) {};

		const MouseMoveEvent& baseEvent;
		const Math::Vec2f relativeDragStart;
		const Math::Vec2f dragStart;
		const Math::Vec2f totalDelta;
	};

	class DraggableElement : public virtual Element {
	public:
		DraggableElement();

		Application::EventHandler<DragEvent> onDrag;

		void setDraggingCursor(Application::Cursor draggingCursor) { m_DraggingCursor = draggingCursor; };

		Application::Cursor getDraggingCursor() { return m_DraggingCursor; };
		Application::Cursor getCurrentCursor();

	private:
		void mouseMove(const MouseMoveEvent& mouseMoveEvent);
		void mouseDown(const MouseBaseEvent& mouseEvent);
		void mouseUp(const MouseBaseEvent& mouseEvent);

		bool m_Dragging;
		Math::Vec2f m_RelativeDragStart;
		Math::Vec2f m_DragStart;
		Application::Cursor m_DraggingCursor;
	};
} }

#endif