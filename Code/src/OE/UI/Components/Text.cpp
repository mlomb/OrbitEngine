#include "OE/UI/Components/Text.hpp"

#include "OE/Misc/Log.hpp"

#include "OE/UI/Render/Painter.hpp"

namespace OrbitEngine { namespace UI {
	Text::Text()
		: Element(), m_Font(nullptr), m_Size(16)
	{
		enableMeasurement();
		setAsTextType();
		setTag("Text");
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
		flushTextLayoutCache();
	}

	void Text::setText(const std::string& text)
	{
		m_Text = text;
		flushTextLayoutCache();
	}

	void Text::paintContent(Painter* painter)
	{
		// draw background
		Element::paintContent(painter);

		if (m_Font == nullptr || m_Text.size() == 0)
			return;

		auto bbox = getBoundingBox();

		Graphics::TextSettings textSettings;
		textSettings.size = m_Size;
		textSettings.wordWrap = true;
		textSettings.wordWrapWidth = bbox.width;

		painter->drawText(getTextLayout(textSettings, false), bbox.position);
	}

	Math::Vec2f Text::measureContent(float width, MeasureMode widthMode, float height, MeasureMode heightMode)
	{
		// See https://github.com/facebook/yoga/pull/576/files

		float measuredWidth = NAN; // NAN from corecrt_math.h
		float measuredHeight = NAN;

		if (m_Font == nullptr || m_Text.size() == 0)
			return Math::Vec2f(measuredWidth, measuredHeight);

		// OE_LOG_DEBUG(width << " (" << widthMode << ") -- " << height << " (" << heightMode << ")");

		if (widthMode == MeasureMode::EXACTLY) {
			measuredWidth = width;
		}
		else {
			Graphics::TextSettings textSettings;
			textSettings.size = m_Size;
			textSettings.wordWrap = false;

			measuredWidth = std::ceil(getTextLayout(textSettings, true).boundingSize.x);

			if (widthMode == MeasureMode::AT_MOST)
				measuredWidth = std::min(measuredWidth, width);
		}

		if (heightMode == MeasureMode::EXACTLY) {
			measuredHeight = height;
		}
		else {
			Graphics::TextSettings textSettings;
			textSettings.size = m_Size;
			textSettings.wordWrap = true;
			textSettings.wordWrapWidth = measuredWidth;

			measuredHeight = std::ceil(getTextLayout(textSettings, true).boundingSize.y);

			if (heightMode == MeasureMode::AT_MOST)
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
