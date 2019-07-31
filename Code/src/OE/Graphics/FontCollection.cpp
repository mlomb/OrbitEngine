#include "OE/Graphics/FontCollection.hpp"

#include <codecvt>

#include "OE/System/IOStream.hpp"
#include "OE/Graphics/TextureAtlas.hpp"
#include "OE/Graphics/2D/SpriteRenderer.hpp"

#undef GetObject

namespace OrbitEngine {	namespace Graphics {
	
	FrameIndex toIndex(GlyphCodepoint code, GlyphRenderMode mode) {
		const int MAX_UNICODE = 0x10FFFF;
		return mode * MAX_UNICODE + code;
	}

	FontCollection* FontCollection::Load(const std::string& font_metadata, const std::string& atlas_metadata, const std::string& atlas_image)
	{
		rapidjson::Document meta = Misc::ReadJSON(font_metadata);
		if (!meta.IsObject())
			return nullptr;

		TextureAtlas* texture_atlas = TextureAtlas::Load(atlas_metadata, atlas_image);
		if (!texture_atlas)
			return nullptr;

		FontCollection* collection = new FontCollection(texture_atlas);
		collection->loadMetadata(meta.GetObject());
		return collection;
	}

	bool FontCollection::addGlyph(Font* font, GlyphCodepoint codepoint, FontSize size, GlyphRenderMode mode)
	{
		if (!font)
			return false;

		if (entryExists(codepoint, mode))
			return false;

		Entry entry;

		if (!font->getGlyph(codepoint, size, mode, entry.bitmap, entry.metrics))
			return false;

		m_Collection[codepoint].emplace(mode, entry);

		return true;
	}

	int FontCollection::addGlyphs(Font* font, const std::vector<GlyphCodepoint>& codepoints, FontSize size, GlyphRenderMode mode)
	{
		int c = 0;
		for (GlyphCodepoint codepoint : codepoints) {
			if (addGlyph(font, codepoint, size, mode)) {
				c++;

				Entry& entry = m_Collection[codepoint][mode];
				
				for (GlyphCodepoint codepoint_right : codepoints) {
					int k = font->getHorizontalKerning(size, codepoint, codepoint_right);
					if (k != 0)
						entry.kernings[codepoint_right] = k;
				}
			}
		}
		return c;
	}

	void FontCollection::drawText(const std::vector<GlyphCodepoint>& text, float size, const Math::Vec2f& position, SpriteRenderer& sr) {
		// TODO: Apply size to the mix
		// TODO: Use the different render modes
		// TODO: Make a Text shader...

		if (text.size() == 0)
			return;

		Math::Vec2f pen(0, 0/*m_Instance->m_MaxBearing - getHeight(string) + m_Instance->m_MaxHeight*/);
		pen.x += position.x;
		pen.y += position.y;

		for (unsigned int i = 0; i < text.size(); i++) {
			GlyphCodepoint code = text[i];
			if (code == '\n' || code == '\r') {
				pen.x = position.x;
				pen.y += size;
				continue;
			}

			const auto& it = m_Collection.find(code);
			if (it == m_Collection.end())
				continue;

			const auto& p = *(*it).second.rbegin();
			GlyphRenderMode mode = p.first;
			FrameIndex index = toIndex(code, mode);
			const Entry& entry = p.second;

			if (m_TextureAtlas->hasFrame(index) && entry.metrics.width > 0 && entry.metrics.height > 0) {
				Math::Vec2f pos(pen.x + entry.metrics.H_bearingX, pen.y - entry.metrics.H_bearingY);
				pos.x = (float)(int)pos.x;
				pos.y = (float)(int)pos.y;

				m_TextureAtlas->drawFrame(index, pos, Math::Vec2f(entry.metrics.width, entry.metrics.height), sr);
			}
			
			if (i + 1 < text.size()) {
				const auto& kit = entry.kernings.find((GlyphCodepoint)text[i + 1]);
				if(kit != entry.kernings.end())
					pen.x += (*kit).second;
			}
			
			pen.x += entry.metrics.H_advance;
		}
	}

	Math::Vec2i FontCollection::getBounds(const std::vector<GlyphCodepoint>& text, float size)
	{
		if (text.size() == 0)
			return Math::Vec2i(0, 0);

		float maxWidth = 0;
		float x = 0;
		int lines = 1;

		for (unsigned int i = 0; i < text.size(); i++) {
			GlyphCodepoint c = text[i];
			if (c == '\n' || c == '\r') {
				lines++;
				x = 0;
				continue;
			}

			/*
			if (x > 0)
				x += getHorizontalKerning(text[i - 1], c);
			x += m_Glyphs[c].H_advance;
			*/

			if (x > maxWidth)
				maxWidth = x;
		}

		return Math::Vec2i(maxWidth, /* m_MaxHeight * */ lines);
	}

	bool FontCollection::exportToFiles(const std::string& font_metadata, const std::string& atlas_metadata, const std::string& atlas_image)
	{
		// generate atlas
		std::map<FrameIndex, BitmapRGBA> bitmaps;
		for (auto& p : m_Collection)
			for (auto& p2 : p.second)
				bitmaps.emplace(toIndex(p.first, p2.first), p2.second.bitmap);
		BitmapAtlasRGBA* atlas = BitmapAtlasRGBA::Generate(bitmaps, 2048, 1);
		if (!atlas)
			return false;
		// save atlas
		if (!atlas->exportToFiles(atlas_metadata, atlas_image)) {
			delete atlas;
			bitmaps.clear();
			return false;
		}
		delete atlas;
		bitmaps.clear();

		// generate font metadata
		rapidjson::StringBuffer buffer;
		Misc::JSONWriter writer(buffer);
		writeMetadata(writer);
		return Misc::WriteJSON(font_metadata, buffer);
	}

