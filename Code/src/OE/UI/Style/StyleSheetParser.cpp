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

		void parse() {
			while (more()) {
				if (!parseBlock()) {
					errorButFound("Block expected");
					recover(); // try to skip to the next block
				}
				skipWhitespace(); // to reach EOF
			}
		}

		// parses a single block of CSS
		// selector, selector { prop-name: value; }
		// returns true if a block was found
		bool parseBlock() {
			std::vector<StyleSelector> selectors;

			bool expect_selector = true;
			while (expect_selector) {
				StyleSelector selector;
				if (parseSingleSelector(selector)) {
					selectors.push_back(selector);
				} else {
					if (expect_selector)
						errorButFound("Selector expected");
					return false; // didn't manage to parse a single selector
				}
					
				skipWhitespace();
				if (source[pos] == ',') {
					expect_selector = true;
					pos++; // skip ,
				}
				else
					expect_selector = false;
			}

			if(source[pos] == '{') {
				pos++; // skip {

				StyleRule rule = { };
				if (parseRule(rule)) {
					if (rule.properties.size() > 0) { // only add if not empty
						int rule_ref = sheet->addRule(rule);

						// connect selectors
						for (StyleSelector& selector : selectors) {
							selector.matching_rule_index = rule_ref;
							selector.computeSpecificity();
							sheet->addSelector(selector);
						}
					}
				}
				else {
					// failed parsing rule
					return false;
				}
			}
			else {
				errorButFound("Expected start of block");
				return false;
			}

			return true;
		}

		// parse a single selector
		// tag .class > a:pseudo
		bool parseSingleSelector(StyleSelector& selector) {
			StyleSelectorRelationship next_rel = StyleSelectorRelationship::NONE;

			while (more()) {
				skipWhitespace();

				char chr = source[pos];
				bool marker = isSelectorMarker(chr);

				if (chr == ',' || chr == '{') {
					// TODO: make sure there is a proper selector after a nesting operator
					return true;
				}
				else if (isIdentStart(chr) || marker) {
					if (marker)
						pos++; // skip marker

					StyleSelectorPart part = { };
					part.prev_relationship = next_rel;
					part.ident = getIdent();

					switch (chr) {
					case '*': part.type = StyleSelectorType::WILDCARD; break;
					case '#': part.type = StyleSelectorType::ID; break;
					case '.': part.type = StyleSelectorType::CLASS; break;
					case ':': part.type = StyleSelectorType::PSEUDO; break;
					default:  part.type = StyleSelectorType::TAG; break;
					}

					std::cout << "(selector type " << std::to_string((int)part.type) << ")" << part.ident << std::endl;

					selector.parts.emplace_back(part);

					if (source[pos] == ' ' || source[pos] == '/') // check for whitespace (also comment)
						next_rel = StyleSelectorRelationship::DESCENDANT;
					else
						next_rel = StyleSelectorRelationship::NONE;
				}
				else if (isNestingOperator(chr)) {
					pos++; // skip operator

					switch (chr) {
					case '>': next_rel = StyleSelectorRelationship::CHILD; break;
					case '+': next_rel = StyleSelectorRelationship::ADJACENT_SIBLING; break;
					case '~': next_rel = StyleSelectorRelationship::GENERAL_SIBLING; break;
					}

					std::cout << "(nesting op)" << chr << std::endl;
				}
				else {
					errorButFound("Expected '{'");
					return false;
				}
			}

			return false; // EOF
		}

		std::string getIdent() {
			std::string result;

			while (more() && isIdent(source[pos])) {
				result.push_back(source[pos]);
				pos++;
			}

			return result;
		}

		bool parseRule(StyleRule& rule) {
			rule.properties.clear();

			do {
				skipWhitespace();
				if (isIdentStart(source[pos])) {
					// name
					std::string name = getIdent();
					skipWhitespace();
					if (source[pos] == ':') {
						pos++; // skip :
					}
					else {
						errorButFound("Expected ':'");
						return false;
					}

					// value
					skipWhitespace();
					auto value_end = source.find_first_of(";}", pos);
					if (value_end == std::string::npos) {
						errorButFound("Expected property value");
						return false;
					}
					std::string value = source.substr(pos, value_end - pos);
					pos = value_end + 1;

					std::cout << "  (property)" << name << ":" << value << std::endl;

					// parse property
					toLower(name);
					StyleProperty prop = ResolveProperty(name, value);
					if (prop.id != StylePropertyID::LAST_PROPERTY_INVALID) {
						// valid prop
						rule.properties.emplace_back(prop);
					}
					else {
						warn("Property '" + name + "' is not supported or the value is malformed.");
					}
				}
				else
					break; // no more properties
			} while (more());

			if (more() && source[pos] == '}') {
				pos++; // skip }
			}

			return true;
		}

		bool more() {
			return pos != std::string::npos && pos >= 0 && pos < l;
		}

		// tries to skip to the next block
		void recover() {
			if (pos < 0 || pos == std::string::npos)
				return; // can't recover
			pos = source.find("}", pos);
			if (pos == std::string::npos)
				pos = l; // EOF
			else
				pos++;
		}

		void skipWhitespace() {
			pos = source.find_first_not_of(WHITESPACE, pos);
			if (pos == std::string::npos) {
				pos = l; // EOF
				return;
			}
			if (pos + 1 < l && source[pos] == '/' && source[pos + 1] == '*') {
				// comment
				pos = source.find("*/", pos);
				if (pos == std::string::npos)
					error("Expected closing comment but got EOF");
				else {
					pos += 2;
					skipWhitespace(); // skip whitespace after comment
				}
			}
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

		bool isNestingOperator(char c) {
			return c == '>' || c == '+' || c == '~';
		}

		bool isSelectorMarker(char c) {
			return c == '*' || c == '#' || c == '.' || c == ':';
		}

		void toLower(std::string& input) {
			for (char& c : input)
				c = tolower(c);
		}

		void error(const std::string& err) {
			result.errors.emplace_back(err);
		}

		void errorButFound(const std::string& expected) {
			if (more())
				error(expected + " but found '" + std::string(1, source[pos]) + "'");
			else
				error(expected + " but found EOF");
		}

		void warn(const std::string& warn) {
			result.warnings.emplace_back(warn);
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
