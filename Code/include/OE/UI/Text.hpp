#ifndef UI_LABEL_HPP
#define UI_LABEL_HPP

#include <string>

#include "OE/UI/Element.hpp"
#include "OE/Graphics/Font.hpp"

namespace OrbitEngine { namespace UI {
	/// Text UI element
	class Text : public Element {
	public:
		Text();
		virtual ~Text();

		// this should be move to styles
		void setFont(Graphics::Font* font);
		void setSize(Graphics::FontSize size);

		void setText(const std::string& text);

	private:
		Graphics::Font* m_Font;
		Graphics::FontSize m_Size;
		std::string m_Text;

		void generateContent(Painter* painter) override;
		Math::Vec2f measureContent(float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) override;

		// TextLayout cache
		// make sure you don't hold the reference after calling flushTextLayoutCache
		const Graphics::TextLayout& getTextLayout(const Graphics::TextSettings& settings, bool skip_glyphs);
		void flushTextLayoutCache();
		std::vector<Graphics::TextLayout> m_TextLayoutCache;
	};
} }

#endif