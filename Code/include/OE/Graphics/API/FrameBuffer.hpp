#ifndef GRAPHICS_FRAME_BUFFER_HPP
#define GRAPHICS_FRAME_BUFFER_HPP

#include "OE/Graphics/API/Texture.hpp"

#include "OE/Misc/BaseStack.hpp"

#include "OE/Math/Mat4.hpp"

namespace OrbitEngine {
	namespace Application { namespace priv {
		class GLContext;
	} }

	namespace Graphics {

	enum class BlitOperation {
		DEPTH
	};
	class FrameBuffer {
	public:
		virtual ~FrameBuffer();

		static void Push(FrameBuffer* frameBuffer);
		static void Pop();
		static void Prepare();
		static FrameBuffer* GetCurrent();
		static Math::Vec2i GetCurrentSize();

		static FrameBuffer* Create(unsigned int width, unsigned int height);

		virtual void attachColorTextures(int count, TextureFormatProperties formatProperties, TextureSampleProperties sampleProperties = TextureSampleProperties(), TextureDimension dimension = TEXTURE2D) = 0;
		virtual void attachDepthStencilTexture(bool stencil = false, TextureSampleProperties sampleProperties = TextureSampleProperties(), TextureDimension dimension = TEXTURE2D) = 0;
		//virtual void attachRenderBuffer(API::TextureFormat format, int count = 1) = 0;

		virtual void finalize() { };

		virtual void setViewport() const = 0;
		virtual void useMip(unsigned int mipLevel) = 0;
		virtual void blit(FrameBuffer* source, BlitOperation operation);
		virtual void clear() const = 0;

		inline void setClearColor(const Math::Vec4f &clearColor) { m_ClearColor = clearColor; };
		inline unsigned int getWidth() { return m_Width; };
		inline unsigned int getHeight() { return m_Height; };
		inline std::vector<Texture*> getColorTextures() const { return m_ColorBuffers; };
		inline Texture* getDepthTexture() const { return m_DepthStencilTexture; };
	protected:
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		FrameBuffer(unsigned int width, unsigned int height);

		std::vector<Texture*> m_ColorBuffers;
		Texture* m_DepthStencilTexture;

		unsigned int m_Width, m_Height, m_MipLevel;
		Math::Vec4f m_ClearColor;

		// GLContext may change the default buffer's dimensions
		friend class Application::priv::GLContext;
	};
} }

#endif