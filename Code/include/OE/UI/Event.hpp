#ifndef UI_EVENT_HPP
#define UI_EVENT_HPP

#include "OE/Application/EventHandler.hpp"
#include "OE/Application/InputEvents.hpp"
#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace UI {
	class Element;

	class Event : public Application::PropagableEvent {
	public:
		Event() {};

		Element* target;
	};

	class KeyBaseEvent : public Event {
	public:
		KeyBaseEvent(const Application::Key key) : key(key) {};

		const Application::Key key;
	};

	class MouseBaseEvent : public Event {
	public:
		MouseBaseEvent(const Math::Vec2f position, const Math::Vec2f relativePosition) : position(position), relativePosition(relativePosition) {};

		const Math::Vec2f position;
		const Math::Vec2f relativePosition;
	};

	class MouseMoveEvent : public MouseBaseEvent {
	public:
		MouseMoveEvent(const Math::Vec2f position, const Math::Vec2f relativePosition, const Math::Vec2f delta) : MouseBaseEvent(position, relativePosition), delta(delta) {};

		const Math::Vec2f delta;
	};

	class WheelEvent : public Event {
	public:
		WheelEvent(float direction) : direction(direction) {};

		float direction;
	};
} }

#endif