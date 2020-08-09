#include "OE/UI/Style/StyleSelector.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace UI {

	bool ParseSingleStyleSelector(const std::string& input, StyleSelector& selector, StyleParseResult& parseResult)
	{
		selector = { };
		selector.specificity = 0;
		selector.matching_rule_index = -1;

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

				StyleSelectorPart part = { };
				part.prev_relationship = next_rel;
				part.ident = ParseCSSIdentifier(input, pos);

				switch (chr) {
				case '*': part.type = StyleSelectorType::WILDCARD; break;
				case '#': part.type = StyleSelectorType::ID; break;
				case '.': part.type = StyleSelectorType::CLASS; break;
				case ':': part.type = StyleSelectorType::PSEUDO; break;
				default:  part.type = StyleSelectorType::TAG; break;
				}

				OE_LOG_DEBUG("(selector type " << std::to_string((int)part.type) << ")" << part.ident);

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

				OE_LOG_DEBUG("(nesting op)" << chr);
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

	// See https://www.w3.org/TR/selectors-3/#specificity
	void StyleSelector::computeSpecificity() {
		int a = 0, b = 0, c = 0;

		for (const StyleSelectorPart& part : parts) {
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
