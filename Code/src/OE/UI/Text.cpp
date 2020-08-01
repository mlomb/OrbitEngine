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

		painter->drawText(m_Text, m_BoundingBox.xy, m_Font, textSettings);
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

			measuredWidth = std::ceil(m_Font->computeTextWidth(m_Text, textSettings));

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

			measuredHeight = std::ceil(m_Font->computeTextHeight(m_Text, textSettings));

			if (heightMode == YGMeasureModeAtMost)
				measuredHeight = std::min(measuredHeight, height);
		}

		return Math::Vec2f(measuredWidth, measuredHeight);
	}
} }
