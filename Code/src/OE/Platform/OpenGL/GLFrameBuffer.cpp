#include "OE/Platform/OpenGL/GLFrameBuffer.hpp"

#include <math.h>

#include "OE/Platform/OpenGL/GLTexture.hpp"

namespace OrbitEngine { namespace Graphics {
	GLFrameBuffer::GLFrameBuffer(unsigned int width, unsigned int height)
		: FrameBuffer(width, height)
	{
		glGenFramebuffers(1, &m_FBO);

		bind();

		glDrawBuffers(0, 0);
		glReadBuffer(GL_NONE);

		unbind();
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_FBO);
	}

	void GLFrameBuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void GLFrameBuffer::setViewport() const
	{
		GLsizei width = (int)(m_Width * powf(0.5f, (float)m_MipLevel));
		GLsizei height = (int)(m_Height * powf(0.5f, (float)m_MipLevel));
		glViewport(0, 0, width, height);
	}

	void GLFrameBuffer::useMip(unsigned int mipLevel)
	{
		m_MipLevel = mipLevel;
		setViewport();

		for (size_t i = 0; i < m_ColorBuffers.size(); i++) {
			Graphics::GLTexture* glTexture = (Graphics::GLTexture*)m_ColorBuffers[i];

			switch (glTexture->getProperties().dimension)
			{
			case TEXTURE2D:
				glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)(GL_COLOR_ATTACHMENT0 + i), GL_TEXTURE_2D, glTexture->getID(), mipLevel);
				break;
			case CUBEMAP:
				//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, glTexture->getID(), mipLevel);
				break;
			}
		}
	}

	void GLFrameBuffer::unbind() const
	{
		Unbind();
	}

	void GLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFrameBuffer::clear() const
	{
		bind();
		glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLFrameBuffer::blit(FrameBuffer* source, BlitOperation operation)
	{
		GLbitfield mask = 0;

		switch (operation) {
		case BlitOperation::DEPTH:
			mask = GL_DEPTH_BUFFER_BIT;
			break;
		}

		if (mask == 0) {
			FrameBuffer::blit(source, operation);
			return;
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER, ((GLFrameBuffer*)source)->getID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
		glBlitFramebuffer(0, 0, source->getWidth(), source->getHeight(), 0, 0, m_Width, m_Height, mask, GL_NEAREST);
	}

	void GLFrameBuffer::attachColorTexture(Graphics::TextureFormatProperties format, int count)
	{
		bind();

		TextureProperties properties;
		properties.formatProperties = format;
		properties.formatProperties.width = m_Width;
		properties.formatProperties.height = m_Height;

		for (int i = 0; i < count; i++) {
			GLTexture* colorTexture = static_cast<GLTexture*>(Texture::Create(properties));
			m_ColorBuffers.push_back(colorTexture);
		}

		unbind();
	}

	void GLFrameBuffer::attachColorCubemap(Graphics::TextureFormatProperties format, int count)
	{
		bind();

		TextureProperties properties;
		properties.dimension = CUBEMAP;
		properties.formatProperties = format;
		properties.formatProperties.width = m_Width;
		properties.formatProperties.height = m_Height;
		properties.sampleProperties.filter = TextureFilter::LINEAR;

		for (int i = 0; i < count; i++) {
			GLTexture* colorTexture = static_cast<GLTexture*>(Texture::Create(properties));
			m_ColorBuffers.push_back(colorTexture);
		}

		unbind();
	}

	void GLFrameBuffer::attachDepthTexture(Graphics::TextureFormatProperties format)
	{
		bind();

		TextureProperties properties;
		properties.formatProperties = format;
		properties.formatProperties.width = m_Width;
		properties.formatProperties.height = m_Height;
		properties.sampleProperties.filter = TextureFilter::LINEAR;

		m_DepthTexture = Texture::Create(properties);

#if !OE_OPENGL_ES
		m_DepthTexture->bind(0);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		//m_DepthTexture->Unbind();
#endif

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, static_cast<GLTexture*>(m_DepthTexture)->getID(), 0);

		unbind();
	}

	void GLFrameBuffer::attachDepthCubemap(Graphics::TextureFormatProperties format)
	{
		bind();

		TextureProperties properties;
		properties.dimension = CUBEMAP;
		properties.formatProperties = format;
		properties.formatProperties.width = m_Width;
		properties.formatProperties.height = m_Height;

		Graphics::GLTexture* m_DepthTexture = static_cast<GLTexture*>(Texture::Create(properties));
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture->getID(), 0);

		unbind();
	}

	void GLFrameBuffer::finalize()
	{
		bind();

		useMip(0);

		std::vector<GLuint> attachments(m_ColorBuffers.size());
		for (size_t i = 0; i < m_ColorBuffers.size(); i++)
			attachments[i] = (GLuint)(GL_COLOR_ATTACHMENT0 + i);

		if (m_ColorBuffers.size() == 0)
			glDrawBuffers(0, 0);
		else
			glDrawBuffers((GLsizei)m_ColorBuffers.size(), &attachments[0]);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			OE_LOG_WARNING("FrameBuffer is not complete! Reason:" << glCheckFramebufferStatus(GL_FRAMEBUFFER));

		unbind();
	}

	/*
	void FrameBuffer::attachRenderBuffer(GLuint internalFormat)
	{
		bind();

		// TODO Create a class for a RenderBuffer
		GLuint rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		unbind();
	}
	*/
} }