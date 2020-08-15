#ifndef UI_EVENTS_HPP
#define UI_EVENTS_HPP

#include <vector>

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace UI {

	class Element;

	enum class EventTypeID {
		MOUSE_MOVE,
		MOUSE_ENTER,
		MOUSE_LEAVE,
		MOUSE_DOWN,
		MOUSE_UP,

		CAPTURE_IN,
		CAPTURE_OUT,

		INVALID
	};

	class EventBase {
	public:
		EventTypeID type = EventTypeID::INVALID;
		Element* target = nullptr;

		// Propagation path
		// See https://w3c.github.io/uievents/#event-flow
		std::vector<Element*> path;
	};

	struct MouseEvent : public EventBase {
		Math::Vec2f mousePosition;
	};

	struct MouseButtonEvent : public MouseEvent {
		int button;
	};

} }

#endif