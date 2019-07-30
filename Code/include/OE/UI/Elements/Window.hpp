#ifndef UI_WINDOW_HPP
#define UI_WINDOW_HPP

#include <string>

#include "OE/UI/Elements/Element.hpp"
#include "OE/UI/Elements/WindowHeader.hpp"
#include "OE/UI/Elements/WindowFooter.hpp"

namespace OrbitEngine {	namespace UI {
	class Window : public Element {
	public:
		Window();

		void setTitle(std::wstring title) { return m_Header->getTitle()->setText(title); }
		void setContent(Element* element, bool borders = true);

		std::wstring getTitle() { return m_Header->getTitle()->getText(); }

	private:
		WindowHeader* m_Header;
		Element* m_Content;
		WindowFooter* m_Footer;
		Math::Vec2f m_MinSize;

		void headerDrag(const DragEvent& dragEvent);
		void footerCornerDrag(const DragEvent& dragEvent);
		void focusGain(const Event& event);
		
		void onRender(Graphics::PathRenderer2D& r2d);
	};
} }

#endif