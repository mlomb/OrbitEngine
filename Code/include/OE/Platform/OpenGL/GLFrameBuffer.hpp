#ifndef GRAPHICS_GLFRAME_BUFFER_HPP
#define GRAPHICS_GLFRAME_BUFFER_HPP

#include "OE/Graphics/API/FrameBuffer.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine { namespace Graphics {
	class GLFrameBuffer : public FrameBuffer {
	public:
		GLFrameBuffer(GLuint fbo);
		GLFrameBuffer(unsigned int width, unsigned int height);
		~GLFrameBuffer();

		void attachColorTextures(int count, TextureFormatProperties formatProperties, TextureSampleProperties sampleProperties, TextureDimension dimension) override;
		void attachDepthStencilTexture(bool stencil, TextureSampleProperties sampleProperties, TextureDimension dimension) override;
		void finalize() override;

		void bind() const override;
		void setViewport() const override;
		void useMip(unsigned int mipLevel) override;
		void unbind() const override;
		void clear() const override;
		void blit(FrameBuffer* source, BlitOperation operation);

		inline GLuint getID() { return m_FBO; };

		static void Unbind();
	private:
		GLuint m_FBO;
	};
} }

#endif