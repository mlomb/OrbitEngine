#ifndef UI_STYLE_SELECTOR_HPP
#define UI_STYLE_SELECTOR_HPP

#include <string>
#include <vector>

namespace OrbitEngine { namespace UI {

	enum class StyleSelectorType {
		ID,
		TAG,
		CLASS,
		WILDCARD,
		PSEUDO
	};

	enum class StyleSelectorRelationship {
		NONE, // same target (for example tag.class:pseudo)
		DESCENDANT, // default
		CHILD, // >
		ADJACENT_SIBLING, // +
		GENERAL_SIBLING // ~
	};

	struct StyleSelectorPart {
		std::string ident;
		StyleSelectorType type;
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