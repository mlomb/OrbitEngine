#ifndef GRAPHICS_GLTEXTURE_HPP
#define GRAPHICS_GLTEXTURE_HPP

#include <vector>

#include "OE/Graphics/API/Texture.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine { namespace Graphics {
	class GLTexture : public Texture {
	public:
		GLTexture(TextureProperties properties, std::vector<void*> data);
		~GLTexture();

		void applyParameters();
		void bind(unsigned int slot) override;
		void setData(void* data, unsigned int offset = 0) override;
		void copy(Texture* source) override;
		void* getData() override;

		static void Unbind(const unsigned int slot);

		inline GLuint getID() const { return m_ID; };

		static GLenum TextureFormatToGL(TextureFormat format);
		static GLenum TextureInternalFormatToGL(TextureFormat format, GLenum _default = (GLenum)0);
		static GLenum TextureDataTypeToGL(TextureDataType dataType);
		static GLenum TextureWrapToGL(TextureWrap wrap);
		static GLenum TextureFilterToGL(TextureFilter filter);
		static GLenum TextureDimensionToGL(TextureDimension dimension);
	private:
		GLuint m_ID;

		GLenum m_Target;
		GLenum m_Format;
		GLenum m_InternalFormat;
		GLenum m_Wrap;
		GLenum m_Filter;
		GLenum m_DataType;
	};
} }

#endif