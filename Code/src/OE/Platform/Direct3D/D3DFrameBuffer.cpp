#include "OE/Platform/Direct3D/D3DFrameBuffer.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Platform/Direct3D/D3DRenderTexture.hpp"
#include "OE/Platform/Direct3D/D3DDepthTexture.hpp"

namespace OrbitEngine { namespace Graphics {
	D3DFrameBuffer::D3DFrameBuffer(unsigned int width, unsigned int height)
		: FrameBuffer(width, height),
		m_DepthStencilView(0)
	{
		m_Viewport.Width = (float)(width);
		m_Viewport.Height = (float)(height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
	}

	D3DFrameBuffer::~D3DFrameBuffer()
	{
		for (size_t i = 0; i < m_ColorBuffers.size(); i++)
			delete m_ColorBuffers[i];
		m_ColorBuffers.clear();
	}

	void D3DFrameBuffer::bind() const
	{
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->OMSetRenderTargets(m_ColorBuffers.size(), &m_RenderTargetViews[0], m_DepthStencilView ? m_DepthStencilView : NULL);
	}

	void D3DFrameBuffer::setViewport() const
	{
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->RSSetViewports(1, &m_Viewport);
	}

	void D3DFrameBuffer::useMip(unsigned int mipLevel)
	{
	}

	void D3DFrameBuffer::unbind() const
	{
		Application::priv::D3DContext::GetCurrent()->setDefaultBackbuffer();
	}

	void D3DFrameBuffer::clear() const
	{
		float clearColor[] = { m_ClearColor.x, m_ClearColor.y,m_ClearColor.z,m_ClearColor.w };
		for (size_t i = 0; i < m_RenderTargetViews.size(); i++)
			Application::priv::D3DContext::GetCurrent()->getDeviceContext()->ClearRenderTargetView(m_RenderTargetViews[i], clearColor);
		if (m_DepthStencilView)
			Application::priv::D3DContext::GetCurrent()->getDeviceContext()->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void D3DFrameBuffer::attachColorTexture(Graphics::TextureFormatProperties format, int count)
	{
		TextureProperties properties;
		properties.textureBufferMode = DEFAULT;
		properties.formatProperties = format;
		properties.formatProperties.width = m_Width;
		properties.formatProperties.height = m_Height;
		properties.dimension = TEXTURE2D;

		for (int i = 0; i < count; i++) {
			D3DRenderTexture* colorTexture = new D3DRenderTexture(properties);
			m_ColorBuffers.push_back(colorTexture);
		}
	}

	void D3DFrameBuffer::attachColorCubemap(Graphics::TextureFormatProperties format, int count)
	{
	}

	void D3DFrameBuffer::attachDepthTexture(Graphics::TextureFormatProperties format)
	{
		TextureProperties properties;
		properties.textureBufferMode = DEFAULT;
		properties.formatProperties = format;
		properties.formatProperties.width = m_Width;
		properties.formatProperties.height = m_Height;
		properties.dimension = TEXTURE2D;

		D3DDepthTexture* depthTex = new D3DDepthTexture(properties);
		m_DepthStencilView = depthTex->getDSV();
		m_DepthTexture = depthTex;
	}

	void D3DFrameBuffer::attachDepthCubemap(Graphics::TextureFormatProperties format)
	{
	}

	void D3DFrameBuffer::finalize()
	{
		m_RenderTargetViews.clear();
		for (size_t i = 0; i < m_ColorBuffers.size(); i++)
			m_RenderTargetViews.push_back(((D3DRenderTexture*)m_ColorBuffers[i])->getRTV());
	}
} }