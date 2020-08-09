#ifndef UI_STYLE_SELECTOR_HPP
#define UI_STYLE_SELECTOR_HPP

#include <string>
#include <vector>

#include "OE/UI/Style/StyleParse.hpp"

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
		int matching_rule_index = -1;

		void computeSpecificity();
	};

	/// Parse a single selector as string until ',', '{' or the end of the string
	/// The input string must have no comments
	/// Returns true if the selector was sucessfully parsed
	bool ParseSingleStyleSelector(const std::string& input_selector, StyleSelector& selector, StyleParseResult& parseResult);
	/// Parse multiple selectors in a string (delimited by ',')
	/// Note that this function will not clear the selectors vector
	/// The input string must have no comments
	/// Returns true if at least one selector was parsed
	bool ParseStyleSelectors(const std::string& input_selectors, std::vector<StyleSelector>& selectors, StyleParseResult& parseResult);
} }

#endif