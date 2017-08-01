#include "OE/Platform/Direct3D/D3DDepthTexture.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {
	D3DDepthTexture::D3DDepthTexture(TextureProperties properties)
		: D3DTexture(properties, { nullptr }, D3D11_BIND_DEPTH_STENCIL)
	{
		// DSV
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = TextureFormatToDSVD3D(properties.formatProperties.format);
		depthStencilViewDesc.ViewDimension = TextureDimensionToDSVD3D(properties.dimension);
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HRESULT hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateDepthStencilView(m_pResource, &depthStencilViewDesc, &m_DepthStencilView);
		if (FAILED(hr))
			OE_LOG_FATAL("Error creating depth stencil view!");
	}

	D3DDepthTexture::~D3DDepthTexture()
	{
		OE_D3D_RELEASE(m_DepthStencilView);
	}

	D3D11_DSV_DIMENSION D3DDepthTexture::TextureDimensionToDSVD3D(TextureDimension dimension)
	{
		switch (dimension) {
		case TEXTURE1D:
			return D3D11_DSV_DIMENSION_TEXTURE1D;
		case TEXTURE2D:
			return D3D11_DSV_DIMENSION_TEXTURE2D;
		}

		OE_LOG_FATAL("DepthView dimension not supported in D3D.");
		// TODO Assert
		return (D3D11_DSV_DIMENSION)0;
	}

	DXGI_FORMAT D3DDepthTexture::TextureFormatToDSVD3D(TextureFormat format)
	{
		switch (format)
		{
		case DEPTH:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		}

		return TextureFormatToD3D(format);
	}
} }