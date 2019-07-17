#ifndef GRAPHICS_D3DFRAME_BUFFER_HPP
#define GRAPHICS_D3DFRAME_BUFFER_HPP

#include "OE/Graphics/API/FrameBuffer.hpp"

#include "OE/Platform/Direct3D/Direct3D.hpp"

namespace OrbitEngine { namespace Graphics {
	class D3DRenderTexture;

	class D3DFrameBuffer : public FrameBuffer {
	public:
		D3DFrameBuffer(unsigned int width, unsigned int height);
		~D3DFrameBuffer();

		void attachColorTexture(TextureFormatProperties format, int count = 1) override;
		void attachColorCubemap(TextureFormatProperties format, int count = 1) override;
		void attachDepthTexture(TextureFormatProperties format) override;
		void attachDepthCubemap(TextureFormatProperties format) override;
		void attachDepthStencilTexture(TextureFormatProperties format) override;
		void finalize() override;

		// D3D-specific
		void attachColorTexture(D3DRenderTexture* renderTexture);

		void bind() const override;
		void setViewport() const override;
		void useMip(unsigned int mipLevel) override;
		void unbind() const override;
		void clear() const override;
	private:
		D3D11_VIEWPORT m_Viewport;

		std::vector<ID3D11RenderTargetView*> m_RenderTargetViews;
		ID3D11DepthStencilView* m_DepthStencilView;
	};
} }

#endif