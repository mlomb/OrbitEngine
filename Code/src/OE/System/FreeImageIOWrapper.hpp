#ifndef SYSTEM_FREEIMAGE_IO_WRAPPER_H
#define SYSTEM_FREEIMAGE_IO_WRAPPER_H

#ifndef FREEIMAGE_LIB
#define FREEIMAGE_LIB
#endif
#include <FreeImage.h>

#include "OE/System/IOStream.hpp"

namespace OrbitEngine { namespace System { namespace priv {
	unsigned FreeImage_Read(void *buffer, unsigned size, unsigned count, fi_handle handle) {
		return (unsigned)((IOStream*)handle)->read(buffer, size, count);
	}

	unsigned FreeImage_Write(void *buffer, unsigned size, unsigned count, fi_handle handle) {
		return (unsigned)((IOStream*)handle)->write(buffer, size, count);
	}

	int FreeImage_Seek(fi_handle handle, long offset, int origin) {
		return ((IOStream*)handle)->seek(offset, origin);
	}

	long FreeImage_Tell(fi_handle handle) {
		return ((IOStream*)handle)->tell();
	}

	void SetFreeImageIO(FreeImageIO* io) {
		io->read_proc = FreeImage_Read;
		io->write_proc = FreeImage_Write;
		io->seek_proc = FreeImage_Seek;
		io->tell_proc = FreeImage_Tell;
	}
} } }

#endif