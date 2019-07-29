#include "OE/Graphics/FreeImage.hpp"

#include "OE/System/IOStream.hpp"
#include "OE/System/File.hpp"
#include "OE/Misc/Log.hpp"

#include <FreeImage.h>

namespace OrbitEngine { namespace Graphics {

	static FreeImageIO freeImage_io = {};
	static bool freeImage_initialized = false;

	unsigned FreeImage_Read(void* buffer, unsigned size, unsigned count, fi_handle handle) {
		return (unsigned)((System::IOStream*)handle)->read(buffer, size, count);
	}

	unsigned FreeImage_Write(void* buffer, unsigned size, unsigned count, fi_handle handle) {
		return (unsigned)((System::IOStream*)handle)->write(buffer, size, count);
	}

	int FreeImage_Seek(fi_handle handle, long offset, int origin) {
		return ((System::IOStream*)handle)->seek(offset, origin);
	}

	long FreeImage_Tell(fi_handle handle) {
		return ((System::IOStream*)handle)->tell();
	}

	void SetFreeImageIO(FreeImageIO* io) {
		io->read_proc = FreeImage_Read;
		io->write_proc = FreeImage_Write;
		io->seek_proc = FreeImage_Seek;
		io->tell_proc = FreeImage_Tell;
	}

	void init() {
		if (!freeImage_initialized) {
			FreeImage_Initialise();
			SetFreeImageIO(&freeImage_io);

			freeImage_initialized = true;
			// TODO ...
			// FreeImage_DeInitialise();
		}
	}

	unsigned char* ReadImage(const std::string& path, unsigned int& width, unsigned int& height, unsigned int& bpp, bool force32bits)
	{
		init();

		System::IOStream* fileStream = System::File::Open(path);

		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib = NULL;

		width = 0;
		height = 0;
		bpp = 0;

		fif = FreeImage_GetFileTypeFromHandle(&freeImage_io, fileStream);
		if (fif == FIF_UNKNOWN) {
			OE_LOG_WARNING("Can't determinate the format of the image: " << path);
			delete fileStream;
			return nullptr;
		}

		if (FreeImage_FIFSupportsReading(fif)) {
			dib = FreeImage_LoadFromHandle(fif, &freeImage_io, fileStream, 0);
		}
		else {
			OE_LOG_WARNING("Unsupported format in FreeImage: " << path);
			delete fileStream;
			return nullptr;
		}

		if (!dib) {
			OE_LOG_WARNING("Problem reading the image: " << path);
			delete fileStream;
			return nullptr;
		}

		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		bpp = FreeImage_GetBPP(dib);

#if OE_D3D
		if (bpp == 24 && force32bits) {
			// DirectX11 don't support 24bit textures
			FIBITMAP* tmp = FreeImage_ConvertTo32Bits(dib);
			FreeImage_Unload(dib);
			dib = tmp;
			bpp = FreeImage_GetBPP(dib);
		}
#endif

		OE_ASSERT(bpp);

		BYTE* bits = FreeImage_GetBits(dib);

		unsigned int stride = (bpp / 8);
		unsigned int size = width * height * stride;

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

		unsigned char* data = (unsigned char*)malloc(size);
		memcpy(data, bits, size);
		FreeImage_Unload(dib);
		delete fileStream;

		OE_ASSERT(data);
		OE_ASSERT(width);
		OE_ASSERT(height);

		return data;
	}

	bool WritePNG(const std::string& path, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data)
	{
		init();

		FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(data, width, height, width * bpp / 8, bpp, 0, 0, 0, TRUE);

		if (!bitmap)
			return false;

		LONG result = 1;
		System::IOStream* fileStream = System::File::Open(path, System::AccessMode::WRITE);

		if (fileStream->isOpen()) {
			result = FreeImage_SaveToHandle(FIF_PNG, bitmap, &freeImage_io, fileStream, PNG_DEFAULT);
			fileStream->flush();
		}

		delete fileStream;
		FreeImage_Unload(bitmap);

		return result == TRUE;
	}
} }