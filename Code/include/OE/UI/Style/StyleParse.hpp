#ifndef UI_STYLE_PARSE_HPP
#define UI_STYLE_PARSE_HPP

#include <vector>
#include <string>

namespace OrbitEngine { namespace UI {

	// Parse definitions and utilities

	struct StyleParseResult {
		std::vector<std::string> errors;
		std::vector<std::string> warnings;
	};

	/// Check if the character is a valid start for a CSS identifier
	inline bool IsCSSIdentStart(char c) {
		return
			(c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c == '-') || (c == '_');
	}

	/// Check if the character is valid for a CSS identifier
	inline bool IsCSSIdent(char c) {
		return IsCSSIdentStart(c) || (c >= '0' && c <= '9');
	}

	/// Check if the character is a CSS nesting operator
	inline bool IsCSSNestingOperator(char c) {
		return c == '>' || c == '+' || c == '~';
	}

	/// Check if the character is a CSS selector marker
	inline bool IsCSSSelectorMarker(char c) {
		return c == '*' || c == '#' || c == '.' || c == ':';
	}

	/// Advances pos until there is no more whitespace
	void ConsumeCSSWhiteSpace(const std::string& input, size_t& pos);

	/// Parse a single CSS identifier from pos (also advances pos)
	std::string ParseCSSIdentifier(const std::string& input, size_t& pos);

} }

#endif