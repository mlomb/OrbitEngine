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
		void captureElement(Element* element);
		void releaseCapture();
		Element* getElementOverMouse() const;

		void sendMouseMove(const Math::Vec2f& position);
		void sendMouseButton(int button, bool down, const Math::Vec2f& position);

	private:
		std::queue<EventBase*> m_EventQueue;

		Element* m_LastElementUnderMouse;
		Element* m_CapturedElement;

		void queueEvent(EventBase* evt);
		void processEventQueue();
		void updateElementUnderMouse(const Math::Vec2f& mousePosition);
	};

} }

#endif