#ifndef APPLICATION_EVENT_HPP
#define APPLICATION_EVENT_HPP

namespace OrbitEngine { namespace Application {
	class Event {
	protected:
		Event() {}
	};

	class PropagableEvent : public Event {
	public:
		bool propagating = true;

	protected:
		PropagableEvent() {}
	};
} }

#endif