#ifndef UI_EVENT_TREE_DISPATCHER_HPP
#define UI_EVENT_TREE_DISPATCHER_HPP

#include <queue>

#include "OE/UI/TreeProcessor.hpp"

#include "OE/UI/Events/Events.hpp"

namespace OrbitEngine { namespace UI {

	class EventTreeDispatcher : public TreeProcessor {
	public:
		EventTreeDispatcher(Surface* surface);
		virtual ~EventTreeDispatcher();

		void process() override;

		void sendMouseMove(const Math::Vec2f& position);

	private:
		std::queue<EventBase*> m_EventQueue;

		void processEventQueue();
	};

} }

#endif