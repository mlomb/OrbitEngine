#ifndef APPLICATION_WINDOW_LINUX_HPP
#define APPLICATION_WINDOW_LINUX_HPP

#include "OE/Application/Window.hpp"
#include "OE/Application/WindowImpl.hpp"
#include "OE/Application/Context.hpp"

#include "OE/Platform/X11/X11.hpp"
#include "OE/Platform/X11/X11Display.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	class WindowLinux : public WindowImpl {
	public:
		WindowLinux();
		WindowLinux(WindowNativeHandle handle);
		~WindowLinux();

		void processEvents() override;
		bool userInterrupt();
		bool isMinimized() const override;

		bool setDisplayMode(DisplayMode mode) override;
		bool setCursor(Cursor cursor) override;
		bool setTitle(const std::string& title) override;
		bool setPosition(const Math::Vec2i& position) override;
		bool setSize(const Math::Vec2i& size) override;
		bool setVisibility(bool visible) override;
		bool requestFocus() override;
		bool setAlpha(float alpha) override;
		
		WindowNativeHandle getWindowNativeHandle() override;
		DisplayNativeHandle getDisplayNativeHandle() override;
		XVisualInfo* getVisualInfo();

	private:
		static Key TranslateKey(int key);

		void loadAtoms();

		Atom m_ATOM_WM_DELETE_WINDOW;
		bool m_Grabbed;

		::Display* m_Display;
		::Window m_Window;
		::XVisualInfo* m_VisualInfo;
	};
} } }

#endif