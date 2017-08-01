#include "OE/UI/Fitters/ContentFitter.hpp"

namespace OrbitEngine { namespace UI {
	void ContentFitter::fit(Element* element, Axis axis)
	{
		element->setSizeAlongAxis(element->getLayoutPropertyAlongAxis(LayoutProperty::PREFERRED_SIZE, axis), axis);
	}
} }