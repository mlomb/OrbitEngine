#ifndef UI_PARENT_FITTER_HPP
#define UI_PARENT_FITTER_HPP

#include "OE/UI/Fitters/Fitter.hpp"

namespace OrbitEngine { namespace UI {
	class ParentFitter : public Fitter {
	public:
		void fit(Element* element, Axis axis);
	};
} }

#endif