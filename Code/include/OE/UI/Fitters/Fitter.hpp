#ifndef UI_FITTER_HPP
#define UI_FITTER_HPP

#include "OE/UI/Elements/Element.hpp"

namespace OrbitEngine { namespace UI {
	// TODO Remove this concept of "fitters"
	class Fitter {
	public:
		virtual void fit(Element* element, Axis axis) = 0;
	};
} }

#endif