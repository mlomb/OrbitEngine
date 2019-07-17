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
		unsigned short width = 16, height = 16;
		TextureFormat format;
		TextureDataType dataType = UNSIGNED_BYTE;
		bool mipmapping = true;
	};

	struct TextureSampleProperties {
		TextureWrap wrap = CLAMP_TO_EDGE;
		TextureFilter filter = ANISOTROPIC;
	};

	struct TextureProperties {
		TextureFormatProperties formatProperties;
		TextureSampleProperties sampleProperties;
		TextureDimension dimension = TEXTURE2D;
		BufferMode textureBufferMode = DYNAMIC;
	};

	class Texture {
	public:
		virtual ~Texture();

		static Texture* Create(TextureProperties& properties, std::vector<void*> data);
		static Texture* Create(TextureProperties& properties, void* data = 0);
		static Texture* Load(std::vector<std::string> files, TextureProperties& properties);
		static Texture* Load(std::string file, TextureProperties& properties);
		static Texture* Load(std::vector<std::string> files);
		static Texture* Load(std::string file);

		virtual void bind(unsigned int slot) = 0;
		virtual void setData(void* data, unsigned int offset = 0) = 0;
		virtual void copy(Texture* source) = 0;

		static void Unbind(const unsigned int slot);

		inline unsigned int getMipmapLevelsCount() { return m_Properties.formatProperties.mipmapping ? m_MipLevels : 1; };

		TextureProperties getProperties() const { return m_Properties; }

		static void* LoadImageData(std::string file, TextureFormatProperties& formatProperties);
		static unsigned int CalculateMipLevelsCount(unsigned int width, unsigned int height);
		static unsigned int BPPFromFormat(TextureFormat format);
	protected:
		Texture(TextureProperties properties);

		TextureProperties m_Properties;
		unsigned int m_MipLevels = 1;

	private:
		static bool s_FreeImageInitialized;
	};
} }


#endif