#include "OE/Application/WindowImpl.hpp"

#include "OE/Application/InputManager.hpp"

#include "OE/Application/ContextImpl.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	
	WindowImpl::WindowImpl(WindowProperties properties)
	{
		p_Properties = properties;

		p_InputManager = new InputManager(this);
	}

	WindowImpl::~WindowImpl(){
		if (p_Context)
			delete p_Context;
	}
	
	bool WindowImpl::destroyRequested() {
		return m_DestroyRequested;
	}

	void WindowImpl::processEvents()
	{
		p_InputManager->update();
	}
	
	void WindowImpl::requestDestroy() {
		m_DestroyRequested = true;
	}

	void WindowImpl::requestCursorMode(const CursorMode cursorMode)
	{
		p_InputManager->m_CursorMode = cursorMode;
	}

	WindowProperties& WindowImpl::getProperties()
	{
		return p_Properties;
	}

	InputManager* WindowImpl::getInputManager()
	{
		return p_InputManager;
	}
} } }