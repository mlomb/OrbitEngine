#include "OE/Graphics/API/FrameBuffer.hpp"

#include "OE/Application/ContextImpl.hpp"
#include "OE/Misc/Log.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLFrameBuffer.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DFrameBuffer.hpp"
#endif

namespace OrbitEngine { namespace Graphics {
	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height), m_DepthStencilTexture(NULL), m_MipLevel(0)
	{
	}

	FrameBuffer::~FrameBuffer()
	{
		for (size_t i = 0; i < m_ColorBuffers.size(); i++)
			delete m_ColorBuffers[i];
		m_ColorBuffers.clear();
		if(m_DepthStencilTexture)
			delete m_DepthStencilTexture;
	}

	void FrameBuffer::blit(FrameBuffer* source, BlitOperation operation)
	{
		if (!source)
			return;

		switch (operation)
		{
		case BlitOperation::DEPTH:
			if (m_DepthStencilTexture) {
				Texture* depthSource = source->getDepthTexture();
				if (depthSource)
					m_DepthStencilTexture->copy(depthSource);
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
		int size = int(currentContext->m_FrameBufferStack.size());
		FrameBuffer* currentFrameBuffer = size > 0 ? currentContext->m_FrameBufferStack[size - 1] : currentContext->getDefaultFramebuffer();

		currentFrameBuffer->bind();
		currentFrameBuffer->setViewport();
	}

	FrameBuffer* FrameBuffer::GetCurrent()
	{
		Application::priv::ContextImpl* currentContext = Application::priv::ContextImpl::GetCurrent();
		int size = int(currentContext->m_FrameBufferStack.size());
		if (size != 0)
			return currentContext->m_FrameBufferStack[size - 1];
		return currentContext->getDefaultFramebuffer();
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