#include "OE/UI/Components/Draggable.hpp"

#include "OE/UI/Events/Events.hpp"
#include "OE/UI/Events/EventsController.hpp"

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
		Element::executeDefault(evt);

		switch (evt->type) {
		case EventTypeID::MOUSE_DOWN:
			m_Dragging = true;
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
				printf("AAA\n");
			}
			break;
		}
	}
} }