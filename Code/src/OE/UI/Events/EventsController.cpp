#include "OE/UI/Events/EventsController.hpp"

#include "OE/UI/Element.hpp"
#include "OE/UI/Surface.hpp"

namespace OrbitEngine { namespace UI {

	EventsController::EventsController(Surface* surface)
		: TreeProcessor(surface),
		m_LastElementUnderMouse(nullptr)
	{
	}

	EventsController::~EventsController()
	{
	}

	void EventsController::process()
	{
		processEventQueue();
	}

	void EventsController::sendMouseMove(const Math::Vec2f& position)
	{
		MouseMoveEvent* e = new MouseMoveEvent();
		e->mousePosition = position;
		e->target = m_Surface->findElementsAt(m_Surface->getRoot(), position, &e->path);
		queueEvent(e);
	}

	void EventsController::queueEvent(EventBase* evt)
	{
		m_EventQueue.push(evt);
	}

	void EventsController::processEventQueue()
	{
		while (!m_EventQueue.empty()) {
			EventBase* evt = m_EventQueue.front();
			m_EventQueue.pop();

			if (evt->type == EventTypeID::MOUSE_MOVE) {
				updateElementUnderMouse(static_cast<MouseEvent*>(evt)->mousePosition);
			}

			// propagate, bubble, stuff

			//if(!prevent default...)
			// default action
			if (evt->path.size() > 0) {
				for (Element* in_path : evt->path)
					in_path->executeDefault(evt);
			}
			else if(evt->target) {
				evt->target->executeDefault(evt);
			}

			// all Events are allocated by the controller
			delete evt;
		}
	}

	void EventsController::updateElementUnderMouse(const Math::Vec2f& mousePosition)
	{
		Element* elementUnderMouse = m_Surface->findElementsAt(m_Surface->getRoot(), mousePosition);
		if (m_LastElementUnderMouse == elementUnderMouse)
			return;

		// compute the LCA of elementUnderMouse and m_LastElementUnderMouse
		// then send MouseLeave on the branch of m_LastElementUnderMouse
		// then send MouseEnter on the branch of elementUnderMouse

		std::vector<Element*> enter, leave;

		int lastDepth = m_LastElementUnderMouse ? m_LastElementUnderMouse->getDepth() : 0;
		int currDepth = elementUnderMouse ? elementUnderMouse->getDepth() : 0;

		Element* last = m_LastElementUnderMouse;
		Element* curr = elementUnderMouse;

		while (lastDepth > currDepth) {
			leave.emplace_back(last);
			lastDepth--;
			last = last->getParent();
		}
		while (currDepth > lastDepth) {
			enter.emplace_back(curr);
			currDepth--;
			curr = curr->getParent();
		}

		// now last and curr have the same depth
		while (last != curr) {
			leave.emplace_back(last);
			enter.emplace_back(curr);

			last = last->getParent();
			curr = curr->getParent();
		}

		// queue events
		for (Element* elem : enter) {
			MouseEnterEvent* e = new MouseEnterEvent();
			e->mousePosition = mousePosition;
			e->target = elem;
			queueEvent(e);
		}
		for (Element* elem : leave) {
			MouseLeaveEvent* e = new MouseLeaveEvent();
			e->mousePosition = mousePosition;
			e->target = elem;
			queueEvent(e);
		}

		m_LastElementUnderMouse = elementUnderMouse;
	}

} }
