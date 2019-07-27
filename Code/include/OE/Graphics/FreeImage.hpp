#ifndef GRAPHICS_FREE_IMAGE_HPP
#define GRAPHICS_FREE_IMAGE_HPP

#include <string>

namespace OrbitEngine { namespace Graphics {

	/**
		@brief Reads an image from disk

		@param[in] path File location
		@param[out] width,height Read image dimensions
		@param[out] bpp Read image bits per pixel
		@param[in] force32bits Force any 24bit image to load as a 32bit image
		@return Raw pointer to the pixel data. Remember to delete it after its used
	*/
	unsigned char* ReadImage(const std::string& path, unsigned int& width, unsigned int& height, unsigned int& bpp, bool force32bits = false);

	/**
		@brief Writes a PNG file to disk

		@param[in] path File location
		@param[in] width,height Image dimensions
		@param[in] bpp Bits per pixel present int the image
		@param[in] data Raw pointer to the pixel data. Make sure it has at least \p width * \p height * \p bpp bits
		@return Whether the image was saved successfully
	*/
	bool WritePNG(const std::string& path, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data);

} }

#endif