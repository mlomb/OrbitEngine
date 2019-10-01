#ifndef GRAPHICS_MSDF_HPP
#define GRAPHICS_MSDF_HPP

#include "OE/Graphics/Bitmap.hpp"
#include "OE/Math/Vec2.hpp"

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

	/**
		@brief Generates a BitmapRGB from a SVG path using msdfgen's signed distance field method

		@param[in] svg_path SVG path (e.j "M150 0 L75 200 L225 200 Z")
		@param[in] mode See SDFMode.
		@param[out] center corrected origin position after translation
		@param[in] range Width of the range around the shape between the minimum and maximum representable signed distance in pixels
		@param[in] scale scale all coordinates by this factor
		@param[in] square force a 1:1 w/h ratio
		@return The generated bitmap, that can be invalid if the generation failed
	*/
	BitmapRGB GenerateBitmapFromSVGPath(const std::string& svg_path, SDFMode mode, double range, Math::Vec2i& center, double scale = 1, bool square = false);

} }

#endif