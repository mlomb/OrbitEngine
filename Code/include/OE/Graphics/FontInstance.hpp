#ifndef GRAPHICS_FONT_INSTANCE_HPP
#define GRAPHICS_FONT_INSTANCE_HPP

#include <string>
#include <map>

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace Graphics {
	/// Character's unicode code
	typedef unsigned int GlyphIndex;
	/// Font sizes specified in pixels
	typedef unsigned int FontSize;

	/**
		Example: 20px Roboto Regular

		@image html font-atlas-bitmap.png "BITMAP" width=200px
		@image html font-atlas-sdf.png "SDF" width=200px
		@image html font-atlas-psdf.png "PSDF" width=200px
		@image html font-atlas-msdf.png "MSDF" width=200px
	*/
	enum FontAtlasMode {
		BITMAP, ///< monochrome bitmap
		SDF,	///< SDFMode::SDF
		PSDF,	///< SDFMode::PSDF
		MSDF	///< SDFMode::MSDF
	};

	/**
		@brief Single glyph metrics

		[Details](https://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html)

		@image html glyph-metrics.png "Font metrics"
	*/
	struct Glyph {
		int width, height;
		int H_advance, H_bearingX, H_bearingY;
		int V_advance, V_bearingX, V_bearingY;
	};

	/**
		A rendereable font object with a specific size, charset and atlas mode
	*/
	class FontInstance {
	public:
		~FontInstance();

		Math::Vec2i getBounds(const std::string& text);
		/// kerning in pixels between two glyphs
		int getHorizontalKerning(GlyphIndex left, GlyphIndex right) const;
		const std::map<GlyphIndex, Glyph>& getGlyphs() const;

	private:
		FontInstance();

		FontSize m_Size;
		int m_MaxHeight;
		int m_MaxBearing;

		std::map<GlyphIndex, Glyph> m_Glyphs;
		std::map<std::pair<GlyphIndex, GlyphIndex>, int> m_Kernings;

		friend class Font;
	};
} }

#endif