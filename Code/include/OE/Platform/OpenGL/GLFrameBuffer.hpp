#ifndef GRAPHICS_GLFRAME_BUFFER_HPP
#define GRAPHICS_GLFRAME_BUFFER_HPP

#include "OE/Graphics/API/FrameBuffer.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine { namespace Graphics {
	class GLFrameBuffer : public FrameBuffer {
	public:
		GLFrameBuffer(unsigned int width, unsigned int height);
		~GLFrameBuffer();

		void attachColorTexture(Graphics::TextureFormatProperties format, int count = 1) override;
		void attachColorCubemap(Graphics::TextureFormatProperties format, int count = 1) override;
		void attachDepthTexture(Graphics::TextureFormatProperties format) override;
		void attachDepthCubemap(Graphics::TextureFormatProperties format) override;
		void finalize() override;

		void bind() const override;
		void setViewport() const override;
		void useMip(unsigned int mipLevel) override;
		void unbind() const override;
		void clear() const override;

		inline GLuint getID() { return m_FBO; };

		static void Unbind();
	private:
		GLuint m_FBO;
	};
} }

#endif