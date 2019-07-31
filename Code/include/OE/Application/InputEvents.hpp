#ifndef APPLICATION_INPUT_EVENTS_HPP
#define APPLICATION_INPUT_EVENTS_HPP

#include "OE/Application/Event.hpp"

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace Application {

	enum CursorMode {
		NORMAL,
		LOCKED,
		GRABBED
	};

	enum Key {
		BACKSPACE = 8,
		TAB = 9,
		CLEAR = 12, // Old
		ENTER = 13,
		SHIFT = 16,
		CONTROL = 17,
		ALT = 18,
		PAUSE = 19,
		CAPS_LOCK = 20,
		ESCAPE = 27,
		SPACEBAR = 32,
		PAGE_UP = 33,
		PAGE_DOWN = 34,
		END = 35,
		HOME = 36,
		ARROW_LEFT = 37,
		ARROW_UP = 38,
		ARROW_RIGHT = 39,
		ARROW_DOWN = 40,
		SNAPSHOT = 44,
		INSERT = 45,
		DELETE_ = 46,
		LWIN = 91,
		RWIN = 92,
		APPS = 93,
		SLEEP = 95,

		NUMPAD0 = 96,
		NUMPAD1 = 97,
		NUMPAD2 = 98,
		NUMPAD3 = 99,
		NUMPAD4 = 100,
		NUMPAD5 = 101,
		NUMPAD6 = 102,
		NUMPAD7 = 103,
		NUMPAD8 = 104,
		NUMPAD9 = 105,
		MULTIPLY = 106,
		ADD = 107,
		SUBTRACT = 109,
		DECIMAL = 110,
		DIVIDE = 111,
		NUMLOCK = 144,
		SCROLLLOCK = 145,

		F1 = 112,
		F2 = 113,
		F3 = 114,
		F4 = 115,
		F5 = 116,
		F6 = 117,
		F7 = 118,
		F8 = 119,
		F9 = 120,
		F10 = 121,
		F11 = 122,
		F12 = 123,
		F13 = 124,
		F14 = 125,
		F15 = 126,
		F16 = 127,
		F17 = 128,
		F18 = 129,
		F19 = 130,
		F20 = 131,
		F21 = 131,
		F22 = 131,
		F23 = 131,
		F24 = 131,

		OEM_1 = 186,
		OEM_PLUS = 187,
		OEM_COMMA = 188,
		OEM_MINUS = 189,
		OEM_PERIOD = 190,
		OEM_2 = 191,
		OEM_3 = 192,
		OEM_4 = 219,
		OEM_5 = 220,
		OEM_6 = 221,
		OEM_7 = 222,
		OEM_8 = 223,
		KEYPAD_0 = 48,
		KEYPAD_1 = 49,
		KEYPAD_2 = 50,
		KEYPAD_3 = 51,
		KEYPAD_4 = 52,
		KEYPAD_5 = 53,
		KEYPAD_6 = 54,
		KEYPAD_7 = 55,
		KEYPAD_8 = 56,
		KEYPAD_9 = 57,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LSHIFT = 160,
		RSHIFT = 161,
		LCONTROL = 162,
		RCONTROL = 163,
		LMENU = 164,
		RMENU = 165,
	};

	enum Button {
		LEFT = 0,
		RIGHT = 1,
		MIDDLE = 2,
		N4 = 3,
		N5 = 4
	};

	enum Cursor {
		DEFAULT,
		HIDDEN,
		ARROW,
		CROSS,
		HOURGLASS,
		HAND,
		ARROW_HOURGLASS,
		HELP,
		IBEAM,
		NO,
		SIZEALL,
		SIZENS,
		SIZENWSE,
		SIZEWE,
		UPARROW,
	};

	enum InputEventType {
		WINDOW_RESIZED,
		WINDOW_FOCUS,
		KEY,
		MOUSE,
		WHEEL,
		BUTTON
	};

	class InputEvent : public Event {
	public:
		InputEvent() : Event() {};

	};

	class KeyInputEvent : public InputEvent {
	public:
		KeyInputEvent(const Key _key, const bool _down, const bool _pressed)
			: key(_key), down(_down), pressed(_pressed) {};

		const Key key;
		const bool down;
		const bool pressed;
	};

	class MouseBaseInputEvent : public InputEvent {
	public:
		MouseBaseInputEvent(const InputEventType type, const Math::Vec2i position)
			: position(position) {};

		const Math::Vec2i position;
	};

	class ButtonInputEvent : public MouseBaseInputEvent {
	public:
		ButtonInputEvent(const Button button, const bool down, const bool clicked, const Math::Vec2i position)
			: MouseBaseInputEvent(InputEventType::BUTTON, position), button(button), down(down), clicked(clicked) {};

		const Button button;
		const bool down;
		const bool clicked;
	};

	class MouseMoveInputEvent : public MouseBaseInputEvent {
	public:
		MouseMoveInputEvent(Math::Vec2i position, Math::Vec2i delta)
			: MouseBaseInputEvent(InputEventType::MOUSE, position), delta(delta) {};

		const Math::Vec2i delta;
	};

	class MouseWheelInputEvent : public MouseBaseInputEvent {
	public:
		MouseWheelInputEvent(const float direction, const Math::Vec2i position)
			: MouseBaseInputEvent(InputEventType::WHEEL, position), direction(direction) {};

		const float direction;
	};

	class WindowResizedInputEvent : public InputEvent {
	public:
		WindowResizedInputEvent(const unsigned int lastWidth, const unsigned int lastHeight, const unsigned int width, const unsigned int height)
			: lastWidth(lastWidth), lastHeight(lastHeight), width(width), height(height) {};

		const unsigned int lastWidth, lastHeight;
		const unsigned int width, height;
	};

	class WindowFocusInputEvent : public InputEvent {
	public:
		WindowFocusInputEvent(const bool focus)
			: focus(focus) {};

		const bool focus;
	};
} }

#endif