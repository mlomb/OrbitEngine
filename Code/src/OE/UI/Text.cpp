#include "OE/UI/Text.hpp"

#include "OE/Misc/Log.hpp"

#include "OE/UI/Painter.hpp"

namespace OrbitEngine { namespace UI {
	Text::Text()
		: Element(), m_Font(nullptr), m_Size(16)
	{
		enableMeasurement();
		setAsTextType();
	}

	Text::~Text()
	{
	}

	void Text::setFont(Graphics::Font* font)
	{
		m_Font = font;
		flushTextLayoutCache();
	}

	void Text::setSize(Graphics::FontSize size)
	{
		m_Size = size;
		markAsDirty();
		flushTextLayoutCache();
	}

	void Text::setText(const std::string& text)
	{
		m_Text = text;
		markAsDirty();
		flushTextLayoutCache();
	}

	void Text::generateContent(Painter* painter)
	{
		// draw background
		Element::generateContent(painter);

		if (m_Font == nullptr || m_Text.size() == 0)
			return;

		Graphics::TextSettings textSettings;
		textSettings.size = m_Size;
		textSettings.wordWrap = true;
		textSettings.wordWrapWidth = m_BoundingBox.z;

		painter->drawText(getTextLayout(textSettings, false), m_BoundingBox.xy);
	}

	Math::Vec2f Text::measureContent(float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
	{
		// See https://github.com/facebook/yoga/pull/576/files

		float measuredWidth = YGUndefined;
		float measuredHeight = YGUndefined;

		if (m_Font == nullptr || m_Text.size() == 0)
			return Math::Vec2f(measuredWidth, measuredHeight);

		// OE_LOG_DEBUG(width << " (" << widthMode << ") -- " << height << " (" << heightMode << ")");

		if (widthMode == YGMeasureModeExactly) {
			measuredWidth = width;
		}
		else {
			Graphics::TextSettings textSettings;
			textSettings.size = m_Size;
			textSettings.wordWrap = false;

			measuredWidth = std::ceil(getTextLayout(textSettings, true).boundingSize.x);

			if (widthMode == YGMeasureModeAtMost)
				measuredWidth = std::min(measuredWidth, width);
		}

		if (heightMode == YGMeasureModeExactly) {
			measuredHeight = height;
		}
		else {
			Graphics::TextSettings textSettings;
			textSettings.size = m_Size;
			textSettings.wordWrap = true;
			textSettings.wordWrapWidth = measuredWidth;

			measuredHeight = std::ceil(getTextLayout(textSettings, true).boundingSize.y);

			if (heightMode == YGMeasureModeAtMost)
				measuredHeight = std::min(measuredHeight, height);
		}

		return Math::Vec2f(measuredWidth, measuredHeight);
	}

	const Graphics::TextLayout& Text::getTextLayout(const Graphics::TextSettings& settings, bool skip_glyphs)
	{
		OE_ASSERT(m_Font);

		for (const Graphics::TextLayout& tl : m_TextLayoutCache) {
			if (!skip_glyphs && tl.no_glyphs)
				continue; // we need glyph data
			if (settings.wordWrap == tl.settings.wordWrap &&
				(!settings.wordWrap || settings.wordWrapWidth == tl.settings.wordWrapWidth))
				return tl;
		}
		m_TextLayoutCache.emplace_back(m_Font->generateTextLayout(m_Text, settings));
		return m_TextLayoutCache.back();
	}

	void Text::flushTextLayoutCache()
	{
		m_TextLayoutCache.clear();
	}
} }
