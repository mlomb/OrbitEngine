#ifndef UI_STYLESHEET_PARSER_HPP
#define UI_STYLESHEET_PARSER_HPP

#include <string>
#include <vector>

namespace OrbitEngine { namespace UI {

	class StyleSheet;

	struct StyleSheetParseResult {
		std::vector<std::string> errors;
		std::vector<std::string> warnings;
	};

	/// Parse CSS source into a StyleSheet object
	StyleSheet* ParseStyleSheet(const std::string& source, StyleSheetParseResult& result);

} }

#endif