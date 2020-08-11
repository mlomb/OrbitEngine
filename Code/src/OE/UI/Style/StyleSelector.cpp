#include "OE/UI/Style/StyleSelector.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace UI {

	bool ParseSingleStyleSelector(const std::string& input, StyleSelector& selector, StyleParseResult& parseResult)
	{
		selector = { };
		selector.specificity = 0;
		selector.order = 0;
		selector.rule = nullptr;

		size_t pos = 0;
		StyleSelectorRelationship next_rel = StyleSelectorRelationship::NONE;

		while (pos < input.size()) {
			ConsumeCSSWhiteSpace(input, pos);
			if (pos >= input.size())
				break; // no more

			char chr = input[pos];
			bool marker = IsCSSSelectorMarker(chr);

			if (IsCSSIdentStart(chr) || marker) {
				if (marker)
					pos++; // skip marker

				StyleIdentifierType type;
				bool is_wildcard = false;

				switch (chr) {
				case '#': type = StyleIdentifierType::ID; break;
				default:  type = StyleIdentifierType::TAG; break;
				case '.': type = StyleIdentifierType::CLASS; break;
				case '*': type = StyleIdentifierType::TAG; is_wildcard = true; break;
				case ':':
					std::string pseudo = ParseCSSIdentifier(input, pos);

					StylePseudoStates state;
					switch (HashStr(pseudo.c_str())) {
					case HashStr("hover"):    state = StylePseudoStates::HOVER; break;
					case HashStr("disabled"): state = StylePseudoStates::DISABLED; break;
					case HashStr("checked"):  state = StylePseudoStates::CHECKED; break;
					case HashStr("active"):   state = StylePseudoStates::ACTIVE; break;
					case HashStr("focus"):    state = StylePseudoStates::FOCUS; break;
					default:
						parseResult.warnings.emplace_back("Unsupported pseudo state '" + pseudo + "'");
						return false;
					}

					// elem:pseudo
					if (next_rel == StyleSelectorRelationship::NONE && selector.parts.size() > 0) {
						selector.parts.back().pseudo_states |= state;
					}
					// :pseudo
					// elem > :pseudo
					else {
						// create wildcard tag
						StyleSelectorPart part = { };
						part.identifier.type = StyleIdentifierType::TAG;
						part.identifier.text = "*";
						part.identifier.computeHash();
						part.prev_relationship = StyleSelectorRelationship::NONE;
						part.pseudo_states = state;
						selector.parts.emplace_back(part);
					}

					if (input[pos] == ' ') // check for space
						next_rel = StyleSelectorRelationship::DESCENDANT;
					else
						next_rel = StyleSelectorRelationship::NONE;

					continue; // skip the rest
				}

				StyleSelectorPart part = { };
				part.identifier.type = type;
				part.identifier.text = is_wildcard ? "*" : ParseCSSIdentifier(input, pos);
				part.identifier.computeHash();
				part.prev_relationship = next_rel;
				part.pseudo_states = StylePseudoStates::NONE;

				// OE_LOG_DEBUG("(selector type " << std::to_string((int)part.identifier.type) << ")" << part.identifier.text);

				selector.parts.emplace_back(part);

				if (input[pos] == ' ') // check for space
					next_rel = StyleSelectorRelationship::DESCENDANT;
				else
					next_rel = StyleSelectorRelationship::NONE;
			}
			else if (IsCSSNestingOperator(chr)) {
				pos++; // skip operator

				switch (chr) {
				case '>': next_rel = StyleSelectorRelationship::CHILD; break;
				case '+': next_rel = StyleSelectorRelationship::ADJACENT_SIBLING; break;
				case '~': next_rel = StyleSelectorRelationship::GENERAL_SIBLING; break;
				}

				// OE_LOG_DEBUG("(nesting op)" << chr);
			}
			else {
				parseResult.errors.emplace_back("Unexpected character '" + std::string(1, chr) + "'");
				return false;
			}
		}

		if (selector.parts.size() > 0) {
			selector.computeSpecificity();
			return true;
		}
		else {
			return false;
		}
	}

	bool ParseStyleSelectors(const std::string& input, std::vector<StyleSelector>& selectors, StyleParseResult& parseResult)
	{
		bool any = false;

		size_t pos = 0;
		while (pos < input.size()) {
			size_t selector_end = input.find(",", pos);
			if (selector_end == std::string::npos)
				selector_end = input.size();

			std::string single_selector = input.substr(pos, selector_end - pos);

			StyleSelector selector;
			if (ParseSingleStyleSelector(single_selector, selector, parseResult)) {
				selectors.emplace_back(selector);
				any = true;
			}

			pos = selector_end + 1;
		}

		return any;
	}

	void UI::StyleIdentifier::computeHash()
	{
		text_hash = HashStr(text.c_str());
	}

	// See https://www.w3.org/TR/selectors-3/#specificity
	void StyleSelector::computeSpecificity() {
		int a = 0, b = 0, c = 0;

		for (const StyleSelectorPart& part : parts) {
			switch (part.identifier.type)
			{
			case StyleIdentifierType::ID:
				a++;
				break;
			case StyleIdentifierType::CLASS:
				b++;
				break;
			case StyleIdentifierType::TAG:
				if (part.identifier.text.size() == 1 && part.identifier.text[0] == '*') {
					// ignore wildcard
				} else
					c++;
				break;
			}

			uint32_t pseudos = (uint32_t)part.pseudo_states;
			while (pseudos) {
				b += pseudos & 1; // count the number of bits set
				pseudos >>= 1;
			}
		}

		specificity = a * 100 + b * 10 + c;
	}
} }
