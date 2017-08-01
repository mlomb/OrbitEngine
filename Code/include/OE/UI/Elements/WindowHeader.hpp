#ifndef UI_WINDOW_HEADER_HPP
#define UI_WINDOW_HEADER_HPP

#include "OE/UI/Elements/Element.hpp"
#include "OE/UI/Elements/DraggableElement.hpp"
#include "OE/UI/Elements/Text.hpp"
#include "OE/UI/Elements/Panel.hpp"

namespace OrbitEngine {	namespace UI {
	class WindowHeader : public Panel, public DraggableElement {
	public:
		WindowHeader();

		Text* getTitle() { return m_Title; };

	private:
		Text* m_Title;

		void onRender(Graphics::PathRenderer2D& r2d);
	};
} }

#endif