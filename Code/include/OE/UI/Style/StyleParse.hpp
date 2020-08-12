#ifndef UI_STYLE_PARSE_HPP
#define UI_STYLE_PARSE_HPP

#include <vector>
#include <string>

#include "OE/UI/Style/StyleEnums.hpp"

namespace OrbitEngine { namespace UI {

	class StyleSheet;

	struct StyleParseResult {
		std::vector<std::string> errors;
		std::vector<std::string> warnings;
	};

	bool ParseStyleProperty(const std::string& name, const std::string& value, StyleRule& rule, StyleParseResult& parseResult);

	/// Parse a single selector
	/// The input string must have no comments
	/// Returns true if the selector was sucessfully parsed
	bool ParseSingleStyleSelector(const std::string& input_selector, StyleSelector& selector, StyleParseResult& parseResult);

	/// Parse multiple selectors (delimited by ',')
	/// Note that this function will not clear the selectors vector
	/// The input string must have no comments
	/// Returns true if at least one selector was parsed
	bool ParseStyleSelectors(const std::string& input_selectors, std::vector<StyleSelector>& selectors, StyleParseResult& parseResult);

	/// Parse CSS source into a StyleSheet object
	/// The parser doesn't support any kind of escaping
	StyleSheet* ParseStyleSheet(const std::string& source, StyleParseResult& parseResult);
} }

#endif