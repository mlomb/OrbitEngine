#ifndef GRAPHICS_FONT_HPP
#define GRAPHICS_FONT_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "OE/Graphics/Bitmap.hpp"
#include "OE/Math/Rect.hpp"

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

	class Font;

	/**
		Example: Grinning Face emoji (U+1F600) and **&** glyphs at 128px

		GRAY
		@image html font-face-gray.png width=60px
		@image html font-amp-gray.png width=60px

		SDF/PSDF
		@image html font-face-sdf.png width=60px
		@image html font-amp-sdf.png width=60px

		MSDF
		@image html font-face-msdf.png width=60px
		@image html font-amp-msdf.png width=60px

		COLOR
		@image html font-face-color.png width=60px
		@image html font-amp-color.png width=60px
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

		@image html glyph-metrics.png "Glyph metrics"
	*/
	struct GlyphMetrics {
		int width, height;
		int H_advance, H_bearingX, H_bearingY;
		int V_advance, V_bearingX, V_bearingY;
		bool valid;
	};

	struct TextSettings {
		FontSize size;
		bool wordWrap = false;
		int wordWrapWidth = 0;
	};

	struct TextLayout {
		struct GlyphInstance {
			GlyphCodepoint codepoint;
			Math::Recti rect;
			int index; // original index
		};

		Font* font;
		bool no_glyphs;
		TextSettings settings;
		std::vector<GlyphInstance> glyphs;
		Math::Vec2i boundingSize;
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

			BitmapRGBA bitmap;
			GlyphMetrics metrics;

			if (font->getGlyph(38, 100, GlyphRenderMode::COLOR, bitmap, metrics)) { // U+0026 or 38 or &
				bitmap.savePNG("&.png");
			}
			@endcode
		*/
		bool getGlyph(GlyphCodepoint codepoint, FontSize size, GlyphRenderMode mode, BitmapRGBA& bitmap, GlyphMetrics& metrics);

		bool getGlyphMetrics(GlyphCodepoint codepoint, const TextSettings& settings, GlyphMetrics& metrics);

		TextLayout generateTextLayout(const std::string& text, const TextSettings& textSettings, bool skip_glyphs = false);

		/// Kerning distance between left and right glyphs at a specific size in unfitted pixels
		int getHorizontalKerning(FontSize size, GlyphCodepoint left, GlyphCodepoint right);

		/// Returns whether the provided codepoint has emoji presentation according to the Unicode spec
		static bool HasEmojiPresentation(GlyphCodepoint codepoint);

	private:
		static FT_Library s_FTLibrary;

		std::vector<char> m_FileBuffer;
		FT_Face m_Face;

		/// internal function to set the current freetype pixel sizes
		bool setSize(FontSize size);
		/// cache the selected size to prevent calling freetype multiple times
		FontSize m_CurrentSize;

		typedef uint32_t GlyphHash; // codepoint + size
		std::unordered_map<GlyphHash, GlyphMetrics> m_MetricsCache;
	};
} }

#endif