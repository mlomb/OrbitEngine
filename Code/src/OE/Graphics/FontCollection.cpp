#include "OE/Graphics/FontCollection.hpp"

#include "OE/System/IOStream.hpp"

namespace OrbitEngine {	namespace Graphics {

	bool FontCollection::addGlyph(Font* font, GlyphCodepoint codepoint, FontSize size, GlyphRenderMode mode)
	{
		if (!font)
			return false;

		EntryKey key = std::make_pair(codepoint, mode);
		if (m_Collection.find(key) != m_Collection.end())
			return false;

		Entry entry;

		if (!font->getGlyph(codepoint, size, mode, entry.bitmap, entry.metrics))
			return false;

		m_Collection.emplace(key, entry);

		return true;
	}

	int FontCollection::addGlyphs(Font* font, const std::vector<GlyphCodepoint>& codepoints, FontSize size, GlyphRenderMode mode)
	{
		int c = 0;
		for (GlyphCodepoint codepoint : codepoints) {
			if (addGlyph(font, codepoint, size, mode)) {
				c++;

				Entry& entry = m_Collection.find(std::make_pair(codepoint, mode))->second;
				
				for (GlyphCodepoint codepoint_right : codepoints) {
					if (codepoint == codepoint_right) continue;

					int k = font->getHorizontalKerning(size, codepoint, codepoint_right);
					if(k != 0)
						entry.kernings[codepoint_right] = k;
				}
			}
		}
		return c;
	}

	bool FontCollection::exportCollection(const std::string& path)
	{
		// Generate atlas
		BitmapAtlasRGBA* atlas = generateAtlas();
		if (!atlas)
			return false;

		// Generate metadata
		rapidjson::StringBuffer buffer;
		json_writer writer(buffer);
		writeMetadata(writer, atlas);

		System::IOStream* stream = new System::IOStream(path + ".json", System::AccessMode::WRITE);
		stream->write((unsigned char*)buffer.GetString(), sizeof(unsigned char), buffer.GetSize());
		delete stream;

		// save
		atlas->savePNG(path + ".png");
		return true;
	}

	BitmapAtlasRGBA* FontCollection::generateAtlas()
	{
		std::map<FrameIndex, BitmapRGBA> bitmaps;
		for (auto& p : m_Collection) {
			bitmaps.emplace(p.first.first, p.second.bitmap);
		}
		return BitmapAtlasRGBA::Generate(bitmaps, 2048, 0);
	}

	void FontCollection::writeMetadata(json_writer& writer, BitmapAtlasRGBA* atlas)
	{
		writer.StartObject();
		
		{
			writer.Key("glyphs");
			writer.StartArray();

			for (auto& p : m_Collection) {
				writer.StartObject();
				writer.Key("i");
				writer.Uint(p.first.first);
				writer.Key("m");
				writer.Int(p.first.second);

				if (p.second.kernings.size() > 0) {
					writer.Key("k");
					writer.StartArray();
					for (auto& pk : p.second.kernings) {
						writer.StartObject();
						writer.Key("i");
						writer.Uint(pk.first);
						writer.Key("d");
						writer.Int(pk.second);
						writer.EndObject();
					}
					writer.EndArray();
				}

				writer.EndObject();
			}

			writer.EndArray();
		}

		writer.Key("atlas");
		atlas->writeMetadata(writer);

		writer.EndObject();
	}

} }
