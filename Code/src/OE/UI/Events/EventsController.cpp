#include "OE/UI/Events/EventsController.hpp"

#include <iostream>

#include "OE/UI/Element.hpp"
#include "OE/UI/Surface.hpp"

namespace OrbitEngine { namespace UI {

	EventsController::EventsController(Surface* surface)
		: TreeProcessor(surface),
		m_LastElementUnderMouse(nullptr),
		m_CapturedElement(nullptr)
	{
	}

	EventsController::~EventsController()
	{
	}

	void EventsController::process()
	{
		processEventQueue();
	}

	void EventsController::captureElement(Element* element)
	{
		if (m_CapturedElement == element)
			return;

		if (m_CapturedElement) {
			EventBase* e = new EventBase();
			e->type = EventTypeID::CAPTURE_OUT;
			e->target = m_CapturedElement;
			queueEvent(e);
		}

		if (element) {
			EventBase* e = new EventBase();
			e->type = EventTypeID::CAPTURE_IN;
			e->target = element;
			queueEvent(e);
		}

		m_CapturedElement = element;
	}

	void EventsController::releaseCapture()
	{
		captureElement(nullptr);
	}

	void EventsController::sendMouseMove(const Math::Vec2f& position)
	{
		MouseEvent* e = new MouseEvent();
		e->type = EventTypeID::MOUSE_MOVE;
		e->mousePosition = position;
		queueEvent(e);
	}

	void EventsController::sendMouseButton(int button, bool down, const Math::Vec2f& position)
	{
		MouseButtonEvent* e = new MouseButtonEvent();
		e->type = down ? EventTypeID::MOUSE_DOWN : EventTypeID::MOUSE_UP;
		e->mousePosition = position;
		e->button = button;
		queueEvent(e);
	}	

	void EventsController::queueEvent(EventBase* evt)
	{
		MouseEvent* mevt = static_cast<MouseEvent*>(evt);

		if (!evt->target && m_CapturedElement) {
			evt->target = m_CapturedElement;
		}

		switch (evt->type) {
		case EventTypeID::MOUSE_ENTER:
		case EventTypeID::MOUSE_LEAVE:
			// no path
			break;
		case EventTypeID::MOUSE_MOVE:
		case EventTypeID::MOUSE_UP:
		case EventTypeID::MOUSE_DOWN:
			if (!evt->target) {
				// compute target and path using the mouse position
				evt->target = m_Surface->findElementsAt(m_Surface->getRoot(), mevt->mousePosition, &evt->path);
			}
			// FALL
		default:
			if (evt->target && evt->path.size() == 0) {
				// compute path going up in the tree
				Element* el = evt->target;
				while (el) {
					evt->path.emplace_back(el);
					el = el->getParent();
				}
				std::reverse(evt->path.begin(), evt->path.end());
			}
		}
		//if (evt->target)
		//	std::cout << evt->target->getQualifiedName() << std::endl;
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
		for (Element* elem : leave) {
			MouseEvent* e = new MouseEvent();
			e->type = EventTypeID::MOUSE_LEAVE;
			e->mousePosition = mousePosition;
			e->target = elem;
			queueEvent(e);
		}
		for (Element* elem : enter) {
			MouseEvent* e = new MouseEvent();
			e->type = EventTypeID::MOUSE_ENTER;
			e->mousePosition = mousePosition;
			e->target = elem;
			queueEvent(e);
		}

		m_LastElementUnderMouse = elementUnderMouse;
	}

	Element* EventsController::getElementOverMouse() const
	{
		return m_LastElementUnderMouse;
	}

} }
