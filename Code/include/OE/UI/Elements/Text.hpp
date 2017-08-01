#ifndef UI_TEXT_HPP
#define UI_TEXT_HPP

#include <string>

#include "OE/UI/Elements/Element.hpp"
#include "OE/Graphics/Font.hpp"

#include "OE/Math/Color.hpp"

namespace OrbitEngine {	namespace UI {
	class Text : public Element {
	public:
		Text();

		void setText(std::string text) { m_Text = text; invalidate(); };
		void setTextAlignament(RectAlignment textAlignament) { m_TextAlignament = textAlignament; invalidate(); };
		void setFontSize(float fontSize) { m_FontSize = fontSize; }

		std::string getText() { return m_Text; }

	private:
		std::string m_Text;
		Graphics::Font* m_Font;
		Graphics::FontStyle m_Style;
		float m_FontSize = 18;
		float m_LineSpacing = 1;
		bool m_Rich = false;
		bool m_BestFit = false;
		RectAlignment m_TextAlignament = RectAlignment::MIDDLE_CENTER;
		Math::Color m_Color = Math::Color(1, 1, 1, 1);
		Math::Vec4f m_TextBounds;

		void onRender(Graphics::PathRenderer2D& r2d);
		void onValidate();

		float getLayoutPropertyAlongAxis(LayoutProperty property, Axis axis) override;
	};
} }

#endif