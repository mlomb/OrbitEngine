#include "OE/UI/Components/Draggable.hpp"

#include "OE/UI/Events/Events.hpp"
#include "OE/UI/Events/EventsController.hpp"

#include <iostream>

namespace OrbitEngine { namespace UI {
	Draggable::Draggable()
		: m_Dragging(false)
	{
	}

	Draggable::~Draggable()
	{
	}

	void Draggable::executeDefault(EventBase* evt)
	{
		MouseEvent* mevt = static_cast<MouseEvent*>(evt);

		Element::executeDefault(evt);

		switch (evt->type) {
		case EventTypeID::MOUSE_DOWN:
			m_Dragging = true;
			m_DragStart = mevt->mousePosition;
			m_DragOffset = mevt->mousePosition - getBoundingRect().position;
			getEventsController()->captureElement(this);
			break;
		case EventTypeID::MOUSE_UP:
			getEventsController()->captureElement(nullptr);
			// fall
		case EventTypeID::CAPTURE_OUT:
			m_Dragging = false;
			break;
		case EventTypeID::MOUSE_MOVE:
			if (m_Dragging) {
				auto delta = mevt->mousePosition - m_DragStart;
				std::cout << delta.x << ", " << delta.y << std::endl;
			}
			break;
		}
	}
} }