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
		WindowLinux(WindowProperties properties);
		~WindowLinux();

		void processEvents() override;
		void setTitle(const char* title) override;
		bool userInterrupt();

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