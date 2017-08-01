#include "OE/UI/Elements/Text.hpp"

#include "OE/Graphics/2D/PathRenderer2D.hpp"
#include "OE/UI/Layouts/AlignableLayout.hpp"

namespace OrbitEngine {	namespace UI {
	Text::Text() :
		m_TextBounds(Math::Vec4f())
	{
		m_Font = Graphics::Font::GetDefaultFont();
	}

	void Text::onRender(Graphics::PathRenderer2D& r2d)
	{
		if (m_Font) {
			r2d.bindColor(m_Color);
			m_Font->prepareFont(m_FontSize);
			m_Font->drawString(m_Text, getRenderPosition() + Math::Vec2f(0, getLayoutPropertyAlongAxis(LayoutProperty::PREFERRED_SIZE, Axis::VERTICAL)), r2d);
		}
		else // Well..
			OE_LOG_WARNING("Text without font.");
	}

	void Text::onValidate()
	{
		m_Font->prepareFont(m_FontSize);
		m_TextBounds.zw = m_Font->getBounds(m_Text);
		m_TextBounds.xy = AlignableLayout::GetAlignedPosition(m_TextAlignament, m_TextBounds.zw, getSize());
	}

	float Text::getLayoutPropertyAlongAxis(LayoutProperty property, Axis axis)
	{
		switch (property) {
		case LayoutProperty::PREFERRED_SIZE:
			if (axis == Axis::HORIZONTAL)
				return m_Font->getWidth(m_Text);
			else
				return m_Font->getHeight(m_Text);
		}
		return Element::getLayoutPropertyAlongAxis(property, axis);
	}
} }