#ifndef UI_PANEL_HPP
#define UI_PANEL_HPP

#include "OE/UI/Elements/Element.hpp"

namespace OrbitEngine { namespace UI {
	class Panel : public virtual Element {
	public:

	private:
		void onRender(Graphics::PathRenderer2D& r2d);
	};
} }

#endif