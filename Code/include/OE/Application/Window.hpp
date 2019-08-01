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
		OVERLAPPED,
		DUMMY
	};

	class Window : public Misc::Pimpl<priv::WindowImpl> {
	public:
		Window();
		Window(WindowNativeHandle handle);
		~Window();

		void processEvents();

		bool destroyRequested() const;
		Math::Vec2i getCursorPosition() const;
		DisplayMode getDisplayMode() const;
		std::string getTitle() const;
		Math::Vec2i getPosition() const;
		Math::Vec2i getSize() const;
		bool isVisible() const;
		bool isFocused() const;
		bool isMinimized() const;

		bool setDisplayMode(DisplayMode mode);
		bool setTitle(const std::string& title);
		bool setPosition(const Math::Vec2i& position);
		bool setSize(const Math::Vec2i& size);
		bool setVisibility(bool visible);
		bool requestFocus();
		bool setAlpha(float alpha);

		DisplayNativeHandle getDisplayNativeHandle();
		WindowNativeHandle getWindowNativeHandle();
	private:
		void setDefaults();
	};
} }

#endif