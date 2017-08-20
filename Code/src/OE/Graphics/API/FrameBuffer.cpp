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

	void FrameBuffer::blit(FrameBuffer* source, BlitOperation operation)
	{
		if (!source)
			return;

		switch (operation)
		{
		case BlitOperation::DEPTH:
			if (m_DepthTexture) {
				Texture* depthSource = source->getDepthTexture();
				if (depthSource)
					m_DepthTexture->copy(depthSource);
			}
			break;
		}
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
		Prepare();
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

	FrameBuffer* FrameBuffer::GetCurrent()
	{
		Application::priv::ContextImpl* currentContext = Application::priv::ContextImpl::GetCurrent();
		int size = currentContext->m_FrameBufferStack.size();
		if (size != 0)
			return currentContext->m_FrameBufferStack[size - 1];
		return nullptr;
	}

	Math::Vec2i FrameBuffer::GetCurrentSize()
	{
		FrameBuffer* currentFrameBuffer = GetCurrent();
		if (currentFrameBuffer != nullptr) {
			return Math::Vec2i(currentFrameBuffer->m_Width, currentFrameBuffer->m_Height);
		}
		else {
			Application::priv::ContextImpl* currentContext = Application::priv::ContextImpl::GetCurrent();
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