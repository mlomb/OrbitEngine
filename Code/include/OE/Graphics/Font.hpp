#ifndef GRAPHICS_FONT_HPP
#define GRAPHICS_FONT_HPP

#include <string>
#include <vector>

#include "Bitmap.hpp"

/* Forward defs */
struct FT_LibraryRec_;
typedef FT_LibraryRec_* FT_Library;
struct FT_FaceRec_;
typedef FT_FaceRec_* FT_Face;

namespace OrbitEngine { namespace Graphics {
	/// Unicode codepoint
	typedef unsigned int GlyphCodepoint;
	/// Font sizes specified in pixels
	typedef unsigned int FontSize;

	/**
		Example: Grinning Face emoji (U+1F600) and **&** glyphs at 128px

		GRAY
		@image html font-face-gray.png width=75px
		@image html font-amp-gray.png width=75px

		SDF/PSDF
		@image html font-face-sdf.png width=75px
		@image html font-amp-sdf.png width=75px

		MSDF
		@image html font-face-msdf.png width=75px
		@image html font-amp-msdf.png width=75px

		COLOR
		@image html font-face-color.png width=75px
	*/
	enum GlyphRenderMode {
		GRAY  = 0, ///< monochrome
		SDF   = 1, ///< SDFMode::SDF
		PSDF  = 2, ///< SDFMode::PSDF
		MSDF  = 3, ///< SDFMode::MSDF
		COLOR = 4  ///< full color with alpha (e.g. emojis)
	};

	/**
		@brief Single glyph metrics

		[Details](https://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html)

		@image html glyph-metrics.png "Font metrics"
	*/
	struct GlyphMetrics {
		int width, height;
		int H_advance, H_bearingX, H_bearingY;
		int V_advance, V_bearingX, V_bearingY;
	};

	/// Represents a single typeface
	class Font {
	public:
		/// Load a font file
		Font(const std::string& file);
		~Font();

		/// Returns a list of all the available code points in the font file
		std::vector<GlyphCodepoint> getAvailableGlyphs() const;
		/**
			@brief Get glyph bitmap and metrics

			@param[in] codepoint Unicode codepoint
			@param[in] size target size
			@param[in] mode render mode. See GlyphRenderMode
			@param[out] bitmap rendered bitmap
			@param[out] metrics glyph metrics
			@return Whether the operation was successful

			Example: Render glyph **&** at size 100

			@code
			Font* font = new Font("Font.ttf");

			BitmapRGBA bmp;
			Glyph g;

			if (font->getGlyph(38, 100, GlyphRenderMode::BITMAP, bmp, g)) { // U+0026 or 38 or &
				bmp.savePNG("&.png");
			}
			@endcode
		*/
		bool getGlyph(GlyphCodepoint codepoint, FontSize size, GlyphRenderMode mode, BitmapRGBA& bitmap, GlyphMetrics& metrics);

		/// Kerning distance between left and right glyphs at a specific size in unfitted pixels
		int getHorizontalKerning(FontSize size, GlyphCodepoint left, GlyphCodepoint right);

	private:
		static FT_Library s_FTLibrary;

		std::vector<char> m_FileBuffer;
		FT_Face m_Face;

		/// internal function to set the current freetype pixel sizes
		bool setSize(FontSize size);
		FontSize m_CurrentSize;
	};
} }

#endif