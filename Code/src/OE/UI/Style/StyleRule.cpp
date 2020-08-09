#include "OE/UI/Style/StyleRule.hpp"

#include "OE/UI/Style/NamedColors.hpp"

namespace OrbitEngine { namespace UI {

	constexpr unsigned int str2int(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
	}

	// removes all spaces and set to lowercase
	std::string sanitize(const std::string& input) {
		std::string result;
		for (const char& c : input)
			if (!std::isspace(c))
				result.push_back(std::tolower(c));
		return result;
	}

	bool isNumeric(char c) {
		return c >= '0' && c <= '9';
	}

	bool isHex(char c) {
		return
			(c >= 'a' && c <= 'f') ||
			(c >= 'A' && c <= 'F') ||
			isNumeric(c);
	}

	int hexToDec(char c) {
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'a' && c <= 'f') return c - 'a' + 10;
		if (c >= 'A' && c <= 'F') return c - 'A' + 10;
		return 0;
	}

	// converts a two digit hex into a scalar float
	// FF -> 1.0
	float hexToFloat(char l, char r) {
		return (16 * hexToDec(l) + hexToDec(r)) / 255.0f;
	}

	bool parseLength(const std::string& input, StyleValue& output) {
		int pos = 0;
		bool negative = false;
		bool mantissa = false;
		int mantissa_place = 1;
		float value = 0;

		output.length.unit = StyleLengthUnit::UNSPECIFIED;

		while (pos < input.size()) {
			char chr = input[pos];
			if (chr == '-') {
				if (pos == 0) {
					negative = true;
				}
				else
					return false; // unexpected -
			}
			else if (chr == '.') {
				if (mantissa) {
					return false; // unexpected .
				}
				mantissa = true;
			}
			else if (isNumeric(chr)) {
				int num = chr - '0';
				if (!mantissa) {
					// whole part
					value = value * 10 + num;
				}
				else {
					// decimal part
					value = value + (float)num / powf(10, mantissa_place++);
				}
			}
			else {
				// must be a unit

				if (chr == '%') {
					output.length.unit = StyleLengthUnit::PERCENT;
					break; // stop reading
				}
				else if (pos + 1 < input.size() && chr == 'p' && input[pos + 1] == 'x') {
					output.length.unit = StyleLengthUnit::PIXELS;
					break; // stop reading
				}

				return false; // unexpected character
			}
			pos++;
		}

		output.length.value = negative ? -value : value;

		return true;
	}

	bool parseColor(const std::string& input, StyleValue& output) {
		if (input.size() < 2)
			return false;

		if (input[0] == '#') { // hex color
			// #RGB
			if (input.size() == 4) {
				if (isHex(input[1]) &&
					isHex(input[2]) &&
					isHex(input[3])) {
					output.color.r = hexToFloat(input[1], input[1]);
					output.color.g = hexToFloat(input[2], input[2]);
					output.color.b = hexToFloat(input[3], input[3]);
					output.color.a = 1.0f;
					return true;
				}
				else {
					return false; // invalid hex
				}
			}
			// #RRGGBB
			else if (input.size() == 7) {
				if (isHex(input[1]) &&
					isHex(input[2]) &&
					isHex(input[3]) &&
					isHex(input[4]) &&
					isHex(input[5]) &&
					isHex(input[6])) {
					output.color.r = hexToFloat(input[1], input[2]);
					output.color.g = hexToFloat(input[3], input[4]);
					output.color.b = hexToFloat(input[5], input[6]);
					output.color.a = 1.0f;
					return true;
				}
				else {
					return false; // invalid hex
				}
			}
			else {
				return false; // invalid size for hex number
			}
		}
		else if (input.find("rgb") == 0) { // rgb/a color
			// TODO
		}
		else {
			// try to match predefined colors
			// See https://www.w3.org/TR/css-color-3/#svg-color
			auto it = NAMED_COLORS.find(input);
			if (it != NAMED_COLORS.end()) {
				const auto& color = (*it).second;
				output.color.r = std::get<0>(color) / 255.0f;
				output.color.g = std::get<1>(color) / 255.0f;
				output.color.b = std::get<2>(color) / 255.0f;
				output.color.a = std::get<3>(color);
				return true;
			}
		}

		return false;
	}

