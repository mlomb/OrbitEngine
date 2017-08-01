#ifndef APPLICATION_WINDOW_HPP
#define APPLICATION_WINDOW_HPP

#include <string>

#include "OE/Application/Context.hpp"
#include "OE/Application/NativeHandlers.hpp"
#include "OE/Misc/Pimpl.hpp"
#include "OE/Math/Vec2.hpp"

namespace OrbitEngine {	namespace Application {
	namespace priv {
		class WindowImpl;
	}
	class InputManager;

	enum DisplayMode {
		WINDOWED = 0,
		WINDOWED_TOOL,
		BORDERLESS,
		FULLSCREEN,
		FULLSCREEN_BORDERLESS,
		DUMMY
	};

	struct WindowProperties {
		std::string title = "OrbitEngine";
		Math::Vec2i resolution = Math::Vec2i(800, 600);
		DisplayMode displayMode = WINDOWED;
	};

	class Window : public Misc::Pimpl<priv::WindowImpl> {
	public:
		Window(WindowProperties windowProperties);
		~Window();

		DisplayNativeHandle getDisplayNativeHandle();
		WindowNativeHandle getWindowNativeHandle();
		void processEvents();
		bool destroyRequested();
		Context* getContext();
		WindowProperties& getProperties();
		InputManager* getInputManager();

	private:

	};
} }

#endif