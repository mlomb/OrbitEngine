#ifndef GRAPHICS_FONT_COLLECTION_HPP
#define GRAPHICS_FONT_COLLECTION_HPP

#include <string>
#include <map>

#include "Font.hpp"
#include "BitmapAtlas.hpp"

namespace OrbitEngine {	namespace Graphics {

	class FontCollection {
	public:
		bool addGlyph(Font* font, GlyphCodepoint codepoint, FontSize size, GlyphRenderMode mode);
		int addGlyphs(Font* font, const std::vector<GlyphCodepoint>& codepoints, FontSize size, GlyphRenderMode mode);

		bool exportCollection(const std::string& path);

	private:
		struct Entry {
			GlyphMetrics metrics;
			std::map<GlyphCodepoint, int> kernings; // in pixels

			// only valid during build
			BitmapRGBA bitmap;
		};

		typedef std::pair<GlyphCodepoint, GlyphRenderMode> EntryKey;

		BitmapAtlasRGBA* generateAtlas();
		void writeMetadata(json_writer& writer, BitmapAtlasRGBA* atlas);

		std::map<EntryKey, Entry> m_Collection;
	};

} }

#endif