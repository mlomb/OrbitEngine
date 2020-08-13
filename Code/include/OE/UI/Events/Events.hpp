#ifndef UI_EVENTS_HPP
#define UI_EVENTS_HPP

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace UI {

	enum class EventTypeID {
		MOUSE_MOVE,
		MOUSE_ENTER,
		MOUSE_LEAVE
	};

	class EventBase {
	public:
		const EventTypeID type;
		Element* target;
		std::vector<Element*> path;

	protected:
		EventBase(EventTypeID _type) : type(_type), target(nullptr) { };
	};

	class MouseMoveEvent : public EventBase {
	public:
		Math::Vec2f mousePosition;

		MouseMoveEvent() : EventBase(EventTypeID::MOUSE_MOVE) { };
	};

} }

#endif