#ifndef UI_COMPONENT_SCROLLBAR_HPP
#define UI_COMPONENT_SCROLLBAR_HPP

#include "OE/UI/Element.hpp"

#include "OE/UI/Components/Button.hpp"
#include "OE/UI/Components/Slider.hpp"

namespace OrbitEngine { namespace UI {

	class Scrollbar : public Element {
	public:
		Scrollbar();
		virtual ~Scrollbar();

	private:
		Button *m_LowButton, *m_HighButton;
		Slider* m_Slider;
	};

} }

#endif