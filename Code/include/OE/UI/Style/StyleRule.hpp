#ifndef UI_STYLE_RULE_HPP
#define UI_STYLE_RULE_HPP

#include <string>
#include <vector>

namespace OrbitEngine { namespace UI {

	enum class StylePropertyType {
		MIN_WIDTH,
		MIN_HEIGHT,

		// always keep it last
		LAST_PROPERTY_INVALID
	};

	enum class StyleValueType {
		FLOAT,
		COLOR
	};

	struct StyleValue {
		union {
			float value;
			int color;
		};
	};

	struct StyleProperty {
		StylePropertyType property_type;
		StyleValueType value_type;
		StyleValue value;
	};

	struct StyleRule {
		std::vector<StyleProperty> properties;
	};

} }

#endif