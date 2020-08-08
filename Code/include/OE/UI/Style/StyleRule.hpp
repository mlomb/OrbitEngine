#ifndef UI_STYLE_RULE_HPP
#define UI_STYLE_RULE_HPP

#include <string>
#include <vector>

namespace OrbitEngine { namespace UI {

	enum class StylePropertyID {
		WIDTH,
		HEIGHT,

		MIN_WIDTH,
		MIN_HEIGHT,

		// always keep it last
		LAST_PROPERTY_INVALID
	};

	struct StyleValue {
		union {
			float value;
			int color;
		};
	};

	struct StyleProperty {
		StylePropertyID id;
		StyleValue value;
	};

	struct StyleRule {
		std::vector<StyleProperty> properties;
	};

	StyleProperty ResolveProperty(const std::string& name, const std::string& value);

} }

#endif