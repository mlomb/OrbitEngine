#ifndef GRAPHICS_D3DTEXTURE_HPP
#define GRAPHICS_D3DTEXTURE_HPP

#include "OE/Graphics/API/Texture.hpp"

#include "OE/Platform/Direct3D/D3DMappedResource.hpp"

namespace OrbitEngine { namespace Graphics {
	class D3DTexture : public Texture, public D3DMappedResource {
	public:
		D3DTexture(TextureProperties properties, std::vector<void*> data, unsigned int additionalBindFlags = 0);
		~D3DTexture();

		void bind(unsigned int slot) override;
		void setData(void* data, unsigned int offset = 0) override;
		void copy(Texture* source) override;

		static void Unbind(const unsigned int slot);

		inline ID3D11Texture2D* getD3DTexture() const { return static_cast<ID3D11Texture2D*>(m_pResource); };

		static D3D11_FILTER TextureFilterToD3D(TextureFilter filter);
		static DXGI_FORMAT TextureFormatToD3D(TextureFormat format);
		static DXGI_FORMAT TextureFormatToSRVD3D(TextureFormat format);
		static D3D_SRV_DIMENSION TextureDimensionToSRVD3D(TextureDimension dimension);
	private:
		ID3D11SamplerState* m_SamplerState;
		ID3D11ShaderResourceView* m_ResourceView;
	};
} }

#endif