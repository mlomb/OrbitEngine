#include "OE/Graphics/API/Texture.hpp"

#include "OE/Config.hpp"

#if OE_WINDOWS
	#include <OE/Platform/Windows/Windows.hpp>
#elif OE_ANDROID
	#include <OE/Platform/Android/Android.hpp>
#endif

#include "OE/System/File.hpp"
#include "OE/Graphics/FreeImage.hpp"
#include "OE/Application/Context.hpp"
#include "OE/Misc/Log.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLTexture.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DTexture.hpp"
#endif

namespace OrbitEngine { namespace Graphics {

	Texture* Texture::Create(TextureProperties& properties, std::vector<void*> data)
	{
		OE_LOG_DEBUG("Creating texture: format: " << properties.formatProperties.format << ", size: " << properties.width << "x" << properties.height);

		switch (Application::Context::GetCurrentAPI()) {
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			return new GLTexture(properties, data);
#endif
#if OE_D3D
		case DIRECT3D:
			return new D3DTexture(properties, data);
#endif
		}

		OE_ASSERT(false);
		return nullptr;
	}

	Texture* Texture::Create(TextureProperties& properties, void* data)
	{
		std::vector<void*> v(1);
		v[0] = data;
		return Create(properties, v);
	}

	Texture* Texture::Load(std::vector<std::string> files, TextureSampleProperties& sampleProperties)
	{
		if (files.size() == 0) {
			OE_LOG_WARNING("Files size can't be zero.");
			return nullptr;
		}

		TextureProperties properties;
		properties.sampleProperties = sampleProperties;
		std::vector<void*> dataPtrs;

		unsigned int w, h, bpp;

		for (size_t i = 0; i < files.size(); i++) {
			bool f32b = false;
#if OE_D3D
			// DirectX11 doesn't support 24bit textures, force 32bit
			f32b = Application::Context::GetCurrentAPI() == DIRECT3D;
#endif
			void* data = ReadImage(files[i], w, h, bpp, f32b); // We assume formats will match

			properties.formatProperties = FormatPropertiesFromBPP(bpp);

			if (data == 0) {
				for (size_t i = 0; i < dataPtrs.size(); i++)
					delete dataPtrs[i];
				return 0;
			}
			dataPtrs.push_back(data);
		}

		properties.width = w;
		properties.height = h;

		Texture* t = Create(properties, dataPtrs);

		for (size_t i = 0; i < dataPtrs.size(); i++)
			delete dataPtrs[i];

		return t;
	}

	Texture* Texture::Load(std::string file, TextureSampleProperties& sampleProperties)
	{
		std::vector<std::string> v(1);
		v[0] = file;
		return Load(v, sampleProperties);
	}

	Texture* Texture::Load(std::string file)
	{
		TextureSampleProperties sampleProperties;
		return Load(file, sampleProperties);
	}

	Texture* Texture::Load(char* file_data, size_t file_size)
	{
		TextureProperties properties;
		unsigned int w, h, bpp;

		unsigned char* data = ReadImage(reinterpret_cast<unsigned char*>(file_data), file_size, w, h, bpp);

		properties.width = w;
		properties.height = h;
		properties.formatProperties = FormatPropertiesFromBPP(bpp);

		return Create(properties, data);
	}

	Texture* Texture::Load(std::vector<std::string> files)
	{
		TextureSampleProperties sampleProperties;
		return Load(files, sampleProperties);
	}

	unsigned int Texture::CalculateMipLevelsCount(unsigned int width, unsigned int height)
	{
		unsigned int mips = 1;

		while (width > 1 && height > 1)
		{
			width = std::max(width / 2, 1u);
			height = std::max(height / 2, 1u);
			++mips;
		}

		return mips;
	}

	unsigned int Texture::BPPFromFormat(TextureFormat format)
	{
		switch (format) {
		case R8:
			return 8;
		case R16:
		case RG8:
			return 16;
		case RGB:
		case RGB8:
			return 24;
		case BGRA:
		case RGBA:
		case RGBA8:
			return 32;
		case RG32F:
			return 64;
		case RGB32F:
			return 96;
		default:
			OE_LOG_WARNING("BPP not set for format!");
			return 0;
		}

	}

	TextureFormatProperties Texture::FormatPropertiesFromBPP(unsigned int bpp)
	{
		TextureFormatProperties fp;
		fp.dataType = UNSIGNED_BYTE;
		switch (bpp) {
		case 8:
			fp.format = R8;
			break;
		case 16:
			fp.format = R16;
			break;
		case 24:
			fp.format = RGB;
			break;
		case 32:
			fp.format = RGBA;
			break;
		case 96:
			fp.format = RGB32F;
			fp.dataType = FLOAT;
			break;
		}
		return fp;
	}

	Texture::Texture(TextureProperties properties)
		: m_Properties(properties)
	{
		//ResourcesManager::RegisterTexture(this);

		if (m_Properties.sampleProperties.mipmapping)
			m_MipLevels = CalculateMipLevelsCount(m_Properties.width, m_Properties.height);
	}

	Texture::~Texture()
	{
		//ResourcesManager::UnregisterTexture(this);
	}

} }