#include "OE/Application/WindowImpl.hpp"
#include "OE/Application/InputManager.hpp"


namespace OrbitEngine { namespace Application {

	InputManager::InputManager()
		: m_WheelDelta(0.0f)
	{
		for (int i = 0; i < OE_MAX_KEYS; i++) {
			m_Keys[i] = false;
			m_KeysPressed[i] = false;
		}
		for (int i = 0; i < OE_MAX_CBUTTONS; i++) {
			m_CursorButtons[i] = false;
			m_CursorButtonsClicked[i] = false;
		}
	}

	InputManager::~InputManager()
	{
		//
	}

	void InputManager::reset() {
		// TODO Fix this :/
		//m_Resized = false;

		//if (m_Window->p_Focused) {
			/*
			if (m_CursorMode == CursorMode::GRABBED) {
				Math::Vec2i center = m_Window->getProperties().resolution / 2;

				if (m_CursorPos.x != center.x || m_CursorPos.y != center.y) {
					m_LastCursorPos = center;
					setCursorPosition((int)center.x, (int)center.y);
				}
			}
			*/
		//}

		//m_Cursor = Cursor::DEFAULT; // Back to default
		m_CursorMoved = false;
		m_CursorDelta = Math::Vec2i();
		//m_CursorDelta = m_LastCursorPos - m_CursorPos;
		//if (m_CursorMode != CursorMode::GRABBED || !hasFocus())
		//	m_LastCursorPos = m_CursorPos;

		for (int i = 0; i < OE_MAX_KEYS; i++)
			m_KeysPressed[i] = false;
		for (int i = 0; i < OE_MAX_CBUTTONS; i++)
			m_CursorButtonsClicked[i] = false;
		m_WheelDelta = 0;
	}

	bool InputManager::isKeyDown(const Key key) const {
		if (key >= OE_MAX_KEYS)
			return false;
		return m_Keys[key];
	}

	bool InputManager::isButtonDown(const Button button) const {
		if (button >= OE_MAX_CBUTTONS)
			return false;
		return m_CursorButtons[button];
	}

	bool InputManager::isKeyPressed(const Key key) const {
		if (key >= OE_MAX_KEYS)
			return false;
		return m_KeysPressed[key];
	}

	bool InputManager::isButtonClicked(const Button button) const {
		if (button >= OE_MAX_CBUTTONS)
			return false;
		return m_CursorButtonsClicked[button];
	}

	float InputManager::getWheelDelta() const
	{
		return m_WheelDelta;
	}

	Math::Vec2i InputManager::getCursorPosition() const {
		return m_CursorPos;
	}

	Math::Vec2i InputManager::getCursorDelta() const {
		return m_CursorDelta;
	}

	bool InputManager::cursorMoved() const
	{
		return m_CursorMoved;
	}

	void InputManager::onInputKey(Key key, bool down)
	{
		if (key < 0 || key >= OE_MAX_KEYS)
			return;

		if (down)
			m_KeysPressed[key] = !m_Keys[key];
		m_Keys[key] = down;

		onKeyEvent.FireEvent(KeyInputEvent(key, down, m_KeysPressed[key]));
	}

	void InputManager::onInputMouseButton(Button button, bool down)
	{
		m_CursorButtonsClicked[button] = !m_CursorButtons[button] && down;
		m_CursorButtons[button] = down;

		onButtonEvent.FireEvent(ButtonInputEvent(button, down, m_CursorButtonsClicked[button], m_CursorPos));
	}

	void InputManager::onInputMouseMove(int x, int y, bool deltaOnly)
	{
		Math::Vec2i delta;
		if (deltaOnly) {
			delta = Math::Vec2i(x, y);
		}
		else {
			delta = m_CursorPos;
			m_CursorPos = Math::Vec2i(x, y);
			m_CursorMoved = true;
			delta -= m_CursorPos;
		}

		m_CursorDelta += delta;

		onMouseMoveEvent.FireEvent(MouseMoveInputEvent(m_CursorPos, delta));
	}

	void InputManager::onInputWheel(float direction)
	{
		m_WheelDelta += direction;

		onWheelEvent.FireEvent(MouseWheelInputEvent(direction, m_CursorPos));
	}
} }