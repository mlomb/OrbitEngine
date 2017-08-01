#include "OE/Graphics/API/Texture.hpp"

#include "OE/Config.hpp"

#if OE_WINDOWS
	#include <OE/Platform/Windows/Windows.hpp>
#elif OE_ANDROID
	#include <OE/Platform/Android/Android.hpp>
#endif

#include "OE/System/File.hpp"

#ifndef FREEIMAGE_LIB
#define FREEIMAGE_LIB
#endif
#include <FreeImage.h>

#include "OE/Application/Context.hpp"
#include "OE/Misc/Log.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLTexture.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DTexture.hpp"
#endif

namespace OrbitEngine { namespace Graphics {

	bool Texture::s_FreeImageInitialized = false;

	Texture* Texture::Create(TextureProperties& properties, std::vector<void*> data)
	{
		OE_LOG_DEBUG("Creating texture: format: " << properties.formatProperties.format << ", size: " << properties.formatProperties.width << "x" << properties.formatProperties.height);

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

	Texture* Texture::Load(std::vector<std::string> files, TextureProperties& properties)
	{
		if (files.size() == 0) {
			OE_LOG_WARNING("Files size can't be zero.");
			return nullptr;
		}

		std::vector<void*> dataPtrs;

		for (size_t i = 0; i < files.size(); i++) {
			void* data = Texture::LoadImageData(files[i], properties.formatProperties); // We assume formats will match
			dataPtrs.push_back(data);
		}

		Texture* t = Create(properties, dataPtrs);

		for (size_t i = 0; i < dataPtrs.size(); i++)
			delete dataPtrs[i];

		return t;
	}

	Texture* Texture::Load(std::string file, TextureProperties& properties)
	{
		std::vector<std::string> v(1);
		v[0] = file;
		return Load(v, properties);
	}

	Texture* Texture::Load(std::string file)
	{
		TextureProperties properties;
		return Load(file, properties);
	}

	Texture* Texture::Load(std::vector<std::string> files)
	{
		TextureProperties properties;
		return Load(files, properties);
	}

	void Texture::Unbind(const unsigned int slot)
	{
		switch (Application::Context::GetCurrentAPI()) {
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			GLTexture::Unbind(slot);
			break;
#endif
#if OE_D3D
		case DIRECT3D:
			//D3DTexture::Unbind(slot);
			break;
#endif
		}
	}

	void* Texture::LoadImageData(std::string file, TextureFormatProperties& formatProperties)
	{
		if (!s_FreeImageInitialized) {
			FreeImage_Initialise();
			s_FreeImageInitialized = true;
			// TODO ...
			// FreeImage_DeInitialise();
		}

		/* Load Image File to BYTE* */
		std::vector<char> buffer = System::LoadFile(file, true);
		if (buffer.size() == 0)
			return 0;

		FIMEMORY* fiStream = FreeImage_OpenMemory((BYTE*)buffer.data(), buffer.size());

		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP *dib(0);

		formatProperties.width = 0;
		formatProperties.height = 0;

		fif = FreeImage_GetFileTypeFromMemory(fiStream);
		if (fif == FIF_UNKNOWN) {
			OE_LOG_WARNING("Can't determinate the format of the image: " + file);
			return 0;
		}
		if (FreeImage_FIFSupportsReading(fif)) dib = FreeImage_LoadFromMemory(fif, fiStream, 0);
		else {
			OE_LOG_WARNING("FreeImage don't support reading the image: " + file);
			return 0;
		}
		if (!dib) {
			OE_LOG_WARNING("Problem reading the image: " + file);
			return 0;
		}

		formatProperties.width = FreeImage_GetWidth(dib);
		formatProperties.height = FreeImage_GetHeight(dib);
		formatProperties.bpp = FreeImage_GetBPP(dib);

#if OE_D3D
		if (formatProperties.bpp == 24 && Application::Context::GetCurrentAPI() == DIRECT3D) {
			// DirectX11 don't support 24bit textures
			FIBITMAP *tmp = FreeImage_ConvertTo32Bits(dib);
			FreeImage_Unload(dib);
			dib = tmp;
			formatProperties.bpp = FreeImage_GetBPP(dib);
		}
#endif

		BYTE* bits = FreeImage_GetBits(dib);

		unsigned int stride = (formatProperties.bpp / 8);
		unsigned int padding = formatProperties.width * formatProperties.height * stride;

#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
		if (formatProperties.bpp == 32 || formatProperties.bpp == 24) {
			// Taken from SwapRedBlue32
			const unsigned pitch = FreeImage_GetPitch(dib);
			const unsigned lineSize = FreeImage_GetLine(dib);
			BYTE* line = bits;
			for (unsigned y = 0; y < formatProperties.height; ++y, line += pitch) {
				for (BYTE* pixel = line; pixel < line + lineSize; pixel += stride) {
					// Taken from INPLACESWAP
					// a ^= b; b ^= a; a ^= b;
					pixel[0] ^= pixel[2]; pixel[2] ^= pixel[0]; pixel[0] ^= pixel[2];
				}
			}
		}
#endif

		void* data = malloc(padding);
		memcpy(data, bits, padding);
		FreeImage_Unload(dib);

		if ((data == 0) || (formatProperties.width == 0) || (formatProperties.height == 0)) {
			OE_LOG_WARNING("Invalid data, width or height when loading texture: " + file);
			return 0;
		}

		switch (formatProperties.bpp) {
		case 8:
			formatProperties.format = R8;
			break;
		case 16:
			formatProperties.format = R16;
			break;
		case 24:
			formatProperties.format = RGB;
			break;
		case 32:
			formatProperties.format = RGBA;
			break;
		case 96:
			formatProperties.format = RGB32F;
			formatProperties.dataType = FLOAT;
			break;
		default:
			OE_LOG_WARNING("Unsupported bit depth in " + file);
			return 0;
		}

		return data;
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

	Texture::Texture(TextureProperties properties)
		: m_Properties(properties)
	{
		//ResourcesManager::RegisterTexture(this);

		if (m_Properties.formatProperties.mipmapping)
			m_MipLevels = CalculateMipLevelsCount(m_Properties.formatProperties.width, m_Properties.formatProperties.height);
	}

	Texture::~Texture()
	{
		//ResourcesManager::UnregisterTexture(this);
	}

} }