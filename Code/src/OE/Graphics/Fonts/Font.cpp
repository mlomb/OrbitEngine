#include "OE/Graphics/Fonts/Font.hpp"

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "OE/Misc/RectsPacker.hpp"
#include "OE/Graphics/Fonts/MSDF.hpp"
#include "OE/Graphics/Images/BitmapAtlas.hpp"
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
			OE_LOG_INFO("FreeType initialized");
		}

		m_FileBuffer = System::File::LoadFile(file);
		FT_New_Memory_Face(Font::s_FTLibrary, (FT_Byte*)m_FileBuffer.data(), (FT_Long)m_FileBuffer.size(), 0, &m_Face);
	}

	Font::~Font() {
		if (m_Face)
			FT_Done_Face(m_Face);
		m_FileBuffer.clear();
	}
	
	FontInstance* Font::generateInstance(FontSize size, FontAtlasMode mode, const std::string& charset)
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

	FontInstance* Font::loadInstance(FontSize size, FontAtlasMode mode, const std::string& charset)
	{
		// TODO: Implement
		return nullptr;
	}

	FontInstance* Font::initInstance(FontSize size, const std::string& charset)
	{
		if (FT_Set_Pixel_Sizes(m_Face, 0, (FT_UInt)size) != 0) {
			OE_LOG_ERROR("Could not set pixel size!");
			return nullptr;
		}

		int maxHeight = 0, minHeight = 0;

		FontInstance* instance = new FontInstance();
		instance->m_Size = size;

		FT_Vector kerning;

		for (GlyphIndex c : charset) {
			if (FT_Load_Char(m_Face, c, FT_LOAD_DEFAULT) != 0) {
				OE_LOG_WARNING("Could not load char " << c << "!");
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

			instance->m_Glyphs[c] = g;

			int cind = FT_Get_Char_Index(m_Face, c);

			for (GlyphIndex c2 : charset) {
				FT_Get_Kerning(m_Face, c, FT_Get_Char_Index(m_Face, c2), FT_KERNING_UNFITTED, &kerning);
				if (kerning.x != 0) {
					instance->m_Kernings.insert(std::make_pair(std::make_pair(c, c2), kerning.x >> 6));
				}
			}
		}
		
		instance->m_MaxHeight = abs(maxHeight) + abs(minHeight);
		instance->m_MaxBearing = minHeight;

		return instance;
	}
} }