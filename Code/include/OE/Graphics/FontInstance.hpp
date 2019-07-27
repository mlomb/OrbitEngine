#ifndef GRAPHICS_FONT_INSTANCE_HPP
#define GRAPHICS_FONT_INSTANCE_HPP

#include <string>
#include <map>
#include <vector>

#include "OE/Math/Vec2.hpp"
#include "OE/Math/UV.hpp"

namespace OrbitEngine { namespace Graphics {
	/// Character's unicode code
	typedef unsigned int GlyphIndex;
	/// Font sizes specified in pixels
	typedef unsigned int FontSize;

	enum FontAtlasMode {
		BITMAP, ///< monochrome bitmap
		SDF,	///< SDFMode::SDF
		PSDF,	///< SDFMode::PSDF
		MSDF	///< SDFMode::MSDF
	};

	/**
		https://www.freetype.org/freetype2/docs/glyphs/metrics.png
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