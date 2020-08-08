#ifndef UI_STYLE_SELECTOR_HPP
#define UI_STYLE_SELECTOR_HPP

#include <string>
#include <vector>

namespace OrbitEngine { namespace UI {

	enum class StyleSelectorType {
		ID,
		TAG,
		CLASS,
		WILDCARD
	};

	enum class StyleSelectorPseudo {
		HOVER = 1,
		ACTIVE = 2,
		FOCUS = 4
	};

	enum class StyleSelectorRelationship {
		NONE, // same target (for example tag.class)
		DESCENDANT, // default
		CHILD, // >
		ADJACENT_SIBLING, // +
		GENERAL_SIBLING // ~
	};

	struct StyleSelectorPart {
		std::string value;
		StyleSelectorType type;
		StyleSelectorPseudo pseudos;
		StyleSelectorRelationship prev_relationship;
	};

	struct StyleSelector {
		std::vector<StyleSelectorPart> parts;
		int specificity;
		int matching_rule_index;

		void computeSpecificity();
	};

} }

#endif