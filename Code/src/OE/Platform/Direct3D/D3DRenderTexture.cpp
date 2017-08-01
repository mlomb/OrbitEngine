#include "OE/Platform/Direct3D/D3DRenderTexture.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {
	D3DRenderTexture::D3DRenderTexture(TextureProperties properties)
		: D3DTexture(properties, { nullptr }, D3D11_BIND_RENDER_TARGET)
	{
		// RTV
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = TextureFormatToD3D(properties.formatProperties.format);
		renderTargetViewDesc.ViewDimension = TextureDimensionToRTVD3D(properties.dimension);
		renderTargetViewDesc.Texture1D.MipSlice = 0;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		renderTargetViewDesc.Texture3D.MipSlice = 0;

		HRESULT hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateRenderTargetView(m_pResource, &renderTargetViewDesc, &m_RenderTargetView);
		if (FAILED(hr))
			OE_LOG_FATAL("Error creating render target view!");
	}

	D3DRenderTexture::~D3DRenderTexture()
	{
		OE_D3D_RELEASE(m_RenderTargetView);
	}

	D3D11_RTV_DIMENSION D3DRenderTexture::TextureDimensionToRTVD3D(TextureDimension dimension)
	{
		switch (dimension) {
		case TEXTURE1D:
			return D3D11_RTV_DIMENSION_TEXTURE1D;
		case TEXTURE2D:
			return D3D11_RTV_DIMENSION_TEXTURE2D;
			//case CUBEMAP:
			//	return D3D11_RTV_DIMENSION_TEXTURE3D;
		}

		OE_LOG_FATAL("Are you trying to invent a new dimension?");
		// TODO assert
		return (D3D11_RTV_DIMENSION)0;
	}
} }