#ifndef GRAPHICS_FONT_COLLECTION_HPP
#define GRAPHICS_FONT_COLLECTION_HPP

#include <string>
#include <map>

#include "Font.hpp"
#include "BitmapAtlas.hpp"

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine {	namespace Graphics {
	class TextureAtlas;
	class SpriteRenderer;

	class FontCollection {
	public:
		FontCollection();
		~FontCollection();

		static FontCollection* Load(const std::string& font_metadata, const std::string& atlas_metadata, const std::string& atlas_image);

		void drawString(const std::vector<GlyphCodepoint>& text, float size, const Math::Vec2f& position, SpriteRenderer& sr);
		Math::Vec2i getBounds(const std::wstring& text);

		bool addGlyph(Font* font, GlyphCodepoint codepoint, FontSize size, GlyphRenderMode mode);
		int addGlyphs(Font* font, const std::vector<GlyphCodepoint>& codepoints, FontSize size, GlyphRenderMode mode);

		bool exportToFiles(const std::string& font_metadata, const std::string& atlas_metadata, const std::string& atlas_image);

	private:
		FontCollection(TextureAtlas* atlas);

		struct Entry {
			GlyphMetrics metrics;
			std::unordered_map<GlyphCodepoint, int> kernings; // in pixels

			// only valid during build
			BitmapRGBA bitmap;
		};

		void writeMetadata(Misc::JSONWriter& writer);
		void loadMetadata(const rapidjson::Value& data);

		bool entryExists(GlyphCodepoint code, GlyphRenderMode mode) const;

		TextureAtlas* m_TextureAtlas;
		std::unordered_map<GlyphCodepoint, std::map<GlyphRenderMode, Entry>> m_Collection;
	};

} }

#endif