#include "OE/Platform/OpenGL/GLTexture.hpp"

#include "OE/Math/Math.hpp"

namespace OrbitEngine { namespace Graphics {
	GLTexture::GLTexture(TextureProperties properties, std::vector<void*> data)
		: Texture(properties)
	{
		glGenTextures(1, &m_ID);

		m_Target = TextureDimensionToGL(properties.dimension);
		m_Format = TextureFormatToGL(m_Properties.formatProperties.format);
		m_InternalFormat = TextureInternalFormatToGL(m_Properties.formatProperties.format, m_Format);
		m_Wrap = TextureWrapToGL(m_Properties.sampleProperties.wrap);
		m_Filter = TextureFilterToGL(m_Properties.sampleProperties.filter);
		m_DataType = TextureDataTypeToGL(m_Properties.formatProperties.dataType);

		if (m_Properties.formatProperties.format == TextureFormat::DEPTH_STENCIL) {
			m_DataType = GL_UNSIGNED_INT_24_8;
		}

		applyParameters();

		unsigned int size = (unsigned int)data.size();

		if (size == 0) {
			setData(nullptr);
			return;
		}
		else if (size == 1 && properties.dimension == CUBEMAP) {
			for (int i = 0; i < 6; i++)
				setData(data[0], i);
		}
		else if (size == 1)
			setData(data[0]);
		else {
			if (properties.dimension == TEXTURE2DARRAY)
				glTexStorage3D(m_Target, size, m_InternalFormat, properties.width, properties.height, size);
			for (int i = 0; i < int(size); i++)
				setData(data[i], (unsigned int)i);
		}

		if (m_Properties.sampleProperties.mipmapping)
			glGenerateMipmap(m_Target);
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &m_ID);
	}

	void GLTexture::applyParameters()
	{
		bind(0);
		glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, m_Wrap);
		glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, m_Wrap);
		glTexParameteri(m_Target, GL_TEXTURE_WRAP_R, m_Wrap);
		glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, m_Filter);
		glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, m_Filter);

		if (m_Properties.sampleProperties.mipmapping) {
			glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//glTexParameterf(m_Target, GL_TEXTURE_LOD_BIAS, -0.2f);

			// TODO Check for extension
#if !OE_EMSCRIPTEN
			if (m_Properties.sampleProperties.filter == ANISOTROPIC) {
				float maxAnisotropy;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
				glTexParameterf(m_Target, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)Math::clamp((int)maxAnisotropy, 0, 8));
			}
#endif
		}
	}

	void GLTexture::setData(void* data, unsigned int offset) {
		bind(0);

		switch (m_Properties.dimension)
		{
		default:
		case TEXTURE2D:
			glTexImage2D(m_Target, 0, m_InternalFormat, m_Properties.width, m_Properties.height, 0, m_Format, m_DataType, data);
			break;
		case TEXTURE2DARRAY:
			//glTexSubImage3D(m_Target, 0, 0, 0, 0, m_Properties.formatProperties.width, m_Properties.formatProperties.height, offset, m_Format, m_DataType, data);
			break;
		case CUBEMAP:
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + offset, 0, m_InternalFormat, m_Properties.width, m_Properties.height, 0, m_Format, m_DataType, data);
			break;
		}
	}

	void GLTexture::copy(Texture* source)
	{
		//glCopyTexSubImage2D(m_Target, 0, 0, 0, 0, 0, m_Properties.formatProperties.width, m_Properties.formatProperties.height);
	}

	void GLTexture::bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(m_Target, m_ID);
	}

	void GLTexture::Unbind(const unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);

		//glBindTexture(GL_TEXTURE_1D, 0);
		//glBindTexture(GL_TEXTURE_1D_ARRAY, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		//glBindTexture(GL_TEXTURE_3D, 0);
		//glBindTexture(GL_TEXTURE_3D_ARRAY, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	GLenum GLTexture::TextureFormatToGL(TextureFormat format)
	{
		switch (format)
		{
		case R8:
		case R16:
			return GL_RED;
		case RG8:
		case RG32F:
			return GL_RG;
		case RGBA:
		case RGBA8:
		case RGBA32F:
			return GL_RGBA;
		case RGB:
		case RGB8:
		case RGB32F:
		case RGB16F:
			return GL_RGB;
		case SRC_ALPHA:
			return GL_ALPHA;
		case LUMINANCE_ALPHA:
			return GL_LUMINANCE_ALPHA;
		case DEPTH:
			return GL_DEPTH_COMPONENT;
		case DEPTH_STENCIL:
			return GL_DEPTH_STENCIL;
		}

		OE_ASSERT(false);
		return 0;
	}

	GLenum GLTexture::TextureInternalFormatToGL(TextureFormat format, GLenum _default)
	{
		switch (format)
		{
		case R8:
			return GL_R8;
		case R16:
			return GL_R16;
		case RG8:
			return GL_RG8;
		case RG32F:
			return GL_RG32F;
		case RGB:
		case RGB8:
			return GL_RGB8;
		case RGB32F:
			return GL_RGB32F;
		case RGB16F:
			return GL_RGB16F;
		case RGBA:
			return GL_RGBA;
		case RGBA8:
			return GL_RGBA8;
		case RGBA32F:
			return GL_RGBA32F;
#if OE_OPENGL_ES
		case DEPTH:
			return GL_DEPTH_COMPONENT24;
#endif
		case DEPTH_STENCIL:
			return GL_DEPTH24_STENCIL8;
		default:
			return _default;
		}
	}

	GLenum GLTexture::TextureDataTypeToGL(TextureDataType dataType)
	{
		switch (dataType)
		{
		case UNSIGNED_INT:
			return GL_UNSIGNED_INT;
		case UNSIGNED_BYTE:
			return GL_UNSIGNED_BYTE;
		case FLOAT:
			return GL_FLOAT;
		}

		OE_ASSERT(false);
		return 0;
	}

	GLenum GLTexture::TextureWrapToGL(TextureWrap wrap)
	{
		switch (wrap)
		{
		case REPEAT:
			return GL_REPEAT;
		case MIRROR_REPEAT:
			return GL_MIRRORED_REPEAT;
		case CLAMP:
#if !OE_OPENGL_ES
		// TDOO See this
			return GL_CLAMP;
#endif
		case CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		case CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
		}

		OE_ASSERT(false);
		return 0;
	}

	GLenum GLTexture::TextureFilterToGL(TextureFilter filter)
	{
		switch (filter)
		{
		case ANISOTROPIC:
		case LINEAR:
			return GL_LINEAR;
		case NEAREST:
			return GL_NEAREST;
		}

		OE_ASSERT(false);
		return 0;
	}

	GLenum GLTexture::TextureDimensionToGL(TextureDimension dimension)
	{
		switch (dimension)
		{
#if !OE_OPENGL_ES
		// TODO See this
		case TEXTURE1D:
			return GL_TEXTURE_1D;
#endif
		case TEXTURE2D:
			return GL_TEXTURE_2D;
		case TEXTURE2DARRAY:
			return GL_TEXTURE_2D_ARRAY;
		case TEXTURE3D:
			return GL_TEXTURE_3D;
		case CUBEMAP:
			return GL_TEXTURE_CUBE_MAP;
		}

		return 0;
	}
} }