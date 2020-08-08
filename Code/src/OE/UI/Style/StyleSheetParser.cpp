#include "OE/UI/Style/StyleSheetParser.hpp"

#include "OE/UI/Style/StyleSheet.hpp"

#include <iostream>

namespace OrbitEngine { namespace UI {

	const std::string WHITESPACE = " \n\r\t";

	// holds context
	struct StyleSheetParser {
		const std::string& source;
		StyleSheetParseResult& result; // errors, warnings
		int pos, l; // pos: current position, l: source.size()
		StyleSheet* sheet;

		void error(const std::string& err) {
			result.errors.emplace_back(err);
		}

		void warn(const std::string& warn) {
			result.errors.emplace_back(warn);
		}

		void parse() {
			parseSelector();
			if (pos < l) {
				error("Rule expected but found '" + std::string(1, source[pos]) + "'");
			}
		}

		// parse multiple selectors
		// div, p, span, ...
		void parseSelector() {
			parseSingleSelector();

			char chr = source[pos];
			while (chr == ',') {
				pos++;
				skipWhitespace();
				if (!parseSingleSelector()) {
					error("Expected selector");
				}
			}
		}

		// parse a single selector
		// tag .class a:pseudo
		bool parseSingleSelector() {
			skipWhitespace();
			if (!parseSelectorPart())
				return false;

			bool smth = false;

			while (1) {
				skipWhitespace();

				char chr = source[pos];
				if (pos >= l || chr == ',' || chr == ')')
					break;
				if (isNestingOperator(chr)) {
					std::cout << "(nesting op)" << chr << std::endl;
					pos++;
					skipWhitespace();
					if (!parseSelectorPart()) {
						error("Rule expected after '" + string(1, chr) + "'");
						break;
					}
				}
				else {
					if (!parseSelectorPart())
						break;
				}
				smth = true;
			}

			return smth;
		}

		// parse a selector part
		// a:pseudo
		bool parseSelectorPart() {
			bool smth = false;
			while (pos < l) {
				char chr = source[pos];
				if (chr == '*') {
					pos++;
					// TODO: wildcard tag selector
					std::cout << "(wildcard)" << std::endl;
					smth = true;
				}
				else if (isIdentStart(chr) || chr == '\\') {
					// TODO: tag selector
					std::string tag = getIdent();
					std::cout << "(tag)" << tag << std::endl;
					smth = true;
				}
				else if (chr == '.') {
					pos++;
					// TODO: class selector
					std::string klass = getIdent();
					std::cout << "(class)" << klass << std::endl;
					smth = true;
				}
				else if (chr == ':') {
					pos++;
					// TODO: pseudo selector
					std::string pseudo = getIdent();
					std::cout << "(pseudo)" << pseudo << std::endl;

				}
				else {
					return smth;
				}
			}
		}

		std::string getIdent() {
			std::string result;

			char chr = source[pos];
			while (pos < l) {
				if (isIdent(chr)) {
					result.push_back(chr);
				}
				else if (chr == '\\') {
					pos++;
					if (pos >= l) {
						error("Expected symbol but end of file reached.");
						return result;
					}
					chr = source[pos];
					if (isIdentSpecial(chr)) {
						result.push_back(chr);
					}
					else if (isHex(chr)) {
						//
						error("TODO: IMPLEMENT HEX");
					}
					else {
						result.push_back(chr);
					}
				}
				else {
					return result;
				}
				pos++;
				chr = source[pos];
			}

			return result;
		}

		void skipWhitespace() {
			pos = source.find_first_not_of(WHITESPACE, pos);
		}

		bool isHex(char c) {
			return
				(c >= 'a' && c <= 'f') ||
				(c >= 'A' && c <= 'F') ||
				(c >= '0' && c <= '9');
		}

		bool isIdentStart(char c) {
			return
				(c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				(c == '-') || (c == '_');
		}

		bool isIdent(char c) {
			return isIdentStart(c) || (c >= '0' && c <= '9');
		}

		bool isIdentSpecial(char c) {
			return
				c == '!' || c == '"' || c == '#' || c == '$' ||
				c == '%' || c == '&' || c == '(' || c == ')' ||
				c == '*' || c == '+' || c == ',' || c == '.' ||
				c == '/' || c == ';' || c == '<' || c == '=' ||
				c == '>' || c == '?' || c == '@' || c == '[' ||
				c == ']' || c == '^' || c == '`' || c == '{' ||
				c == '|' || c == '}' || c == '~' ||
				c == '\'' || c == '\\';
		}

		bool isNestingOperator(char c) {
			return c == '>' || c == '+' || c == '~';
		}
	};

	StyleSheet* ParseStyleSheet(const std::string& source, StyleSheetParseResult& result)
	{
		result.errors.clear();
		result.warnings.clear();

		StyleSheet* sheet = new StyleSheet();

		StyleSheetParser parser = { source, result, 0, source.size(), sheet };

		parser.parse();

		return sheet;
	}

} }
