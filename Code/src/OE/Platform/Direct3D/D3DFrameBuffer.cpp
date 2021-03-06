#include "OE/Platform/Direct3D/D3DFrameBuffer.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Platform/Direct3D/D3DRenderTexture.hpp"
#include "OE/Platform/Direct3D/D3DDepthTexture.hpp"

#include "OE/Misc/Log.hpp"

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
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->OMSetRenderTargets(0, 0, 0);
	}

	void D3DFrameBuffer::clear() const
	{
		float clearColor[] = { m_ClearColor.x, m_ClearColor.y,m_ClearColor.z,m_ClearColor.w };
		for (size_t i = 0; i < m_RenderTargetViews.size(); i++)
			Application::priv::D3DContext::GetCurrent()->getDeviceContext()->ClearRenderTargetView(m_RenderTargetViews[i], clearColor);
		if (m_DepthStencilView)
			Application::priv::D3DContext::GetCurrent()->getDeviceContext()->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		setViewport();
	}

	void D3DFrameBuffer::attachColorTextures(int count, TextureFormatProperties formatProperties, TextureSampleProperties sampleProperties, TextureDimension dimension)
	{
		TextureProperties properties;
		properties.textureBufferMode = DEFAULT;
		properties.formatProperties = formatProperties;
		properties.sampleProperties = sampleProperties;
		properties.width = m_Width;
		properties.height = m_Height;
		properties.dimension = dimension;

		for (int i = 0; i < count; i++) {
			attachColorTexture(new D3DRenderTexture(properties));
		}
	}

	void D3DFrameBuffer::attachColorTexture(D3DRenderTexture* renderTexture)
	{
		m_ColorBuffers.push_back(renderTexture);
	}

	void D3DFrameBuffer::attachDepthStencilTexture(bool stencil, TextureSampleProperties sampleProperties, TextureDimension dimension)
	{
		TextureProperties properties;
		properties.textureBufferMode = DEFAULT;
		// TODO: Check those in D3D
		properties.formatProperties.dataType = stencil ? TextureDataType::UNSIGNED_INT : TextureDataType::UNSIGNED_BYTE;
		properties.formatProperties.format = stencil ? TextureFormat::DEPTH_STENCIL : TextureFormat::DEPTH;
		properties.sampleProperties = sampleProperties;
		properties.width = m_Width;
		properties.height = m_Height;
		properties.dimension = dimension;

		D3DDepthTexture* depthTex = new D3DDepthTexture(properties);
		m_DepthStencilView = depthTex->getDSV();
		m_DepthStencilTexture = depthTex;
	}

	void D3DFrameBuffer::finalize()
	{
		m_RenderTargetViews.clear();
		for (size_t i = 0; i < m_ColorBuffers.size(); i++)
			m_RenderTargetViews.push_back(((D3DRenderTexture*)m_ColorBuffers[i])->getRTV());
	}
} }