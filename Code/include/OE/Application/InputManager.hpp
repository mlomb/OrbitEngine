#ifndef APPLICATION_INPUT_MANAGER_HPP
#define APPLICATION_INPUT_MANAGER_HPP

#include "OE/Application/EventHandler.hpp"
#include "OE/Application/InputEvents.hpp"
#include "OE/Application/WindowImpl.hpp"

#include "OE/Math/Vec2.hpp"

#define OE_MAX_KEYS 512
#define OE_MAX_CBUTTONS 5

namespace OrbitEngine { namespace Application {

	class InputManager {
	public:
		static InputManager* Get();

		bool cursorMoved() const;
		bool isKeyDown(const Key key) const;
		bool isKeyPressed(const Key key) const;
		bool isButtonDown(const Button button) const;
		bool isButtonClicked(const Button button) const;
		float getWheelDelta() const;
		Math::Vec2i getCursorPosition() const;
		Math::Vec2i getCursorDelta() const;

		EventHandler<const KeyInputEvent> onKeyEvent;
		EventHandler<const ButtonInputEvent> onButtonEvent;
		EventHandler<const MouseMoveInputEvent> onMouseMoveEvent;
		EventHandler<const MouseWheelInputEvent> onWheelEvent;

		// -- INTERNAL USE --
		// This is where platforms inject events
		void onInputKey(Key key, bool down);
		void onInputMouseButton(Button button, bool down);
		void onInputMouseMove(int x, int y, bool deltaOnly = false);
		void onInputWheel(float direction);
		void reset();
	private:
		static InputManager* s_Instance;

		bool m_Keys[OE_MAX_KEYS];
		bool m_KeysPressed[OE_MAX_KEYS];
		bool m_CursorButtons[OE_MAX_CBUTTONS];
		bool m_CursorButtonsClicked[OE_MAX_CBUTTONS];
		bool m_CursorMoved;
		Math::Vec2i m_CursorDelta;
		Math::Vec2i m_CursorPos;
		float m_WheelDelta;

		InputManager();
		~InputManager();
	};
} }

#endif