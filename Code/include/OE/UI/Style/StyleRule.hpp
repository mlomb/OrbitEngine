#ifndef UI_STYLE_RULE_HPP
#define UI_STYLE_RULE_HPP

#include <vector>
#include <string>

#include "OE/UI/Style/StyleParse.hpp"

#undef OVERFLOW

namespace OrbitEngine { namespace UI {

	enum class StylePropertyID {
		WIDTH,
		HEIGHT,
		MIN_WIDTH,
		MIN_HEIGHT,
		MAX_WIDTH,
		MAX_HEIGHT,

		MARGIN_LEFT,
		MARGIN_TOP,
		MARGIN_RIGHT,
		MARGIN_BOTTOM,

		PADDING_LEFT,
		PADDING_TOP,
		PADDING_RIGHT,
		PADDING_BOTTOM,

		BORDER_COLOR,
		BORDER_TOP_LEFT_RADIUS,
		BORDER_TOP_RIGHT_RADIUS,
		BORDER_BOTTOM_LEFT_RADIUS,
		BORDER_BOTTOM_RIGHT_RADIUS,
		BORDER_LEFT_WIDTH,
		BORDER_TOP_WIDTH,
		BORDER_RIGHT_WIDTH,
		BORDER_BOTTOM_WIDTH,

		FLEX_GROW,
		FLEX_SHRINK,
		FLEX_BASIS,
		FLEX_DIRECTION,
		FLEX_WRAP,

		ALIGN_SELF,
		ALIGN_ITEMS,
		ALIGN_CONTENT,
		JUSTIFY_CONTENT,

		POSITION,
		LEFT,
		TOP,
		RIGHT,
		BOTTOM,

		COLOR,
		BACKGROUND_COLOR,

		OVERFLOW,
		DISPLAY,

		FONT_SIZE,
		WHITE_SPACE,

		CURSOR,

		// always keep it last
		LAST_PROPERTY_INVALID
	};

	enum class StyleLengthUnit {
		UNSPECIFIED,
		PIXELS,
		PERCENT
	};

	struct StyleLength {
		float value;
		StyleLengthUnit unit;
	};

	struct StyleValue {
		union {
			StyleLength length;
			struct {
				union {
					struct {
						float r, g, b, a;
					};
					float components[4];
				};
			} color;
			int enum_index;
		};
	};

	struct StyleProperty {
		StylePropertyID id;
		StyleValue value;
	};

	struct StyleRule {
		std::vector<StyleProperty> properties;
	};

	bool ParseStyleProperty(const std::string& name, const std::string& value, StyleRule& rule, StyleParseResult& parseResult);
} }

#endif