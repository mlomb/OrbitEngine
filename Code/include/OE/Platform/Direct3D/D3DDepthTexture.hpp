#ifndef GRAPHICS_D3DDEPTH_TEXTURE_HPP
#define GRAPHICS_D3DDEPTH_TEXTURE_HPP

#include "OE/Platform/Direct3D/D3DTexture.hpp"

namespace OrbitEngine { namespace Graphics {
	class D3DDepthTexture : public D3DTexture {
	public:
		D3DDepthTexture(TextureProperties properties);
		~D3DDepthTexture();

		static D3D11_DSV_DIMENSION TextureDimensionToDSVD3D(TextureDimension dimension);
		static DXGI_FORMAT TextureFormatToDSVD3D(TextureFormat format);

		inline ID3D11DepthStencilView* getDSV() { return m_DepthStencilView; };
	private:
		ID3D11DepthStencilView* m_DepthStencilView;
	};
} }

#endif