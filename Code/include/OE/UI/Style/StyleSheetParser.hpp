#ifndef UI_STYLESHEET_PARSER_HPP
#define UI_STYLESHEET_PARSER_HPP

#include <string>
#include <vector>

#include "OE/UI/Style/StyleParse.hpp"

namespace OrbitEngine { namespace UI {

	class StyleSheet;

	/// Parse CSS source into a StyleSheet object
	/// The parser doesn't support any kind of escaping
	StyleSheet* ParseStyleSheet(const std::string& source, StyleParseResult& parseResult);

} }

#endif