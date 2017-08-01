#include "OE/Graphics/API/FrameBuffer.hpp"

#include "OE/Application/ContextImpl.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLFrameBuffer.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DFrameBuffer.hpp"
#endif

namespace OrbitEngine { namespace Graphics {
	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height)
	{
	}

	FrameBuffer::~FrameBuffer()
	{
		for (size_t i = 0; i < m_ColorBuffers.size(); i++)
			delete m_ColorBuffers[i];
		m_ColorBuffers.clear();
		delete m_DepthTexture;
	}

	void FrameBuffer::Push(FrameBuffer* frameBuffer)
	{
		Application::priv::ContextImpl* currentContext = Application::priv::ContextImpl::GetCurrent();
		currentContext->m_FrameBufferStack.push_back(frameBuffer);
	}

	void FrameBuffer::Pop()
	{
		Application::priv::ContextImpl* currentContext = Application::priv::ContextImpl::GetCurrent();
		currentContext->m_FrameBufferStack.pop_back();
	}

	void FrameBuffer::Prepare()
	{
		Application::priv::ContextImpl* currentContext = Application::priv::ContextImpl::GetCurrent();
		int size = currentContext->m_FrameBufferStack.size();
		if (size != 0) {
			FrameBuffer* currentFrameBuffer = currentContext->m_FrameBufferStack[size - 1];
			currentFrameBuffer->bind();
			currentFrameBuffer->setViewport();
		}
		else {
			currentContext->setDefaultBackbuffer();
		}
	}

	Math::Vec2i FrameBuffer::GetCurrentSize()
	{
		Application::priv::ContextImpl* currentContext = Application::priv::ContextImpl::GetCurrent();
		int size = currentContext->m_FrameBufferStack.size();
		if (size != 0) {
			FrameBuffer* currentFrameBuffer = currentContext->m_FrameBufferStack[size - 1];
			return Math::Vec2i(currentFrameBuffer->m_Width, currentFrameBuffer->m_Height);
		}
		else {
			return currentContext->GetCurrent()->p_Size;
		}
	}

	FrameBuffer* FrameBuffer::Create(unsigned int width, unsigned int height)
	{
		OE_LOG_DEBUG("Creating FrameBuffer: " << width << "x" << height);

		switch (Application::Context::GetCurrentAPI())
		{
#if OE_OPENGL_ANY
		case RenderAPI::OPENGL:
#if OE_OPENGL_ES
		case RenderAPI::OPENGL_ES:
#endif
			return new GLFrameBuffer(width, height);
#endif
#if OE_D3D
		case RenderAPI::DIRECT3D:
			return new D3DFrameBuffer(width, height);
#endif
		}

		return 0;
	}
} }