	bool ParseStyleProperty(const std::string& name, const std::string& value, StyleRule& rule)
	{
		using ID = StylePropertyID;

		StyleProperty prop;

		switch (str2int(sanitize(name).c_str())) {
			
		#define PARSE_PROP(func, prop_name, prop_id) \
		case str2int(prop_name): \
			if (func(sanitize(value), prop.value)) { \
				prop.id = prop_id; \
				rule.properties.emplace_back(prop); \
				return true; \
			} \
			break;

		#define LENGTH_PROPERTY(prop_name, prop_id) PARSE_PROP(parseLength, prop_name, prop_id);
		#define COLOR_PROPERTY(prop_name, prop_id) PARSE_PROP(parseColor, prop_name, prop_id);

		LENGTH_PROPERTY("width",      ID::WIDTH);
		LENGTH_PROPERTY("height",     ID::HEIGHT);
		LENGTH_PROPERTY("min-width",  ID::MIN_WIDTH);
		LENGTH_PROPERTY("min-height", ID::MIN_HEIGHT);
		LENGTH_PROPERTY("max-width",  ID::MAX_WIDTH);
		LENGTH_PROPERTY("max-height", ID::MAX_HEIGHT);

		LENGTH_PROPERTY("margin-left",   ID::MARGIN_LEFT);
		LENGTH_PROPERTY("margin-top",    ID::MARGIN_TOP);
		LENGTH_PROPERTY("margin-right",  ID::MARGIN_RIGHT);
		LENGTH_PROPERTY("margin-bottom", ID::MARGIN_BOTTOM);
			
		LENGTH_PROPERTY("padding-left",   ID::PADDING_LEFT);
		LENGTH_PROPERTY("padding-top",    ID::PADDING_TOP);
		LENGTH_PROPERTY("padding-right",  ID::PADDING_RIGHT);
		LENGTH_PROPERTY("padding-bottom", ID::PADDING_BOTTOM);

		COLOR_PROPERTY("border-color", ID::BORDER_COLOR);
		LENGTH_PROPERTY("border-top-left-radius",     ID::BORDER_TOP_LEFT_RADIUS);
		LENGTH_PROPERTY("border-top-right-radius",    ID::BORDER_TOP_RIGHT_RADIUS);
		LENGTH_PROPERTY("border-bottom-left-radius",  ID::BORDER_BOTTOM_LEFT_RADIUS);
		LENGTH_PROPERTY("border-bottom-right-radius", ID::BORDER_BOTTOM_RIGHT_RADIUS);
		LENGTH_PROPERTY("border-left-width",   ID::BORDER_LEFT_WIDTH);
		LENGTH_PROPERTY("border-top-width",    ID::BORDER_TOP_WIDTH);
		LENGTH_PROPERTY("border-right-width",  ID::BORDER_RIGHT_WIDTH);
		LENGTH_PROPERTY("border-bottom-width", ID::BORDER_BOTTOM_WIDTH);

		LENGTH_PROPERTY("left",   ID::LEFT);
		LENGTH_PROPERTY("top",    ID::TOP);
		LENGTH_PROPERTY("right",  ID::RIGHT);
		LENGTH_PROPERTY("bottom", ID::BOTTOM);

		COLOR_PROPERTY("color", ID::COLOR);
		COLOR_PROPERTY("background-color", ID::BACKGROUND_COLOR);

		LENGTH_PROPERTY("font-size", ID::FONT_SIZE);

		default:
			break;
		}

		return false;
	}
} }