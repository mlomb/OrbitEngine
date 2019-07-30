#include "OE/Graphics/Font.hpp"

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "OE/System/File.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Misc/RectsPacker.hpp"

#include "OE/Graphics/MSDF.hpp"
#include "OE/Graphics/BitmapAtlas.hpp"

namespace OrbitEngine { namespace Graphics {
	FT_Library Font::s_FTLibrary = nullptr;

	Font::Font(const std::string& file)
		: m_Face(NULL), m_CurrentSize(0)
	{
		if (Font::s_FTLibrary == 0) {
			if (FT_Init_FreeType(&Font::s_FTLibrary) != 0) {
				OE_LOG_FATAL("Couldn't initialize FreeType library");
				return;
			}
			OE_LOG_INFO("FreeType " << FREETYPE_MAJOR << "." << FREETYPE_MINOR << "." << FREETYPE_PATCH << " initialized");
		}

		m_FileBuffer = System::File::Read(file);
		FT_New_Memory_Face(Font::s_FTLibrary, (FT_Byte*)m_FileBuffer.data(), (FT_Long)m_FileBuffer.size(), 0, &m_Face);
		FT_Select_Charmap(m_Face, FT_ENCODING_UNICODE);
	}

	Font::~Font() {
		if (m_Face)
			FT_Done_Face(m_Face);
		m_FileBuffer.clear();
	}

	bool Font::setSize(FontSize size)
	{
		FT_Error error;

		if (FT_HAS_FIXED_SIZES(m_Face)) {
			// fixed size font, ignore given size
			size = m_Face->available_sizes[0].height;
		}

		if (size == m_CurrentSize)
			return true;

		if (FT_HAS_FIXED_SIZES(m_Face)) {
			if ((error = FT_Select_Size(m_Face, 0)) != FT_Err_Ok) {
				OE_LOG_WARNING("Could not select fixed pixel size! " << FT_Error_String(error));
				return false;
			}
		}
		else {
			if ((error = FT_Set_Pixel_Sizes(m_Face, 0, (FT_UInt)size)) != FT_Err_Ok) {
				OE_LOG_WARNING("Could not set pixel size!" << FT_Error_String(error));
				return false;
			}
		}

		m_CurrentSize = size;

		return true;
	}

	std::vector<GlyphCodepoint> Font::getAvailableGlyphs() const
	{
		std::vector<GlyphCodepoint> result;

		FT_UInt codepoint;
		FT_ULong charcode = FT_Get_First_Char(m_Face, &codepoint);

		while (codepoint != 0) {
			result.push_back(charcode);
			charcode = FT_Get_Next_Char(m_Face, charcode, &codepoint);
		}

		return result;
	}

	bool Font::getGlyph(GlyphCodepoint codepoint, FontSize size, GlyphRenderMode mode, BitmapRGBA& bitmap, GlyphMetrics& metrics)
	{
		if (!setSize(size))
			return false;

		FT_Error error;
		bool sdf = mode != GRAY && mode != COLOR;
		FT_Int32 load_flags = sdf ? FT_LOAD_DEFAULT : FT_LOAD_RENDER;
		if (!sdf && FT_HAS_COLOR(m_Face) && mode == COLOR) // render colored glyphs like emojis
			load_flags |= FT_LOAD_COLOR;

		if ((error = FT_Load_Glyph(m_Face, FT_Get_Char_Index(m_Face, codepoint), load_flags)) != FT_Err_Ok) {
			OE_LOG_WARNING("Could not load glyph " << codepoint << "! " << FT_Error_String(error));
			return false;
		}

		// load metrics
		metrics = {};
		metrics.width      = (m_Face->glyph->metrics.width        >> 6); // (>> 6) = (/ 64)
		metrics.height     = (m_Face->glyph->metrics.height       >> 6);
		metrics.H_advance  = (m_Face->glyph->metrics.horiAdvance  >> 6);
		metrics.H_bearingX = (m_Face->glyph->metrics.horiBearingX >> 6);
		metrics.H_bearingY = (m_Face->glyph->metrics.horiBearingY >> 6);
		metrics.V_advance  = (m_Face->glyph->metrics.vertAdvance  >> 6);
		metrics.V_bearingX = (m_Face->glyph->metrics.vertBearingX >> 6);
		metrics.V_bearingY = (m_Face->glyph->metrics.vertBearingY >> 6);

		// render glyph
		if (sdf) {
			const double sdf_range = 16.0;
			SDFMode sdf_mode;
			switch (mode) {
			default:
			case SDF: sdf_mode = SDFMode::SDF; break;
			case PSDF: sdf_mode = SDFMode::PSDF; break;
			case MSDF: sdf_mode = SDFMode::MSDF; break;
			}
			BitmapRGB rgb = GenerateBitmapFromOutline(&m_Face->glyph->outline, sdf_mode, sdf_range);
			if (!rgb.valid())
				return false;
			bitmap = ConvertBitmap<unsigned char, 3, unsigned char, 4>(rgb);
			return true;
		}
		else {
			if (m_Face->glyph->bitmap.buffer == nullptr)
				return false;

			FT_Pixel_Mode pixel_mode = (FT_Pixel_Mode)m_Face->glyph->bitmap.pixel_mode;
			unsigned int w = m_Face->glyph->bitmap.width;
			unsigned int h = m_Face->glyph->bitmap.rows;

			switch (pixel_mode) {
			case FT_Pixel_Mode::FT_PIXEL_MODE_GRAY:
			{
				Bitmap<unsigned char, 1> b(m_Face->glyph->bitmap.buffer, w, h); // no copy
				bitmap = ConvertBitmap<unsigned char, 1, unsigned char, 4>(b);
				return true;
			}
			case FT_Pixel_Mode::FT_PIXEL_MODE_BGRA:
			{
				Bitmap<unsigned char, 4> b(m_Face->glyph->bitmap.buffer, w, h); // no copy
				bitmap = ConvertBGRAtoRGBA(b);
				return true;
			}
			}
		}

		return false;
	}

	int Font::getHorizontalKerning(FontSize size, GlyphCodepoint left, GlyphCodepoint right)
	{
		if (!setSize(size))
			return 0;

		FT_Vector kerning;
		FT_Get_Kerning(m_Face, FT_Get_Char_Index(m_Face, left), FT_Get_Char_Index(m_Face, right), FT_KERNING_UNFITTED, &kerning);
		return kerning.x;
	}
} }