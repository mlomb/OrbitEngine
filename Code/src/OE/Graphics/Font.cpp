#include "OE/Graphics/Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "OE/Application/Context.hpp"

#include "OE/Graphics/API/Texture.hpp"
#include "OE/Graphics/2D/Renderer2D.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/System/File.hpp"

#include "OE/Math/Math.hpp"

namespace OrbitEngine { namespace Graphics {
	FT_Library Font::s_FTLibrary = nullptr;
	Font* Font::s_DefaultFont = nullptr;

	Font::Font(std::string file) {
		if (Font::s_FTLibrary == 0) {
			if (FT_Init_FreeType(&Font::s_FTLibrary) != 0) {
				OE_LOG_FATAL("Couldn't initialize FreeType library");
				return;
			}
			OE_LOG_INFO("FreeType initialized");
		}

		m_FileBuffer = System::LoadFile(file, true);

		FT_New_Memory_Face(Font::s_FTLibrary, (FT_Byte*)m_FileBuffer.data(), m_FileBuffer.size(), 0, &m_Face);
	}

	Font::~Font() {
		if (m_Face)
			FT_Done_Face(m_Face);
		m_FileBuffer.clear();
		m_FontInstances.clear(); // TODO see
	}

	void Font::prepareFont(float size)
	{
		for (FontInstance* i : m_FontInstances) {
			if (i->m_Size == size) {
				m_Instance = i;
				return;
			}
		}

		if (FT_Set_Pixel_Sizes(m_Face, 0, (FT_UInt)size) != 0) {
			OE_LOG_FATAL("Could not set pixel size!");
			return;
		}

		FontInstance* instance = new FontInstance();
		instance->m_Size = size;
		float maxHeight = 0, minHeight = 0;

		Graphics::TextureProperties texProperties;
		texProperties.formatProperties.bpp = 32;
		texProperties.formatProperties.format = Graphics::TextureFormat::RGBA8;
		texProperties.sampleProperties.wrap = Graphics::TextureWrap::CLAMP_TO_EDGE;
		texProperties.sampleProperties.filter = Graphics::TextureFilter::NEAREST;

		FT_Vector kerning;
		FT_Bitmap bmp;

		for (char c = OE_FONT_START; c <= OE_FONT_END; ++c) {
			if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER) != 0) {
				OE_LOG_WARNING("Could not load char '" << c << "'");
				continue;
			}

			int bitmapWidth = m_Face->glyph->bitmap.width;
			int bitmapHeight = m_Face->glyph->bitmap.rows;
			int width = (m_Face->glyph->metrics.width >> 6); // (>> 6) = (/ 64)
			int height = (m_Face->glyph->metrics.height >> 6);
			int H_advance = (m_Face->glyph->metrics.horiAdvance >> 6);
			int H_bearingX = (m_Face->glyph->metrics.horiBearingX >> 6);
			int H_bearingY = (m_Face->glyph->metrics.horiBearingY >> 6);
			int V_advance = (m_Face->glyph->metrics.vertAdvance >> 6);
			int V_bearingX = (m_Face->glyph->metrics.vertBearingX >> 6);
			int V_bearingY = (m_Face->glyph->metrics.vertBearingY >> 6);

			float h = (float)height;
			float offset = H_bearingY - h;
			if (offset < minHeight)
				minHeight = offset;
			if (h > maxHeight)
				maxHeight = h;

			Glyph g = {};
			g.c = c;
			g.x = 0;
			g.y = 0;
			g.w = bitmapWidth;
			g.h = bitmapHeight;

			g.m_Size = Math::Vec2f((float)width, (float)height);
			g.m_H_advance = H_advance;
			g.m_H_bearingX = H_bearingX;
			g.m_H_bearingY = H_bearingY;
			g.m_V_advance = V_advance;
			g.m_V_bearingX = V_bearingX;
			g.m_V_bearingY = V_bearingY;

