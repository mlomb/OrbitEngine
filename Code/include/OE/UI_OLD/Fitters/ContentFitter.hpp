#ifndef UI_CONTENT_FITTER_H
#define UI_CONTENT_FITTER_H

#include "OE/UI/Fitters/Fitter.hpp"

namespace OrbitEngine { namespace UI {
	class ContentFitter : public Fitter {
	public:
		void fit(Element* element, Axis axis);
	};
} }

#endif