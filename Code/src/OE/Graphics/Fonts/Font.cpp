#include "OE/Graphics/Font.hpp"

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "OE/Misc/RectsPacker.hpp"
#include "OE/Graphics/MSDF.hpp"
#include "OE/Graphics/BitmapAtlas.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/System/File.hpp"

namespace OrbitEngine { namespace Graphics {
	FT_Library Font::s_FTLibrary = nullptr;

	Font::Font(const std::string& file)
		: m_Face(NULL)
	{
		if (Font::s_FTLibrary == 0) {
			if (FT_Init_FreeType(&Font::s_FTLibrary) != 0) {
				OE_LOG_FATAL("Couldn't initialize FreeType library");
				return;
			}
			OE_LOG_INFO("FreeType " << FREETYPE_MAJOR << "." << FREETYPE_MINOR << "." << FREETYPE_PATCH << " initialized");
		}

		m_FileBuffer = System::File::LoadFile(file);
		FT_New_Memory_Face(Font::s_FTLibrary, (FT_Byte*)m_FileBuffer.data(), (FT_Long)m_FileBuffer.size(), 0, &m_Face);
	}

	Font::~Font() {
		if (m_Face)
			FT_Done_Face(m_Face);
		m_FileBuffer.clear();
	}

	std::wstring Font::getFullCharset() const
	{
		std::wstring result;

		FT_ULong charcode;
		FT_UInt gindex;
		charcode = FT_Get_First_Char(m_Face, &gindex);
		while (gindex != 0)
		{
			result.push_back((wchar_t)charcode);
			charcode = FT_Get_Next_Char(m_Face, charcode, &gindex);
		}

		return result;
	}
	
	FontInstance* Font::generateInstance(FontSize size, FontAtlasMode mode, const std::wstring& charset)
	{
		FontInstance* instance = initInstance(size, charset);

		if (!instance)
			return nullptr;

		// Font generation target is BitmapRGB

		std::vector<std::pair<std::string, BitmapRGB>> bitmaps;

		std::string key = "X";
		bool sdf = mode != BITMAP;

		for (auto& pair : instance->getGlyphs()) {
			GlyphIndex index = pair.first;

			if (FT_Load_Char(m_Face, index, sdf ? FT_LOAD_DEFAULT : FT_LOAD_RENDER) != 0)
				continue;

			key[0] = index; // TODO: Handle unicode

			if (sdf) {
				const double sdf_range = 16.0;
				SDFMode sdf_mode{};
				switch (mode) {
				case SDF: sdf_mode = SDFMode::SDF; break;
				case PSDF: sdf_mode = SDFMode::PSDF; break;
				case MSDF: sdf_mode = SDFMode::MSDF; break;
				}
				bitmaps.emplace_back(key, GenerateBitmapFromOutline(&m_Face->glyph->outline, sdf_mode, sdf_range));
			}
			else {
				if (m_Face->glyph->bitmap.buffer == nullptr) {
					continue;
				}

				unsigned int w = m_Face->glyph->bitmap.width;
				unsigned int h = m_Face->glyph->bitmap.rows;

				Bitmap<unsigned char, 1> b(w, h, m_Face->glyph->bitmap.buffer);
				bitmaps.emplace_back(key, ConvertBitmap<unsigned char, 1, unsigned char, 3>(b));
			}
		}

		BitmapAtlas<unsigned char, 3>* atlas = BitmapAtlas<unsigned char, 3>::Generate(bitmaps, 2048, 0);
		atlas->savePNG("atlas.png");

		return instance;
	}

	FontInstance* Font::loadInstance(FontSize size, FontAtlasMode mode, const std::wstring& charset)
	{
		// TODO: Implement
		return nullptr;
	}

	FontInstance* Font::initInstance(FontSize size, const std::wstring& charset)
	{
		if (FT_Set_Pixel_Sizes(m_Face, 0, (FT_UInt)size) != 0) {
			OE_LOG_ERROR("Could not set pixel size!");
			return nullptr;
		}

		int maxHeight = 0, minHeight = 0;

		FontInstance* instance = new FontInstance();
		instance->m_Size = size;

		FT_Vector kerning;

		for (wchar_t c : charset) {
			GlyphIndex char_code = (GlyphIndex)c;
			if (FT_Load_Char(m_Face, char_code, FT_LOAD_DEFAULT) != 0) {
				OE_LOG_WARNING("Could not load char " << char_code << "!");
				continue;
			}

			int width = (m_Face->glyph->metrics.width >> 6); // (>> 6) = (/ 64)
			int height = (m_Face->glyph->metrics.height >> 6);
			int H_advance = (m_Face->glyph->metrics.horiAdvance >> 6);
			int H_bearingX = (m_Face->glyph->metrics.horiBearingX >> 6);
			int H_bearingY = (m_Face->glyph->metrics.horiBearingY >> 6);
			int V_advance = (m_Face->glyph->metrics.vertAdvance >> 6);
			int V_bearingX = (m_Face->glyph->metrics.vertBearingX >> 6);
			int V_bearingY = (m_Face->glyph->metrics.vertBearingY >> 6);

			int offset = H_bearingY - height;
			if (offset < minHeight)
				minHeight = offset;
			if (height > maxHeight)
				maxHeight = height;

			Glyph g = {};

			g.width = width;
			g.height = height;
			g.H_advance = H_advance;
			g.H_bearingX = H_bearingX;
			g.H_bearingY = H_bearingY;
			g.V_advance = V_advance;
			g.V_bearingX = V_bearingX;
			g.V_bearingY = V_bearingY;

			instance->m_Glyphs[char_code] = g;

			int cind = FT_Get_Char_Index(m_Face, char_code);

			for (GlyphIndex c2 : charset) {
				FT_Get_Kerning(m_Face, char_code, FT_Get_Char_Index(m_Face, c2), FT_KERNING_UNFITTED, &kerning);
				if (kerning.x != 0) {
					instance->m_Kernings.insert(std::make_pair(std::make_pair(char_code, c2), kerning.x >> 6));
				}
			}
		}
		
		instance->m_MaxHeight = abs(maxHeight) + abs(minHeight);
		instance->m_MaxBearing = minHeight;

		return instance;
	}
} }