			instance->m_Glyphs[c - OE_FONT_START] = g;

			int cind = FT_Get_Char_Index(m_Face, c);

			for (char k = OE_FONT_START; k <= OE_FONT_END; ++k) {
				FT_Get_Kerning(m_Face, cind, FT_Get_Char_Index(m_Face, k), FT_KERNING_UNFITTED, &kerning);
				if (kerning.x != 0) instance->m_Kernings.insert(std::pair<std::pair<unsigned char, unsigned char>, float>(std::pair<unsigned char, unsigned char>(c, k), kerning.x >> 6));
			}
		}

		instance->m_MaxHeight = abs(maxHeight) + abs(minHeight);
		instance->m_MaxBearing = minHeight;

		Math::Vec2f texSize;
		std::vector<Misc::Packeable2D*> rects;
		rects.resize(instance->m_Glyphs.size());
		for (size_t i = 0; i < instance->m_Glyphs.size(); i++)
			rects[i] = &instance->m_Glyphs[i];

		if (!Misc::Packer2D::Pack(rects, 4096, texSize)) {
			OE_LOG_WARNING("Could not pack the font glyphs. Maybe font too big?...");
			delete instance;
			return;
		}

		texProperties.formatProperties.width = (unsigned short)texSize.x;
		texProperties.formatProperties.height = (unsigned short)texSize.y;
		if (texProperties.formatProperties.width % 2 != 0) {
			texProperties.formatProperties.width++;
			texProperties.formatProperties.height++;
		}
#if OE_D3D
		if (Application::Context::GetCurrentAPI() == RenderAPI::DIRECT3D) {
			// Set size to the next power of two
			texProperties.formatProperties.width = Math::nextPowerOfTwo(texProperties.formatProperties.width);
			texProperties.formatProperties.height = Math::nextPowerOfTwo(texProperties.formatProperties.height);
		}
#endif
		float texelSize = 1.0f / texProperties.formatProperties.width;
		float halfPix = 0.0f;
		//if (Application::Context::GetCurrentAPI() == RenderAPI::OPENGL)
		//	halfPix = texelSize / 2.0f;

		std::vector<unsigned char> buffer = std::vector<unsigned char>(texProperties.formatProperties.width * texProperties.formatProperties.height * (texProperties.formatProperties.bpp / 8), 255);

		for (size_t i = 0; i < rects.size(); ++i) {
			Glyph* rect = (Glyph*)rects[i];

			if (FT_Load_Char(m_Face, rect->c, FT_LOAD_RENDER) != 0) {
				OE_LOG_WARNING("Could not load char '" << i << "'");
				continue;
			}

			bmp = m_Face->glyph->bitmap;

			if (bmp.buffer != nullptr) {
				int base = (rect->y * texProperties.formatProperties.width + rect->x) * 4;

				if (rect->flipped) {
					for (int y = 0; y < rect->w; ++y) {
						for (int x = 0; x < rect->h; ++x) {
							int c = base + (x * texProperties.formatProperties.width + y) * 4;
							buffer[c + 3] = bmp.buffer[x + bmp.width * y];
						}
					}

					float u0 = (rect->x - halfPix) * texelSize;
					float v0 = (rect->y + rect->h - halfPix) * texelSize;
					float u1 = (rect->x + rect->w - halfPix) * texelSize;
					float v1 = (rect->y - halfPix) * texelSize;

					rect->m_UVs = Math::UV(Math::Vec2f(u0, v1), Math::Vec2f(u1, v1), Math::Vec2f(u1, v0), Math::Vec2f(u0, v0));
				}
				else {
					for (int y = 0; y < rect->h; ++y) {
						for (int x = 0; x < rect->w; ++x) {
							int c = base + (y * texProperties.formatProperties.width + x) * 4;
							buffer[c + 3] = bmp.buffer[x + rect->w * y];
						}
					}

					float u0 = (rect->x - halfPix) * texelSize;
					float v0 = (rect->y - halfPix) * texelSize;
					float u1 = (rect->x + rect->w - halfPix) * texelSize;
					float v1 = (rect->y + rect->h - halfPix) * texelSize;

					rect->m_UVs = Math::UV(u0, v0, u1, v1);
				}
			}
		}

