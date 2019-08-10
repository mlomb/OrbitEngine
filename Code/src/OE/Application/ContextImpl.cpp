#include "OE/Application/ContextImpl.hpp"

#include "OE/Application/WindowImpl.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"

#ifdef OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLStates.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DStates.hpp"
#endif

namespace OrbitEngine {	namespace Application { namespace priv {

	ContextImpl* ContextImpl::s_CurrentContext = 0;

	ContextImpl::ContextImpl(WindowImpl* window)
		: p_DefaultFramebuffer(NULL), m_States(NULL), m_Wrapper(NULL)
	{
		// If the context belongs to a Window, here we assign
		// it that a Context was created for it
		// Find a better solution?
		p_Window = window;
		if (p_Window) {
			OE_ASSERT_MSG(p_Window->p_Context == 0, This window already have a context.)

			p_Window->p_Context = this;
		}
	}

	void ContextImpl::contextInitialized()
	{
		p_DefaultFramebuffer->setClearColor(Math::Vec4f(0.25f, 0.5f, 1.0f, 1.0));
	}

	ContextImpl::~ContextImpl(){
		if (p_Window && p_Window->p_Context == this)
			p_Window->p_Context = 0;
		if (p_DefaultFramebuffer)
			delete p_DefaultFramebuffer;
		if (m_States)
			delete m_States;
	}

	void ContextImpl::makeCurrent(bool active)
	{
		if (active)
			s_CurrentContext = this;
		else
			s_CurrentContext = NULL;
	}

	void ContextImpl::resizeContext(Math::Vec2i size)
	{
		p_Size = size;
	}

	WindowImpl* ContextImpl::getWindowImpl()
	{
		return p_Window;
	}

	Graphics::States* ContextImpl::getGlobalStates()
	{
		// TODO We should move this to the constructor
		if (m_States == NULL) {
			switch (GetCurrentAPI()) {
#ifdef OE_OPENGL_ANY
			case OPENGL:
#if OE_OPENGL_ES
			case OPENGL_ES:
#endif
				m_States = new Graphics::GLStates();
				break;
#endif
#if OE_D3D
			case DIRECT3D:
				m_States = new Graphics::D3DStates();
				break;
#endif
			}
		}

		return m_States;
	}

	Graphics::FrameBuffer* ContextImpl::getDefaultFramebuffer()
	{
		return p_DefaultFramebuffer;
	}

	ContextImpl* ContextImpl::GetCurrent()
	{
		return s_CurrentContext;
	}

	RenderAPI ContextImpl::GetCurrentAPI()
	{
		ContextImpl* current = GetCurrent();
		return current ? current->getAPI() : RenderAPI::NONE;
	}
} } }