#ifndef GRAPHICS_FREE_IMAGE_HPP
#define GRAPHICS_FREE_IMAGE_HPP

#include <string>

namespace OrbitEngine { namespace Graphics {

	unsigned char* ReadImage(const std::string& path, unsigned int& width, unsigned int& height, unsigned int& bpp, bool force32bits = false);
	bool WritePNG(const std::string& path, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data);

} }

#endif