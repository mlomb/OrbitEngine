#ifndef GRAPHICS_TEXTURE_HPP
#define GRAPHICS_TEXTURE_HPP

#include "OE/Graphics/API/Buffer.hpp"

namespace OrbitEngine { namespace Graphics {
	enum TextureFormat {
		R8 = 0,
		RGB,
		R16,
		RG8,
		RG32F,
		RGB8,
		RGB32F,
		RGB16F,
		RGBA,
		RGBA8,
		RGBA32F,
		SRC_ALPHA,
		LUMINANCE_ALPHA,
		DEPTH,
		DEPTH_STENCIL
	};

	enum TextureFilter {
		ANISOTROPIC = 0,
		LINEAR,
		NEAREST
	};

	enum TextureWrap {
		REPEAT = 0,
		MIRROR_REPEAT,
		CLAMP,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	enum TextureDataType {
		UNSIGNED_INT,
		UNSIGNED_BYTE,
		FLOAT
	};

	enum TextureDimension {
		TEXTURE1D,
		TEXTURE2D,
		TEXTURE2DARRAY,
		TEXTURE3D,
		CUBEMAP
	};

	struct TextureFormatProperties {
		TextureFormat format = RGB;
		TextureDataType dataType = UNSIGNED_BYTE;
	};

	struct TextureSampleProperties {
		TextureWrap wrap = CLAMP_TO_EDGE;
		TextureFilter filter = ANISOTROPIC;
		bool mipmapping = false;
	};

	struct TextureProperties {
		TextureFormatProperties formatProperties;
		TextureSampleProperties sampleProperties;
		TextureDimension dimension = TEXTURE2D;
		BufferMode textureBufferMode = DYNAMIC;
		unsigned short width = 16, height = 16;
	};

	class Texture {
	public:
		virtual ~Texture();

		static Texture* Create(TextureProperties& properties, std::vector<void*> data);
		static Texture* Create(TextureProperties& properties, void* data = 0);
		static Texture* Load(std::vector<std::string> files, TextureSampleProperties& sampleProperties);
		static Texture* Load(std::string file, TextureSampleProperties& sampleProperties);
		static Texture* Load(std::vector<std::string> files);
		static Texture* Load(std::string file);

		virtual void bind(unsigned int slot) = 0;
		virtual void setData(void* data, unsigned int offset = 0) = 0;
		virtual void copy(Texture* source) = 0;
		virtual void* getData() = 0; // you must delete the pointer after using it

		inline unsigned int getMipmapLevelsCount() { return m_Properties.sampleProperties.mipmapping ? m_MipLevels : 1; };

		TextureProperties getProperties() const { return m_Properties; }

		static unsigned int CalculateMipLevelsCount(unsigned int width, unsigned int height);
		static unsigned int BPPFromFormat(TextureFormat format);
	protected:
		Texture(TextureProperties properties);

		TextureProperties m_Properties;
		unsigned int m_MipLevels = 1;
	};
} }


#endif