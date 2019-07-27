#include "OE/Graphics/FontInstance.hpp"

namespace OrbitEngine { namespace Graphics {

	FontInstance::FontInstance()
	{
	}

	FontInstance::~FontInstance()
	{
	}

	Math::Vec2i FontInstance::getBounds(const std::string& text)
	{
		if (text.length() == 0)
			return Math::Vec2i(0, 0);

		float maxWidth = 0;
		float x = 0;
		int lines = 1;

		for (unsigned int i = 0; i < text.size(); i++) {
			GlyphIndex c = text[i];
			if (c == '\n' || c == '\r') {
				lines++;
				x = 0;
				continue;
			}

			if (x > 0)
				x += getHorizontalKerning(text[i - 1], c);
			x += m_Glyphs[c].H_advance;

			if (x > maxWidth)
				maxWidth = x;
		}

		return Math::Vec2i(maxWidth, m_MaxHeight * lines);
	}

	int FontInstance::getHorizontalKerning(GlyphIndex left, GlyphIndex right) const
	{
		auto k = m_Kernings.find(std::make_pair(left, right));
		if (k != m_Kernings.end())
			return k->second;
		return 0;
	}

	const std::map<GlyphIndex, Glyph>& FontInstance::getGlyphs() const
	{
		return m_Glyphs;
	}
} }