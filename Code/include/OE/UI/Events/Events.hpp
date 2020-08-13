#ifndef UI_EVENTS_HPP
#define UI_EVENTS_HPP

#include <vector>

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace UI {

	class Element;

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

	class MouseEvent : public EventBase {
	public:
		Math::Vec2f mousePosition;

		MouseEvent(EventTypeID type) : EventBase(type) { };
	};

	class MouseMoveEvent : public MouseEvent {
	public:
		MouseMoveEvent() : MouseEvent(EventTypeID::MOUSE_MOVE) { };
	};
	class MouseEnterEvent : public MouseEvent {
	public:
		MouseEnterEvent() : MouseEvent(EventTypeID::MOUSE_ENTER) { };
	};
	class MouseLeaveEvent : public MouseEvent {
	public:
		MouseLeaveEvent() : MouseEvent(EventTypeID::MOUSE_LEAVE) { };
	};

} }

#endif