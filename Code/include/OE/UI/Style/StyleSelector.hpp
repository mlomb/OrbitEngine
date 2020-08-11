#ifndef UI_STYLE_SELECTOR_HPP
#define UI_STYLE_SELECTOR_HPP

#include <string>
#include <vector>

#include "OE/UI/Style/StyleParse.hpp"
#include "OE/UI/Style/StyleEnums.hpp"

namespace OrbitEngine { namespace UI {

	class StyleRule;

	enum class StyleIdentifierType {
		ID,
		TAG,
		CLASS
	};

	enum class StylePseudoStates : int {
		NONE     = 0,
		HOVER    = 1 << 0,
		DISABLED = 1 << 1,
		CHECKED  = 1 << 2,
		ACTIVE   = 1 << 3,
		FOCUS    = 1 << 4
	};

	enum class StyleSelectorRelationship {
		NONE, // same target (for example tag.class:pseudo)
		DESCENDANT, // default
		CHILD, // >
		ADJACENT_SIBLING, // +
		GENERAL_SIBLING // ~
	};

	struct StyleIdentifier {
		StyleIdentifierType type;
		std::string text;
		StyleHash text_hash;

		void computeHash();
		bool operator==(const StyleIdentifier& oth) const { return type == oth.type && text_hash == oth.text_hash; }
	};

	struct StyleSelectorPart {
		StyleIdentifier identifier;
		StyleSelectorRelationship prev_relationship;
		StylePseudoStates pseudo_states;
	};

	struct StyleSelector {
		std::vector<StyleSelectorPart> parts;
		StyleRule* rule = nullptr; // filled by StyleSheet
		int specificity;
		int order; // order in StyleSheet (filled by StyleSheet)

		void computeSpecificity();
	};

	/// Parse a single selector
	/// The input string must have no comments
	/// Returns true if the selector was sucessfully parsed
	bool ParseSingleStyleSelector(const std::string& input_selector, StyleSelector& selector, StyleParseResult& parseResult);
	
	/// Parse multiple selectors (delimited by ',')
	/// Note that this function will not clear the selectors vector
	/// The input string must have no comments
	/// Returns true if at least one selector was parsed
	bool ParseStyleSelectors(const std::string& input_selectors, std::vector<StyleSelector>& selectors, StyleParseResult& parseResult);

	// operators for StylePseudoStates
	inline StylePseudoStates& operator|= (StylePseudoStates& a, StylePseudoStates b) { return (StylePseudoStates&)((int&)a |= (int)b); }
} }

#endif