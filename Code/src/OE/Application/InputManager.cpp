#include "OE/Application/WindowImpl.hpp"
#include "OE/Application/InputManager.hpp"


namespace OrbitEngine { namespace Application {
	InputManager::InputManager(priv::WindowImpl* window) : m_Window(window) {
		m_Keys = new bool[MAX_KEYS];
		m_KeysPressed = new bool[MAX_KEYS];
		m_CursorButtons = new bool[MAX_CBUTTONS];
		m_CursorButtonsClicked = new bool[MAX_CBUTTONS];

		for (int i = 0; i < MAX_KEYS; i++) {
			m_Keys[i] = false;
			m_KeysPressed[i] = false;
		}
		for (int i = 0; i < MAX_CBUTTONS; i++) {
			m_CursorButtons[i] = false;
			m_CursorButtonsClicked[i] = false;
		}
	}

	InputManager::~InputManager()
	{
		delete[] m_Keys;
		delete[] m_KeysPressed;
		delete[] m_CursorButtons;
		delete[] m_CursorButtonsClicked;
	}

	void InputManager::update() {
		// TODO Fix this :/
		//m_Resized = false;

		if (hasFocus()) {
			/*
			if (m_CursorMode == CursorMode::GRABBED) {
				Math::Vec2i center = m_Window->getProperties().resolution / 2;

				if (m_CursorPos.x != center.x || m_CursorPos.y != center.y) {
					m_LastCursorPos = center;
					setCursorPosition((int)center.x, (int)center.y);
				}
			}
			*/
		}

		//m_Cursor = Cursor::DEFAULT; // Back to default
		m_CursorDelta = Math::Vec2i();
		//m_CursorDelta = m_LastCursorPos - m_CursorPos;
		//if (m_CursorMode != CursorMode::GRABBED || !hasFocus())
		//	m_LastCursorPos = m_CursorPos;

		for (int i = 0; i < MAX_KEYS; i++)
			m_KeysPressed[i] = false;
		for (int i = 0; i < MAX_CBUTTONS; i++)
			m_CursorButtonsClicked[i] = false;
		m_WheelDelta = 0;
	}

	bool InputManager::isKeyDown(const Key key) const {
		if (key >= MAX_KEYS)
			return false;
		return m_Keys[key];
	}

	bool InputManager::isButtonDown(const Button button) const {
		if (button >= MAX_CBUTTONS)
			return false;
		return m_CursorButtons[button];
	}

	bool InputManager::isKeyPressed(const Key key) const {
		if (key >= MAX_KEYS)
			return false;
		return m_KeysPressed[key];
	}

	bool InputManager::isButtonClicked(const Button button) const {
		if (button >= MAX_CBUTTONS)
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

	void InputManager::setCursorPosition(int x, int y, bool relative)
	{
		//m_Window->setCursorPosition(x, y, relative);
	}

	Math::Vec2i InputManager::getCursorDelta() const {
		return m_CursorDelta;
	}

	bool InputManager::wasResized() const {
		return m_Resized;
	}

	bool InputManager::hasFocus() const {
		return m_Focus;
	}

	CursorMode InputManager::getCursorMode() const {
		return m_CursorMode;
	}

	priv::WindowImpl* InputManager::getWindow() const {
		return m_Window;
	}

	void InputManager::requestCursorMode(const CursorMode cursorMode) {
		m_Window->requestCursorMode(cursorMode);
	}

	void InputManager::setCursor(const Cursor cursor)
	{
		m_Cursor = cursor;
	}

	void InputManager::onInputKey(Key key, bool down)
	{
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

	void InputManager::onInputFocus(bool focus)
	{
		m_Focus = focus;

		if (!focus) {
			m_CursorMode = CursorMode::NORMAL;
		}

		onFocusEvent.FireEvent(WindowFocusInputEvent(m_Focus));
	}

	void InputManager::onInputResized(int width, int height)
	{
		unsigned int lastw, lasth;
		lastw = m_Window->getProperties().resolution.x;
		lasth = m_Window->getProperties().resolution.y;

		m_Window->getProperties().resolution = Math::Vec2i(width, height);

		m_Resized = true;

		onResizedEvent.FireEvent(WindowResizedInputEvent(lastw, lasth, m_Window->getProperties().resolution.x, m_Window->getProperties().resolution.y));
	}
} }