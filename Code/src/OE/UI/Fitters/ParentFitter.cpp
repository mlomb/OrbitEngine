#include "OE/UI/Fitters/ParentFitter.hpp"

namespace OrbitEngine { namespace UI {
	void ParentFitter::fit(Element* element, Axis axis)
	{
		float parentSize = element->getParent()->getSize()[axis];
		float minimum = element->getLayoutPropertyAlongAxis(LayoutProperty::MINIMUM_SIZE, axis);
		float result = std::max(parentSize, minimum);
		element->setSizeAlongAxis(result, axis);
	}
} }