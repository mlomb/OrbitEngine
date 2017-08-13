#include "OE/Platform/Direct3D/D3DTexture.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Platform/Direct3D/D3DBuffer.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Math/Math.hpp"

namespace OrbitEngine { namespace Graphics {

	D3DTexture::D3DTexture(TextureProperties properties, std::vector<void*> data, unsigned int additionalBindFlags) : Texture(properties)
	{
		if (data.size() == 0) {
			OE_LOG_WARNING("You must provide at least one pointer of data, it can be nullptr.");
			return;
		}

		HRESULT hr;
		if (!data[0]) {
			m_Properties.formatProperties.mipmapping = false;
			m_MipLevels = 1;
		}

		if (m_Properties.formatProperties.mipmapping || m_Properties.dimension == CUBEMAP)
			m_Properties.textureBufferMode = DEFAULT;

		switch (m_Properties.dimension) {
		case TEXTURE1D:
			OE_LOG_FATAL("Texture1D not implemented yet!");
			break;
		case CUBEMAP:
		case TEXTURE2DARRAY:
		case TEXTURE2D:
			D3D11_TEXTURE2D_DESC texDesc;
			texDesc.Width = m_Properties.formatProperties.width;
			texDesc.Height = m_Properties.formatProperties.height;
			texDesc.Format = TextureFormatToD3D(m_Properties.formatProperties.format);
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | additionalBindFlags;
			texDesc.Usage = D3DBuffer::BufferModeToD3D(m_Properties.textureBufferMode);
			texDesc.CPUAccessFlags = m_Properties.textureBufferMode == DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
			texDesc.ArraySize = data.size();
			texDesc.MipLevels = m_MipLevels;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.MiscFlags = 0;
			if (m_Properties.formatProperties.mipmapping) {
				texDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
				texDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			}
			if (m_Properties.dimension == CUBEMAP)
				texDesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

			ID3D11Texture2D* pTexture;
			hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateTexture2D(&texDesc, nullptr, &pTexture);
			m_pResource = pTexture;
			break;
		}

		if (FAILED(hr))
			OE_LOG_FATAL("Error creating D3DTexture!");

		// SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = TextureFormatToSRVD3D(m_Properties.formatProperties.format);
		srvDesc.Texture2D.MipLevels = m_MipLevels;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.ViewDimension = TextureDimensionToSRVD3D(m_Properties.dimension);

		hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateShaderResourceView(m_pResource, &srvDesc, &m_ResourceView);
		if (FAILED(hr))
			OE_LOG_FATAL("Error creating texture SRV!");

		for (unsigned int i = 0; i < data.size(); i++)
			if (data[i])
				setData(data[i], D3D11CalcSubresource(0, i, m_MipLevels));
		if (m_Properties.formatProperties.mipmapping)
			Application::priv::D3DContext::GetCurrent()->getDeviceContext()->GenerateMips(m_ResourceView);

		// Move to D3DRenderer
		// Sampler
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = TextureFilterToD3D(m_Properties.sampleProperties.filter);
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = Math::clamp(D3D11_DEFAULT_MAX_ANISOTROPY, 0, 8);
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateSamplerState(&samplerDesc, &m_SamplerState);
		if (FAILED(hr))
			OE_LOG_FATAL("Error creating sampler state!");
	}

	D3DTexture::~D3DTexture()
	{
		OE_D3D_RELEASE(m_SamplerState);
		OE_D3D_RELEASE(m_ResourceView);
	}

	void D3DTexture::bind(unsigned int slot)
	{
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->PSSetShaderResources(slot, 1, &m_ResourceView);
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	void D3DTexture::Unbind(const unsigned int slot)
	{
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->PSSetShaderResources(slot, 1, nullSRV);
	}

	void D3DTexture::setData(void* data, unsigned int offset)
	{
		unsigned int stride = (m_Properties.formatProperties.bpp / 8);
		switch (m_Properties.textureBufferMode) {
		case DYNAMIC:
			D3DMappedResource::setData(m_Properties.formatProperties.width * m_Properties.formatProperties.height * stride, data);
			break;
		case DEFAULT:
			Application::priv::D3DContext::GetCurrent()->getDeviceContext()->UpdateSubresource(m_pResource, offset, 0, data, m_Properties.formatProperties.width * stride, m_Properties.formatProperties.width * m_Properties.formatProperties.height * stride);
			break;
		}
	}

	void D3DTexture::copy(Texture* source)
	{
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->CopyResource(m_pResource, ((D3DTexture*)source)->getD3DResource());
	}

	D3D11_FILTER D3DTexture::TextureFilterToD3D(TextureFilter filter)
	{
		switch (filter)
		{
		case ANISOTROPIC:
			return D3D11_FILTER_ANISOTROPIC;
		case LINEAR:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		case NEAREST:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		}

		OE_LOG_FATAL("Unsupported texture filter (" + std::to_string(filter) + ") in D3D.");

		return (D3D11_FILTER)0;
	}

	DXGI_FORMAT D3DTexture::TextureFormatToD3D(TextureFormat format)
	{
		switch (format)
		{
		case SRC_ALPHA:
		case LUMINANCE_ALPHA:
		case R8:
			return DXGI_FORMAT_R8_UNORM;
		case R16:
			return DXGI_FORMAT_R16_UNORM;
		case RG8:
			return DXGI_FORMAT_R8G8_SINT;
		case RGB: // 24 bit formats are not longer supported since DX11
		case RGB8:
		case RGBA:
		case RGBA8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case RGB16F:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case RGBA32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case DEPTH:
			return DXGI_FORMAT_R24G8_TYPELESS;
		}

		OE_LOG_FATAL("Unsupported texture format (" + std::to_string(format) + ") in D3D.");

		return DXGI_FORMAT_UNKNOWN;
	}

	DXGI_FORMAT D3DTexture::TextureFormatToSRVD3D(TextureFormat format)
	{
		switch (format)
		{
		case DEPTH:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}

		return TextureFormatToD3D(format);
	}

	D3D_SRV_DIMENSION D3DTexture::TextureDimensionToSRVD3D(TextureDimension dimension)
	{
		switch (dimension) {
		case TEXTURE1D:
			return D3D11_SRV_DIMENSION_TEXTURE1D;
		case TEXTURE2D:
			return D3D11_SRV_DIMENSION_TEXTURE2D;
		case CUBEMAP:
			return D3D11_SRV_DIMENSION_TEXTURECUBE;
		}

		OE_LOG_FATAL("Are you trying to invent a new dimension?");
		// TODO Assert
		return (D3D_SRV_DIMENSION)0;
	}
} }