	FontCollection::FontCollection(TextureAtlas* atlas)
		: m_TextureAtlas(atlas)
	{
	}

	FontCollection::FontCollection()
		: m_TextureAtlas(NULL)
	{
	}

	FontCollection::~FontCollection()
	{
		if(m_TextureAtlas)
			delete m_TextureAtlas;
	}

	void FontCollection::writeMetadata(Misc::JSONWriter& writer)
	{
		writer.StartObject();
		
		{
			writer.Key("glyphs");
			writer.StartArray();

			for (auto& p : m_Collection) {
				for (auto& p2 : p.second) {
					writer.StartObject();
					writer.Key("i");
					writer.Uint(p.first);
					writer.Key("m");
					writer.Int(p2.first);

					const GlyphMetrics& metrics = p2.second.metrics;
					writer.Key("w");
					writer.Int(metrics.width);
					writer.Key("h");
					writer.Int(metrics.height);
					writer.Key("ha");
					writer.Int(metrics.H_advance);
					writer.Key("va");
					writer.Int(metrics.V_advance);
					writer.Key("hbx");
					writer.Int(metrics.H_bearingX);
					writer.Key("hby");
					writer.Int(metrics.H_bearingY);
					writer.Key("vbx");
					writer.Int(metrics.V_bearingX);
					writer.Key("vby");
					writer.Int(metrics.V_bearingY);

					if (p2.second.kernings.size() > 0) {
						writer.Key("k");
						writer.StartArray();
						for (auto& pk : p2.second.kernings) {
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
			}

			writer.EndArray();
		}

		writer.EndObject();
	}

	void FontCollection::loadMetadata(const rapidjson::Value& data)
	{
		if (data.HasMember("glyphs")) {
			const rapidjson::Value& glyphs = data["glyphs"];
			if (glyphs.IsArray()) {
				for (rapidjson::SizeType i = 0; i < glyphs.Size(); ++i) {
					const rapidjson::Value& glyph = glyphs[i];
					if (glyph.IsObject()) {
						if (glyph.HasMember("i") &&
							glyph.HasMember("m") &&
							glyph.HasMember("w") &&
							glyph.HasMember("h") &&
							glyph.HasMember("ha") &&
							glyph.HasMember("va") &&
							glyph.HasMember("hbx") &&
							glyph.HasMember("hby") &&
							glyph.HasMember("vbx") &&
							glyph.HasMember("vby") &&

							glyph["i"].IsUint() &&
							glyph["m"].IsInt() &&
							glyph["w"].IsInt() &&
							glyph["h"].IsInt() &&
							glyph["ha"].IsInt() &&
							glyph["va"].IsInt() &&
							glyph["hbx"].IsInt() &&
							glyph["hby"].IsInt() &&
							glyph["vbx"].IsInt() &&
							glyph["vby"].IsInt()) {
							GlyphCodepoint code = (GlyphCodepoint)glyph["i"].GetUint();
							GlyphRenderMode mode = static_cast<GlyphRenderMode>(glyph["m"].GetInt());
							if (entryExists(code, mode)) {
								OE_LOG_WARNING("a glyph has been found twice, skipping");
								continue;
							}

							Entry entry;
							entry.metrics.width = glyph["w"].GetInt();
							entry.metrics.height = glyph["h"].GetInt();
							entry.metrics.H_advance = glyph["ha"].GetInt();
							entry.metrics.V_advance = glyph["va"].GetInt();
							entry.metrics.H_bearingX = glyph["hbx"].GetInt();
							entry.metrics.H_bearingY = glyph["hby"].GetInt();
							entry.metrics.V_bearingX = glyph["vbx"].GetInt();
							entry.metrics.V_bearingY = glyph["vby"].GetInt();

							// load kernings
							if (glyph.HasMember("k")) {
								const rapidjson::Value& kernings = glyph["k"];
								if (kernings.IsArray()) {
									for (rapidjson::SizeType j = 0; j < kernings.Size(); ++j) {
										const rapidjson::Value& kerning = kernings[j];
										if (kerning.IsObject()) {
											if (kerning.HasMember("i") &&
												kerning.HasMember("d") &&

												kerning["i"].IsUint() &&
												kerning["d"].IsInt()) {
												entry.kernings.emplace((GlyphCodepoint)kerning["i"].GetUint(), kerning["d"].GetInt());
											}
											else OE_LOG_WARNING("a kerning entry is malformed");
										}
										else OE_LOG_WARNING("a kerning entry is not an object");
									}
								}
								else OE_LOG_WARNING("a kerning entry is not an array");
							}

							m_Collection[code].emplace(mode, entry);
						}
						else OE_LOG_WARNING("a glyph is malformed");
					}
					else OE_LOG_WARNING("a glyph is not an object");
				} // for
			}
			else OE_LOG_WARNING("glyphs is not an array");
		}
	}

	bool FontCollection::entryExists(GlyphCodepoint code, GlyphRenderMode mode) const
	{
		const auto& it = m_Collection.find(code);
		if (it == m_Collection.end())
			return false;
		const auto& modes = (*it).second;
		if (modes.find(mode) == modes.end())
			return false;
		return true;
	}

} }
