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
#include "OE/System/FreeImageIOWrapper.hpp"

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

		unsigned int w, h;

		for (size_t i = 0; i < files.size(); i++) {
			void* data = Texture::LoadImageData(files[i], properties.formatProperties, w, h); // We assume formats will match
			if (data == 0)
				return 0;
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

	Texture* Texture::Load(std::vector<std::string> files)
	{
		TextureSampleProperties sampleProperties;
		return Load(files, sampleProperties);
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

	void* Texture::LoadImageData(const std::string& file, TextureFormatProperties& formatProperties, unsigned int& width, unsigned int& height)
	{
		if (!s_FreeImageInitialized) {
			FreeImage_Initialise();
			s_FreeImageInitialized = true;
			// TODO ...
			// FreeImage_DeInitialise();
		}

		FreeImageIO freeImage_io;
		System::priv::SetFreeImageIO(&freeImage_io);
		System::IOStream* fileStream = System::File::Open(file);

		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP *dib(0);

		width = 0;
		height = 0;

		fif = FreeImage_GetFileTypeFromHandle(&freeImage_io, fileStream);
		if (fif == FIF_UNKNOWN) {
			OE_LOG_WARNING("Can't determinate the format of the image: " + file);
			delete fileStream;
			return 0;
		}
		if (FreeImage_FIFSupportsReading(fif)) dib = FreeImage_LoadFromHandle(fif, &freeImage_io, fileStream, 0);
		else {
			OE_LOG_WARNING("FreeImage don't support reading the image: " + file);
			delete fileStream;
			return 0;
		}
		if (!dib) {
			OE_LOG_WARNING("Problem reading the image: " + file);
			delete fileStream;
			return 0;
		}

		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		unsigned int bpp = FreeImage_GetBPP(dib);

#if OE_D3D
		if (bpp == 24 && Application::Context::GetCurrentAPI() == DIRECT3D) {
			// DirectX11 don't support 24bit textures
			FIBITMAP *tmp = FreeImage_ConvertTo32Bits(dib);
			FreeImage_Unload(dib);
			dib = tmp;
			bpp = FreeImage_GetBPP(dib);
		}
#endif

		BYTE* bits = FreeImage_GetBits(dib);

		unsigned int stride = (bpp / 8);
		unsigned int padding = width * height * stride;

#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
		if (bpp == 32 || bpp == 24) {
			// Taken from SwapRedBlue32
			const unsigned pitch = FreeImage_GetPitch(dib);
			const unsigned lineSize = FreeImage_GetLine(dib);
			BYTE* line = bits;
			for (unsigned y = 0; y < height; ++y, line += pitch) {
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

		if ((data == 0) || (width == 0) || (height == 0)) {
			OE_LOG_WARNING("Invalid data, width or height when loading texture: " + file);
			return 0;
		}

		switch (bpp) {
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