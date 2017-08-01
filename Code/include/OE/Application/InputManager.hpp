#ifndef APPLICATION_INPUT_MANAGER_HPP
#define APPLICATION_INPUT_MANAGER_HPP

#include "OE/Application/EventHandler.hpp"
#include "OE/Application/InputEvents.hpp"
#include "OE/Application/WindowImpl.hpp"

#include "OE/Math/Vec2.hpp"

#define MAX_KEYS 512
#define MAX_CBUTTONS 5

namespace OrbitEngine { namespace Application {
	class WindowImplType;

	namespace priv {
		class WindowImpl;
#if OE_WINDOWS
		class WindowWindows;
#elif OE_ANDROID
		class WindowAndroid;
#elif OE_EMSCRIPTEN
		class WindowEmscripten;
#elif OE_UNIX
		class WindowLinux;
#endif
	}
	
	class InputManager {
	public:
		InputManager(priv::WindowImpl* window);
		~InputManager();

		bool wasResized() const;
		bool hasFocus() const;
		bool isKeyDown(const Key key) const;
		bool isKeyPressed(const Key key) const;
		bool isButtonDown(const Button button) const;
		bool isButtonClicked(const Button button) const;
		float getWheelDelta() const;
		Math::Vec2i getCursorPosition() const;
		void setCursorPosition(int x, int y, bool relative = true);
		void setCursor(const Cursor cursor);
		void requestCursorMode(const CursorMode cursorMode);
		Math::Vec2i getCursorDelta() const;
		CursorMode getCursorMode() const;
		priv::WindowImpl* getWindow() const;

		EventHandler<KeyInputEvent> onKeyEvent;
		EventHandler<ButtonInputEvent> onButtonEvent;
		EventHandler<MouseMoveInputEvent> onMouseMoveEvent;
		EventHandler<MouseWheelInputEvent> onWheelEvent;
		EventHandler<WindowFocusInputEvent> onFocusEvent;
		EventHandler<WindowResizedInputEvent> onResizedEvent;

	private:
#if OE_WINDOWS
		friend class priv::WindowWindows;
#elif OE_ANDROID
		friend class priv::WindowAndroid;
#elif OE_EMSCRIPTEN
		friend class priv::WindowEmscripten;
#elif OE_UNIX
		friend class priv::WindowLinux;
#endif
		friend class priv::WindowImpl;

		priv::WindowImpl* m_Window;

		void update();

		bool* m_Keys;
		bool* m_KeysPressed;
		bool* m_CursorButtons;
		bool* m_CursorButtonsClicked;
		bool m_Resized;
		bool m_Focus;
		Math::Vec2i m_CursorPos;
		Math::Vec2i m_CursorDelta;
		float m_WheelDelta;
		CursorMode m_CursorMode = CursorMode::NORMAL;
		Cursor m_Cursor = Cursor::DEFAULT;

		void onInputResized(int width, int height);
		void onInputFocus(bool focus);
		void onInputKey(Key key, bool down);
		void onInputWheel(float direction);
		void onInputMouseButton(Button button, bool down);
		void onInputMouseMove(int x, int y, bool deltaOnly = false);
	};
} }

#endif