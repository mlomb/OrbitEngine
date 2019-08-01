#include "OE/Application/WindowImpl.hpp"

#include "OE/Application/InputManager.hpp"

#include "OE/Application/ContextImpl.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	
	WindowImpl::WindowImpl()
		: p_Visible(false), p_Focused(false)
	{
	}

	WindowImpl::~WindowImpl(){
		if (p_Context)
			delete p_Context;
	}
	
	bool WindowImpl::destroyRequested() const {
		return m_DestroyRequested;
	}

	void WindowImpl::processEvents()
	{
	}
	
	void WindowImpl::requestDestroy() {
		m_DestroyRequested = true;
	}

	Math::Vec2i WindowImpl::getCursorPosition() const
	{
		Math::Vec2i absolute_position = InputManager::Get()->getCursorPosition();
		return Math::Vec2i(
			absolute_position.x - p_Position.x,
			absolute_position.y - p_Position.y
		);
	}

	DisplayMode WindowImpl::getDisplayMode() const
	{
		return p_DisplayMode;
	}

	Math::Vec2i WindowImpl::getPosition() const
	{
		return p_Position;
	}

	Math::Vec2i WindowImpl::getSize() const
	{
		return p_Size;
	}

	bool WindowImpl::isVisible() const
	{
		return p_Visible;
	}

	bool WindowImpl::isFocused() const
	{
		return p_Focused;
	}
} } }