		instance->m_Texture = Graphics::Texture::Create(texProperties, &buffer[0]);

		m_FontInstances.push_back(instance);
		m_Instance = instance;
	}

	void Font::drawString(const std::string& string, const Math::Vec2f& position, Graphics::Renderer2D& r2d) {
		if (!m_Instance) {
			OE_LOG_WARNING("Font instance not generated!");
			return;
		}
		if (string.length() == 0)
			return;

		r2d.bindTexture(m_Instance->m_Texture);
		Math::Vec2f pen(0, m_Instance->m_MaxBearing - getHeight(string) + m_Instance->m_MaxHeight); // - getHeight(string) + m_Instance->m_MaxHeight TEMPORAL TODO
		pen.x += position.x;
		pen.y += position.y;

		for (unsigned int i = 0; i < string.length(); i++) {
			char c = string[i];
			if (c == '\n' || c == '\r') {
				pen.x = position.x;
				pen.y += m_Instance->m_Size;
				continue;
			}
			else if (c < OE_FONT_START || c > OE_FONT_END) continue;
			Glyph* glyph = &m_Instance->m_Glyphs[c - OE_FONT_START];

			if (i > 0)
				pen.x += getHorizontalKerning(string[i - 1], c);

			if (glyph->m_Size.x > 0 && glyph->m_Size.y > 0) {
				Math::Vec2f pos(pen.x + glyph->m_H_bearingX, pen.y - glyph->m_H_bearingY);
				pos.x = (float)(int)pos.x;
				pos.y = (float)(int)pos.y;

				r2d.rect(pos, glyph->m_Size, glyph->m_UVs);
			}

			pen.x += glyph->m_H_advance;
		}
	}

	float Font::getHorizontalKerning(char left, char right)
	{
		auto k = m_Instance->m_Kernings.find(std::make_pair(left, right));
		if (k != m_Instance->m_Kernings.end())
			return k->second;
		return 0;
	}

	Font* Font::GetDefaultFont()
	{
		if (s_DefaultFont)
			return s_DefaultFont;
		s_DefaultFont = new Font("Resources/Fonts/Roboto-Regular.ttf");
		return s_DefaultFont;
	}

	float Font::getWidth(const std::string& text)
	{
		if (!m_Instance) {
			OE_LOG_WARNING("Font instance not generated!");
			return 0;
		}
		if (text.length() == 0)
			return 0;

		float maxWidth = 0;
		float x = 0;

		for (unsigned int i = 0; i < text.length(); i++) {
			char c = text[i];
			if (c == '\n' || c == '\r') {
				x = 0;
				continue;
			}
			else if (c < OE_FONT_START || c > OE_FONT_END) continue;
			Glyph* glyph = &m_Instance->m_Glyphs[c - OE_FONT_START];

			if (i > 0)
				x += getHorizontalKerning(text[i - 1], c);
			x += glyph->m_H_advance;

			if (x > maxWidth)
				maxWidth = x;
		}

		return maxWidth;
	}

	float Font::getHeight(const std::string& text)
	{
		if (!m_Instance) {
			OE_LOG_WARNING("Font instance not generated!");
			return 0;
		}
		if (text.length() == 0)
			return 0;

		int lines = 1;
		for (unsigned int i = 0; i < text.length(); i++) {
			char c = text[i];
			if (c == '\n' || c == '\r')
				lines++;
		}

		return m_Instance->m_MaxHeight * lines;
	}

	Math::Vec2f Font::getBounds(const std::string& text)
	{
		return Math::Vec2f(getWidth(text), getHeight(text));
	}
} }