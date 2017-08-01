#ifndef GRAPHICS_D3DRENDER_TEXTURE_HPP
#define GRAPHICS_D3DRENDER_TEXTURE_HPP

#include "OE/Platform/Direct3D/D3DTexture.hpp"

namespace OrbitEngine { namespace Graphics {
	class D3DRenderTexture : public D3DTexture {
	public:
		D3DRenderTexture(TextureProperties properties);
		~D3DRenderTexture();

		inline ID3D11RenderTargetView* getRTV() { return m_RenderTargetView; };

		static D3D11_RTV_DIMENSION TextureDimensionToRTVD3D(TextureDimension dimension);
	private:
		ID3D11RenderTargetView* m_RenderTargetView;
	};
} }

#endif