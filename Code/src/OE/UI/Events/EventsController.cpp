#include "OE/UI/Events/EventTreeDispatcher.hpp"

#include "OE/UI/Element.hpp"
#include "OE/UI/Surface.hpp"

namespace OrbitEngine { namespace UI {

	EventTreeDispatcher::EventTreeDispatcher(Surface* surface)
		: TreeProcessor(surface)
	{
	}

	EventTreeDispatcher::~EventTreeDispatcher()
	{
	}

	void EventTreeDispatcher::process()
	{
		processEventQueue();
	}

	void EventTreeDispatcher::sendMouseMove(const Math::Vec2f& position)
	{
		MouseMoveEvent* e = new MouseMoveEvent();
		e->mousePosition = position;
		e->target = m_Surface->findElementsAt(m_Surface->getRoot(), position, &e->path);
		m_EventQueue.push(e);
	}

	void EventTreeDispatcher::processEventQueue()
	{
		while (!m_EventQueue.empty()) {
			EventBase* evt = m_EventQueue.front();
			m_EventQueue.pop();

			// propagate, bubble, stuff

			//if(!prevent default...)
			// default action
			for (Element* in_path : evt->path)
				in_path->executeDefault(evt);
		}
	}

} }
