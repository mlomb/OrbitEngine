#include "OE/Platform/OpenGL/GLContext.hpp"

#include "OE/Platform/OpenGL/GLFrameBuffer.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	void GLContext::prepare()
	{
		OE_CHECK_GL(glClearColor(0.25f, 0.5f, 1.0f, 1.0f));
		OE_CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void GLContext::setDefaultBackbuffer()
	{
		Graphics::GLFrameBuffer::Unbind();
		setViewport();
	}

	void GLContext::setViewport()
	{
		glViewport(0.0f, 0.0f, p_Size.w, p_Size.h);
	}

	void GLContext::makeCurrent(bool active)
	{
		ContextImpl::makeCurrent(active);
	}
} } }