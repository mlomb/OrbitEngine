#ifndef UI_WINDOW_FOOTER_HPP
#define UI_WINDOW_FOOTER_HPP

#include "OE/UI/Elements/Panel.hpp"

namespace OrbitEngine {	namespace UI {
	class Resizer;

	class WindowFooter : public Panel {
	public:
		WindowFooter();

		Resizer* getCornerResizer() { return m_CornerResizer; };

	private:
		Resizer* m_CornerResizer;

		void onRender(Graphics::PathRenderer2D& r2d) {};
	};
} }

#endif