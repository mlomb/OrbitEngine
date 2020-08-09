#include "OE/UI/Style/StyleSelector.hpp"

namespace OrbitEngine { namespace UI {

	// See https://www.w3.org/TR/selectors-3/#specificity
	void StyleSelector::computeSpecificity() {
		int a = 0, b = 0, c = 0;

		for (const StyleSelectorPart& part : parts) {
			// TODO
			switch (part.type)
			{
			case StyleSelectorType::ID:
				a++;
				break;
			case StyleSelectorType::CLASS:
			case StyleSelectorType::PSEUDO:
				b++;
				break;
			case StyleSelectorType::TAG:
				c++;
				break;
			case StyleSelectorType::WILDCARD:
				// no contribution
				break;
			}
		}

		specificity = a * 100 + b * 10 + c;
	}

} }
