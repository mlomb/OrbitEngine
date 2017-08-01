#include "OE/Application/Window.hpp"

#include "OE/Application/Context.hpp"

#if OE_WINDOWS
	#include "OE/Platform/Windows/WindowWindows.hpp"
	typedef OrbitEngine::Application::priv::WindowWindows WindowImplType;
#elif OE_ANDROID
	#include "OE/Platform/Android/WindowAndroid.hpp"
	typedef OrbitEngine::Application::priv::WindowAndroid WindowImplType;
#elif OE_EMSCRIPTEN
	#include "OE/Platform/Emscripten/WindowEmscripten.hpp"
	typedef OrbitEngine::Application::priv::WindowEmscripten WindowImplType;
#elif OE_UNIX
	#include "OE/Platform/X11/WindowLinux.hpp"
	typedef OrbitEngine::Application::priv::WindowLinux WindowImplType;
#endif

namespace OrbitEngine { namespace Application {
	Window::Window(WindowProperties windowProperties)
		: Pimpl(new ::WindowImplType(windowProperties))
	{
	}

	Window::~Window()
	{
	}

	void Window::processEvents() {
		p_Pimpl->processEvents();
	}

	bool Window::destroyRequested() {
		return p_Pimpl->destroyRequested();
	}

	DisplayNativeHandle Window::getDisplayNativeHandle()
	{
		return p_Pimpl->getDisplayNativeHandle();
	}

	WindowNativeHandle Window::getWindowNativeHandle(){
		return p_Pimpl->getWindowNativeHandle();
	}

	WindowProperties& Window::getProperties() {
		return p_Pimpl->getProperties();
	}

	InputManager* Window::getInputManager()
	{
		return p_Pimpl->getInputManager();
	}
} }