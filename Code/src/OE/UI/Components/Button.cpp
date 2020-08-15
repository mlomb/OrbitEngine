#include "OE/UI/Components/Button.hpp"

#include "OE/UI/Events/Events.hpp"
#include "OE/UI/Events/EventsController.hpp"

namespace OrbitEngine { namespace UI {

	Button::Button()
	{
		setTag("Button");
	}

	Button::~Button()
	{
	}

	void Button::executeDefault(EventBase* evt)
	{
		Text::executeDefault(evt);

		switch (evt->type) {
		case EventTypeID::MOUSE_DOWN:
			getEventsController()->captureElement(this);
			break;
		case EventTypeID::MOUSE_UP:
			getEventsController()->captureElement(nullptr);
			break;
		}
	}

} }
