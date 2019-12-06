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
		if(m_Face)
			FT_Select_Charmap(m_Face, FT_ENCODING_UNICODE);
	}

	Font::~Font() {
		if (m_Face)
			FT_Done_Face(m_Face);
		m_FileBuffer.clear();
	}

	bool Font::setSize(FontSize size)
	{
		if (!m_Face)
			return false;

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

		if (m_Face) {
			FT_UInt codepoint;
			FT_ULong charcode = FT_Get_First_Char(m_Face, &codepoint);

			while (codepoint != 0) {
				result.push_back(charcode);
				charcode = FT_Get_Next_Char(m_Face, charcode, &codepoint);
			}
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
		
		// exception
		if (codepoint == ' ') {
			bitmap = BitmapRGBA();
			return true;
		}

		// render glyph
		if (sdf) {
			const double sdf_range = 10.0;
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
			bitmap.flipVertically();
			return true;
		}
		else {
			if (m_Face->glyph->bitmap.buffer == nullptr)
				return false;

			FT_Pixel_Mode pixel_mode = (FT_Pixel_Mode)m_Face->glyph->bitmap.pixel_mode;
			unsigned int w = m_Face->glyph->bitmap.width;
			unsigned int h = m_Face->glyph->bitmap.rows;

			// Fix for some fonts :( (ex TwemojiMozilla.ttf)
			if (metrics.width == 0 && metrics.height == 0) {
				metrics.height = size;
				metrics.width = (double)size / (double)((double)h / (double)w); // keep aspect
				metrics.H_bearingX = metrics.width * 0.1;
				metrics.H_bearingY = metrics.height * 0.9;
				metrics.H_advance = metrics.width * 1.1 + metrics.H_bearingX;
			}

			switch (pixel_mode) {
			case FT_Pixel_Mode::FT_PIXEL_MODE_GRAY:
			{
				Bitmap<unsigned char, 1> b(m_Face->glyph->bitmap.buffer, w, h); // no copy
				if (mode == COLOR) {
					// convert the grayscale image to white with the grayscale values applied to the alpha channel
					bitmap = ConvertAlphaToWhiteRGBA(b);
				}
				else {
					// just return the converted bitamp
					bitmap = ConvertBitmap<unsigned char, 1, unsigned char, 4>(b);
				}
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
		if (!FT_HAS_KERNING(m_Face))
			return 0;
		FT_Vector kerning;
		FT_Get_Kerning(m_Face, FT_Get_Char_Index(m_Face, left), FT_Get_Char_Index(m_Face, right), FT_KERNING_UNFITTED, &kerning);
		return kerning.x >> 6;
	}

	bool Font::HasEmojiPresentation(GlyphCodepoint c) {
		// source: http://unicode.org/Public/emoji/latest/emoji-data.txt
		if (c < 0x02728 - 1) // discard
			return false;
		return 
			 c == 0x02728 || c == 0x0274C  || c == 0x0274E ||
			(c >= 0x02753 && c <= 0x02755) || c == 0x02757 ||
			(c >= 0x02795 && c <= 0x02797) || c == 0x027B0 || c == 0x027BF ||
			(c >= 0x02B1B && c <= 0x02B1C) || c == 0x02B50 || c == 0x02B55 || c == 0x1F004 || c == 0x1F0CF || c == 0x1F18E ||
			(c >= 0x1F191 && c <= 0x1F19A) ||
			(c >= 0x1F1E6 && c <= 0x1F1FF) || c == 0x1F201 || c == 0x1F21A || c == 0x1F22F ||
			(c >= 0x1F232 && c <= 0x1F236) ||
			(c >= 0x1F238 && c <= 0x1F23A) ||
			(c >= 0x1F250 && c <= 0x1F251) ||
			(c >= 0x1F300 && c <= 0x1F320) ||
			(c >= 0x1F32D && c <= 0x1F335) ||
			(c >= 0x1F337 && c <= 0x1F37C) ||
			(c >= 0x1F37E && c <= 0x1F393) ||
			(c >= 0x1F3A0 && c <= 0x1F3CA) ||
			(c >= 0x1F3CF && c <= 0x1F3D3) ||
			(c >= 0x1F3E0 && c <= 0x1F3F0) || c == 0x1F3F4 ||
			(c >= 0x1F3F8 && c <= 0x1F43E) || c == 0x1F440 ||
			(c >= 0x1F442 && c <= 0x1F4FC) ||
			(c >= 0x1F4FF && c <= 0x1F53D) ||
			(c >= 0x1F54B && c <= 0x1F54E) ||
			(c >= 0x1F550 && c <= 0x1F567) || c == 0x1F57A ||
			(c >= 0x1F595 && c <= 0x1F596) || c == 0x1F5A4 ||
			(c >= 0x1F5FB && c <= 0x1F64F) ||
			(c >= 0x1F680 && c <= 0x1F6C5) || c == 0x1F6CC ||
			(c >= 0x1F6D0 && c <= 0x1F6D2) || c == 0x1F6D5 ||
			(c >= 0x1F6EB && c <= 0x1F6EC) ||
			(c >= 0x1F6F4 && c <= 0x1F6FA) ||
			(c >= 0x1F7E0 && c <= 0x1F7EB) ||
			(c >= 0x1F90D && c <= 0x1F93A) ||
			(c >= 0x1F93C && c <= 0x1F945) ||
			(c >= 0x1F947 && c <= 0x1F971) ||
			(c >= 0x1F973 && c <= 0x1F976) ||
			(c >= 0x1F97A && c <= 0x1F9A2) ||
			(c >= 0x1F9A5 && c <= 0x1F9AA) ||
			(c >= 0x1F9AE && c <= 0x1F9CA) ||
			(c >= 0x1F9CD && c <= 0x1F9FF) ||
			(c >= 0x1FA70 && c <= 0x1FA73) ||
			(c >= 0x1FA78 && c <= 0x1FA7A) ||
			(c >= 0x1FA80 && c <= 0x1FA82) ||
			(c >= 0x1FA90 && c <= 0x1FA95) ||
			(c >= 0x0231A && c <= 0x0231B) ||
			(c >= 0x023E9 && c <= 0x023EC) || c == 0x023F0 || c == 0x023F3 ||
			(c >= 0x025FD && c <= 0x025FE) ||
			(c >= 0x02614 && c <= 0x02615) ||
			(c >= 0x02648 && c <= 0x02653) || c == 0x0267F || c == 0x02693 || c == 0x026A1 ||
			(c >= 0x026AA && c <= 0x026AB) ||
			(c >= 0x026BD && c <= 0x026BE) ||
			(c >= 0x026C4 && c <= 0x026C5) || c == 0x026CE || c == 0x026D4 || c == 0x026EA ||
			(c >= 0x026F2 && c <= 0x026F3) || c == 0x026F5 || c == 0x026FA || c == 0x026FD || c == 0x02705 ||
			(c >= 0x0270A && c <= 0x0270B);
	}
} }