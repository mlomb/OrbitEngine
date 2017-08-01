#ifndef GRAPHICS_FRAME_BUFFER_HPP
#define GRAPHICS_FRAME_BUFFER_HPP

#include "OE/Graphics/API/Texture.hpp"

#include "OE/Misc/BaseStack.hpp"

#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Graphics {
	class FrameBuffer {
	public:
		~FrameBuffer();

		static void Push(FrameBuffer* frameBuffer);
		static void Pop();
		static void Prepare();
		static Math::Vec2i GetCurrentSize();

		static FrameBuffer* Create(unsigned int width, unsigned int height);

		virtual void attachColorTexture(Graphics::TextureFormatProperties format, int count = 1) = 0;
		virtual void attachColorCubemap(Graphics::TextureFormatProperties format, int count = 1) = 0;
		virtual void attachDepthTexture(Graphics::TextureFormatProperties format) = 0;
		virtual void attachDepthCubemap(Graphics::TextureFormatProperties format) = 0;
		//virtual void attachRenderBuffer(API::TextureFormat format, int count = 1) = 0;

		virtual void finalize() { };

		virtual void setViewport() const = 0;
		virtual void useMip(unsigned int mipLevel) = 0;
		virtual void clear() const = 0;

		inline void setClearColor(const Math::Vec4f &clearColor) { m_ClearColor = clearColor; };
		inline unsigned int getWidth() { return m_Width; };
		inline unsigned int getHeight() { return m_Height; };
		inline std::vector<Graphics::Texture*> getColorTextures() const { return m_ColorBuffers; };
		inline Graphics::Texture* getDepthTexture() const { return m_DepthTexture; };
	protected:
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		FrameBuffer(unsigned int width, unsigned int height);

		std::vector<Graphics::Texture*> m_ColorBuffers;
		Graphics::Texture* m_DepthTexture = 0;

		unsigned int m_Width, m_Height, m_MipLevel;
		Math::Vec4f m_ClearColor;
	};
} }

#endif