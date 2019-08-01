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
	Window::Window()
		: Pimpl(new ::WindowImplType())
	{
		setDefaults();
	}

	Window::Window(WindowNativeHandle handle)
#if (OE_WINDOWS || OE_UNIX) && !OE_EMSCRIPTEN
		: Pimpl(new ::WindowImplType(handle))
#endif
	{
		setDefaults();
	}

	Window::~Window()
	{
	}

	void Window::setDefaults()
	{
		setTitle("OrbitEngine");
		setSize(Math::Vec2i(800, 600));
		setDisplayMode(DisplayMode::WINDOWED);
	}

	void Window::processEvents() {
		p_Pimpl->processEvents();
	}

	bool Window::destroyRequested() const {
		return p_Pimpl->destroyRequested();
	}

	Math::Vec2i Window::getCursorPosition() const
	{
		return p_Pimpl->getCursorPosition();
	}

	DisplayMode Window::getDisplayMode() const
	{
		return p_Pimpl->getDisplayMode();
	}

	std::string Window::getTitle() const
	{
		return p_Pimpl->getTitle();
	}

	Math::Vec2i Window::getPosition() const
	{
		return p_Pimpl->getPosition();
	}

	Math::Vec2i Window::getSize() const
	{
		return p_Pimpl->getSize();
	}

	bool Window::isVisible() const
	{
		return p_Pimpl->isVisible();
	}

	bool Window::isFocused() const
	{
		return p_Pimpl->isFocused();
	}

	bool Window::isMinimized() const
	{
		return p_Pimpl->isMinimized();
	}

	bool Window::setDisplayMode(DisplayMode mode)
	{
		return p_Pimpl->setDisplayMode(mode);
	}

	bool Window::setTitle(const std::string& title)
	{
		return p_Pimpl->setTitle(title);
	}

	bool Window::setPosition(const Math::Vec2i& position)
	{
		return p_Pimpl->setPosition(position);
	}

	bool Window::setSize(const Math::Vec2i& size)
	{
		return p_Pimpl->setSize(size);
	}

	bool Window::setVisibility(bool visible)
	{
		return p_Pimpl->setVisibility(visible);
	}

	bool Window::requestFocus()
	{
		return p_Pimpl->requestFocus();
	}

	bool Window::setAlpha(float alpha)
	{
		return p_Pimpl->setAlpha(alpha);
	}

	DisplayNativeHandle Window::getDisplayNativeHandle()
	{
		return p_Pimpl->getDisplayNativeHandle();
	}

	WindowNativeHandle Window::getWindowNativeHandle(){
		return p_Pimpl->getWindowNativeHandle();
	}
} }