#ifndef UI_RESIZER_HPP
#define UI_RESIZER_HPP

#include "OE/UI/Elements/Panel.hpp"
#include "OE/UI/Elements/DraggableElement.hpp"

namespace OrbitEngine {	namespace UI {
	class Resizer : public Panel, public DraggableElement {
	public:
		Resizer();

	private:
		void onRender(Graphics::PathRenderer2D& r2d);
	};
} }

#endif