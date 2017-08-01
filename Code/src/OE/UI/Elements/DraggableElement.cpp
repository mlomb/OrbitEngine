#include "OE/UI/Elements/DraggableElement.hpp"

#include "OE/UI/Elements/Canvas.hpp"

namespace OrbitEngine {	namespace UI {
	DraggableElement::DraggableElement()
		: m_Dragging(false),
		m_DraggingCursor(Application::Cursor::DEFAULT)
	{
		onMouseMove.AddListener(this, &DraggableElement::mouseMove);
		onMouseDown.AddListener(this, &DraggableElement::mouseDown);
		onMouseUp.AddListener(this, &DraggableElement::mouseUp);
	}

	Application::Cursor DraggableElement::getCurrentCursor()
	{
		if (m_Dragging && m_DraggingCursor != Application::Cursor::DEFAULT)
			return m_DraggingCursor;
		else
			return Element::getCurrentCursor();
	}
	
	void DraggableElement::mouseMove(const MouseMoveEvent& mouseMoveEvent)
	{
		if (m_Dragging) {
			DragEvent e = DragEvent(mouseMoveEvent, m_DragStart, m_RelativeDragStart, mouseMoveEvent.position - m_DragStart);
			e.target = this;
			onDrag.FireEvent(e);
		}
	}

	void DraggableElement::mouseDown(const MouseBaseEvent& mouseEvent)
	{
		m_Dragging = true;
		m_RelativeDragStart = mouseEvent.relativePosition;
		m_DragStart = getRenderPosition();
		getCanvas()->captureElement(this);
	}

	void DraggableElement::mouseUp(const MouseBaseEvent& mouseEvent)
	{
		m_Dragging = false;
		getCanvas()->captureElement(nullptr);
	}
} }