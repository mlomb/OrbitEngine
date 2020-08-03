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
	}

	void Text::setSize(Graphics::FontSize size)
	{
		m_Size = size;
		markAsDirty();
	}

	void Text::setText(const std::string& text)
	{
		m_Text = text;
		markAsDirty();
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

		Graphics::TextLayout layout = m_Font->generateTextLayout(m_Text, textSettings);

		painter->drawText(layout, m_BoundingBox.xy);
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

			Graphics::TextLayout layout = m_Font->generateTextLayout(m_Text, textSettings);
			measuredWidth = std::ceil(layout.boundingSize.x);

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

			Graphics::TextLayout layout = m_Font->generateTextLayout(m_Text, textSettings);
			measuredHeight = std::ceil(layout.boundingSize.y);

			if (heightMode == YGMeasureModeAtMost)
				measuredHeight = std::min(measuredHeight, height);
		}

		return Math::Vec2f(measuredWidth, measuredHeight);
	}
} }
