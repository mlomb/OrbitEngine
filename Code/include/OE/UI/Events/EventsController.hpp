#ifndef UI_EVENTS_CONTROLLER_HPP
#define UI_EVENTS_CONTROLLER_HPP

#include <queue>

#include "OE/UI/TreeProcessor.hpp"

#include "OE/UI/Events/Events.hpp"

namespace OrbitEngine { namespace UI {

	class EventsController : public TreeProcessor {
	public:
		EventsController(Surface* surface);
		virtual ~EventsController();

		void process() override;

		void sendMouseMove(const Math::Vec2f& position);

	private:
		std::queue<EventBase*> m_EventQueue;

		void processEventQueue();
	};

} }

#endif