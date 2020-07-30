#include "OE/UI/Elements/Text.hpp"

#include "OE/Graphics/2D/PathRenderer2D.hpp"
#include "OE/UI/Layouts/AlignableLayout.hpp"

namespace OrbitEngine {	namespace UI {
	Text::Text() :
		m_TextBounds(Math::Vec4f())
	{
		m_Font = NULL; // TODO: Fix
		//m_Font = Graphics::Font::GetDefaultFont();
	}

	void Text::onRender(Graphics::PathRenderer2D& r2d)
	{
		if (m_Font) {
			// TODO: Add support for fonts in the PathRenderer2D
			/*
			r2d.bindColor(m_Color);
			m_Font->prepareFont(m_FontSize);
			m_Font->drawString(m_Text, getRenderPosition() + Math::Vec2f(0, getLayoutPropertyAlongAxis(LayoutProperty::PREFERRED_SIZE, Axis::VERTICAL)), r2d);
			*/
		}
		else // Well..
			OE_LOG_WARNING("Text without font.");
	}

	void Text::onValidate()
	{
		//m_TextBounds.zw = m_Font->getBounds(m_Text);
		//m_TextBounds.xy = AlignableLayout::GetAlignedPosition(m_TextAlignament, m_TextBounds.zw, getSize());
	}

	float Text::getLayoutPropertyAlongAxis(LayoutProperty property, Axis axis)
	{
		switch (property) {
		case LayoutProperty::PREFERRED_SIZE:
			//Math::Vec2i bounds = m_Font->getBounds(m_Text);
			//if (axis == Axis::HORIZONTAL)
			//	return bounds.x;
			//else
			//	return bounds.y;
			break;
		}
		return Element::getLayoutPropertyAlongAxis(property, axis);
	}
} }