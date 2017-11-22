#include "OE/UI/Elements/Canvas.hpp"

#include "OE/Application/WindowImpl.hpp"
#include "OE/System/System.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace UI {

	Canvas::Canvas()
		: m_FocusedElement(nullptr),
		m_CapturedElement(nullptr),
		m_PressedElement(nullptr),
		m_LastClickedElement(nullptr)
	{
		m_LayoutManager = new LayoutManager();
		// Suscribe events if parent
	}

	Canvas::Canvas(Application::priv::WindowImpl* view)
		: Canvas()
	{
		m_Window = view;

		view->getInputManager()->onKeyEvent.AddListener(this, &Canvas::input_keyEvent);
		view->getInputManager()->onButtonEvent.AddListener(this, &Canvas::input_buttonEvent);
		view->getInputManager()->onMouseMoveEvent.AddListener(this, &Canvas::input_mouseMoveEvent);
		view->getInputManager()->onWheelEvent.AddListener(this, &Canvas::input_wheelEvent);
	}

	void Canvas::captureElement(Element* element)
	{
		m_CapturedElement = element;
	}

	void Canvas::onUpdate(float deltaTime)
	{
		if (!getParent()) {
			if (m_Window)
				m_Window->getInputManager()->setCursor(m_CapturedElement ? m_CapturedElement->getFinalCursorAtMe() : m_CurrentCursor);
		}
	}

	void Canvas::onLateUpdate()
	{
		m_LayoutManager->performLayout(this);
	}

	void Canvas::onValidate()
	{
		setAnchor(Anchor::TOP_LEFT);
		setPivot(Math::Vec2f());
		setPosition(Math::Vec2f());
		setSize(m_Window->getProperties().resolution);
	}

	void Canvas::input_keyEvent(const Application::KeyInputEvent& keyEvent)
	{
		KeyBaseEvent e(keyEvent.key);

		if (keyEvent.down) {
			OE_UI_EVENT_PROPAGATE(m_FocusedElement, onKeyDown, e);
			if (keyEvent.pressed)
				OE_UI_EVENT_PROPAGATE(m_FocusedElement, onKeyPress, e);
		}
		else
			OE_UI_EVENT_PROPAGATE(m_FocusedElement, onKeyUp, e);
	}

	void Canvas::input_buttonEvent(const Application::ButtonInputEvent& buttonEvent)
	{
		Element* element = Element::getComponentAt(buttonEvent.position);

		MouseBaseEvent e(buttonEvent.position, (element ? Math::Vec2f(buttonEvent.position) - element->getRenderPosition() : Math::Vec2f()));

		if (buttonEvent.down) {
			OE_UI_EVENT_PROPAGATE(element, onMouseDown, e);
			m_PressedElement = element;
			//if (m_PressedComponent)
			//	m_PressedComponent->m_Pressed = true;
			changeFocus(element);
		}
		else {
			OE_UI_EVENT_PROPAGATE(element, onMouseUp, e);
			if (m_PressedElement == element) {

				long long now = System::System::Instance()->currentNano() * 1000000;

				if (m_LastClickedElement == element) {
					if (now - m_LastClickTime < 500) // 500ms
						m_ClickCount++;
					else
						m_ClickCount = 1;
				}
				else
					m_ClickCount = 1;

				m_LastClickTime = now;
				m_LastClickedElement = element;

				OE_UI_EVENT_PROPAGATE(element, onClick, e);
				if (m_ClickCount == 2)
					OE_UI_EVENT_PROPAGATE(element, onDblClick, e);
			}
			else {
				m_ClickCount = 1;
				m_LastClickTime = 0;
				m_LastClickedElement = nullptr;
			}
			if (m_PressedElement) {
				//m_PressedComponent->m_Pressed = false;
				m_PressedElement = nullptr;
			}
		}
	}

	void Canvas::input_mouseMoveEvent(const Application::MouseMoveInputEvent& mouseMoveEvent)
	{
		//updateMousePosition(this, mouseMoveEvent);

		Element* element = getComponentAt(mouseMoveEvent.position);
		if (element)
			m_CurrentCursor = element->getFinalCursorAtMe();

		MouseMoveEvent e = MouseMoveEvent(mouseMoveEvent.position, (element ? Math::Vec2f(mouseMoveEvent.position) - element->getRenderPosition() : Math::Vec2f()), mouseMoveEvent.delta);
		OE_UI_EVENT_PROPAGATE(element, onMouseMove, e);
	}

	void Canvas::input_wheelEvent(const Application::MouseWheelInputEvent& wheelEvent)
	{
		Element* element = Element::getComponentAt(wheelEvent.position);
		WheelEvent e = WheelEvent(wheelEvent.direction);
		OE_UI_EVENT_PROPAGATE(element, onWheelDelta, e);
	}

	void Canvas::changeFocus(Element* element)
	{
		if (m_FocusedElement) {
			Event e = Event();
			e.target = m_FocusedElement;
			OE_UI_EVENT_PROPAGATE(element, onFocusLost, e);
		}

		m_FocusedElement = element;

		if (m_FocusedElement) {
			Event e = Event();
			e.target = m_FocusedElement;
			OE_UI_EVENT_PROPAGATE(element, onFocusGain, e);
		}
	}

	Canvas* Canvas::getCanvas()
	{
		return this;
	}
} }