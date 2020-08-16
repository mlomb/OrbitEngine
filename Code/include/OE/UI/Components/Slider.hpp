#ifndef UI_COMPONENTS_SLIDER_HPP
#define UI_COMPONENTS_SLIDER_HPP

#include "OE/UI/Element.hpp"

#include "OE/UI/Components/Draggable.hpp"

namespace OrbitEngine { namespace UI { 

	class Slider : public Element {
	public:
		Slider();
		virtual ~Slider();

	private:
		Element* m_Track;
		Draggable* m_Thumb;
	};

} }

#endif