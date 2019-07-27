#ifndef GRAPHICS_MSDF_HPP
#define GRAPHICS_MSDF_HPP

#include "OE/Graphics/Bitmap.hpp"

struct FT_Outline_;
typedef FT_Outline_ FT_Outline;

namespace OrbitEngine { namespace Graphics {

	/// [msdfgen](https://github.com/Chlumsky/msdfgen)'s modes
	enum class SDFMode {
		SDF,  ///< monochrome signed distance field
		PSDF, ///< monochrome signed pseudo-distance field
		MSDF  ///< multi-channel signed distance field
	};

	/**
		@brief Generates a BitmapRGB from a FreeType outline using msdfgen's signed distance field method

		@param[in] outline FreeType's outline
		@param[in] mode See SDFMode.
		@param[in] range Width of the range around the shape between the minimum and maximum representable signed distance in pixels
		@return The generated bitmap, that can be invalid if the generation failed
	*/
	BitmapRGB GenerateBitmapFromOutline(FT_Outline* outline, SDFMode mode, double range);

} }

#endif