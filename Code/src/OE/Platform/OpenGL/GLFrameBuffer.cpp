#include "OE/Platform/OpenGL/GLFrameBuffer.hpp"

#include <math.h>

#include "OE/Platform/OpenGL/GLTexture.hpp"

namespace OrbitEngine { namespace Graphics {
	GLFrameBuffer::GLFrameBuffer(GLuint fbo)
		: m_FBO(fbo), FrameBuffer(0, 0)
	{
	}

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

	void GLFrameBuffer::attachColorTextures(int count, TextureFormatProperties formatProperties, TextureSampleProperties sampleProperties, TextureDimension dimension)
	{
		bind();

		TextureProperties properties;
		properties.dimension = dimension;
		properties.formatProperties = formatProperties;
		properties.sampleProperties = sampleProperties;
		properties.width = m_Width;
		properties.height = m_Height;

		for (int i = 0; i < count; i++) {
			GLTexture* colorTexture = static_cast<GLTexture*>(Texture::Create(properties));
			m_ColorBuffers.push_back(colorTexture);
		}

		unbind();
	}

	void GLFrameBuffer::attachDepthStencilTexture(bool stencil, TextureSampleProperties sampleProperties, TextureDimension dimension)
	{
		bind();

		TextureProperties properties;
		properties.formatProperties.dataType = stencil ? TextureDataType::UNSIGNED_INT : TextureDataType::UNSIGNED_BYTE;
		properties.formatProperties.format = stencil ? TextureFormat::DEPTH_STENCIL : TextureFormat::DEPTH;
		properties.sampleProperties = sampleProperties;
		properties.width = m_Width;
		properties.height = m_Height;

		m_DepthStencilTexture = Texture::Create(properties);

#if !OE_OPENGL_ES
		m_DepthStencilTexture->bind(0);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		//m_DepthTexture->Unbind();
#endif
		GLenum attachment = stencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, static_cast<GLTexture*>(m_DepthStencilTexture)->getID(), 0);

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

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCheckFramebufferStatus.xhtml

#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8

			std::string status_value = "-";
			switch (status)
			{
			case GL_FRAMEBUFFER_UNDEFINED:				  status_value = "GL_FRAMEBUFFER_UNDEFINED"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:    status_value = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: status_value = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
			case GL_FRAMEBUFFER_UNSUPPORTED:			  status_value = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:	  status_value = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:	  status_value = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:	  status_value = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: status_value = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
			}
			OE_LOG_ERROR("FrameBuffer is not complete! Reason: " << status_value << " (" << status << "). See glCheckFramebufferStatus.");
		}

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