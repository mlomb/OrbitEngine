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

	/**
		@brief A collection of Glyphs and metadata

		Example: create a collection of text and emojis (no SDF)
		@code
		const FontSize size = 22;

		Font* font = new Font("latin.ttf");
		Font* font_emojis = new Font("emojis.ttf");

		// create an empty collection
		FontCollection* collection = new FontCollection();

		// add all the glyphs available in the text font
		collection->addGlyphs(font, font->getAvailableGlyphs(), size, GlyphRenderMode::COLOR);

		// filter the emoji font to only keep true emojis
		auto emojis = font_emojis->getAvailableGlyphs();
		emojis.erase(std::remove_if(emojis.begin(), emojis.end(), [](const GlyphCodepoint& code) {
			return !Font::HasEmojiPresentation(code);
		}), emojis.end());

		// add the glyphs to the collection
		collection->addGlyphs(emojis_font, emojis, size, GlyphRenderMode::COLOR);

		// export
		collection->exportToFiles("font-collection-fnt.json", "font-collection.json", "font-collection.png");

		// cleanup
		delete collection;
		@endcode

		The resulting **font-collection.png**:
		@image html font-collection.png width=300px

		Metadata format:
		@code{json}
		{
			"glyphs": [
				{
					"i": 87, // codepoint
					"m": 4, // render mode
					"s": 22, // original font size
					"w": 19, // width
					"h": 14, // height
					"ha": 19, // horizontal advance
					"va": 19, // vertical advance
					"hbx": 0, // horizontal bearing X
					"hby": 14, // horizontal bearing Y
					"vbx": -10, // vertical bearing X
					"vby":2, // vertical bearing Y
					"k": [ // kernings, can be missing
						{
							"i": 228, // right codepoint
							"d": -1 // kerning distance
						},
						...
					]
				},
				...
			]
		}
		@endcode
		@see GlyphMetrics for more information about the metrics generated for each glyph
	*/
	class FontCollection {
	public:
		/// Creates an empty collection
		FontCollection();
		~FontCollection();

		/**
			@brief Loads a font collection from disk
			@param[in] font_metadata path to the font metadata file to read (.json)
			@param[in] atlas_metadata path to the atlas metadata fke to read (.json)
			@param[in] atlas_image path to the atlas image file to read (.png)
			@return The FontCollection instance if the operation was successful, NULL otherwise
			@see FontCollection for the font's metadata format
			@see Atlas for the atlas's metadata format
			@note Imported collection **can't** be modified, you can't add new glyphs nor export it again
		*/
		static FontCollection* Load(const std::string& font_metadata, const std::string& atlas_metadata, const std::string& atlas_image);

		/**
			@brief Add a single glyph to the collection
			@param[in] font Font to extract the glyph
			@param[in] codepoint glyph unicode's codepoint
			@param[in] size target size to generate the glyph
			@param[in] mode mode used to render the glyph
			@return Whether the glyph has been generated and added successfully
			@note Adding the glyphs one by one prevents kerning generation. Better use \ref addGlyphs for multiple
				  glyphs in the same font
		*/
		bool addGlyph(Font* font, GlyphCodepoint codepoint, FontSize size, GlyphRenderMode mode);

		/**
			@brief Add a group of glyph to the collection
			@param[in] font Font to extract the glyphs
			@param[in] codepoints list of the glyphs code points to add
			@param[in] size target size to generate the glyphs
			@param[in] mode mode used to render the glyphs
			@return The number of glyphs that have been generated and added successfully
			@note This function will generate kerning information between the glyphs provided
		*/
		int addGlyphs(Font* font, const std::vector<GlyphCodepoint>& codepoints, FontSize size, GlyphRenderMode mode);

		/**
			@brief Export the font collection to disk
			@param[in] font_metadata path to write the font metadata file (.json)
			@param[in] atlas_metadata path to write the atlas metadata file (.json)
			@param[in] atlas_image path to write the atlas image file (.png)
			@return Whether the collection was exported correctly
			@see Atlas for the atlas's metadata format
		*/
		bool exportToFiles(const std::string& font_metadata, const std::string& atlas_metadata, const std::string& atlas_image);

		static FrameIndex toIndex(GlyphCodepoint code, GlyphRenderMode mode);

	private:
		FontCollection(TextureAtlas* atlas);

		/// Represents a specific glyph
		struct Entry {
			GlyphMetrics metrics;
			std::unordered_map<GlyphCodepoint, int> kernings; // in pixels
			FontSize original_size;

			// only valid during build
			BitmapRGBA bitmap;
		};

		/// Generates JSON metadata
		void writeMetadata(Misc::JSONWriter& writer);
		/// Load JSON metadata
		void loadMetadata(const rapidjson::Value& data);

		/// Checks if an entry exists
		bool entryExists(GlyphCodepoint code, GlyphRenderMode mode) const;

		/// Pointer to the atlas texture. May be NULL if the collection is not built
		TextureAtlas* m_TextureAtlas;
		std::unordered_map<GlyphCodepoint, std::map<GlyphRenderMode, Entry>> m_Collection;

		friend class TextRenderer2D;
	};

} }

#endif