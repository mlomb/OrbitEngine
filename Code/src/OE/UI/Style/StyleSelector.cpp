#include "OE/UI/Style/StyleSelector.hpp"

namespace OrbitEngine { namespace UI {

	void StyleSelector::computeSpecificity() {
		specificity = 0;

		for (const StyleSelectorPart& part : parts) {
			// TODO
		}
	}

} }
