#ifndef UI_STYLE_COMPUTED_HPP
#define UI_STYLE_COMPUTED_HPP

#include "OE/UI/Style/StyleEnums.hpp"

namespace OrbitEngine { namespace UI {

	struct StyleValueComputed {
		bool populated = false; // value is ready
		bool inherited = false; // value is being inherited
		StyleValue value;
	};

	struct StyleComputed {
		StyleValueComputed width;
		StyleValueComputed height;
		StyleValueComputed minWidth;
		StyleValueComputed minHeight;
		StyleValueComputed maxWidth;
		StyleValueComputed maxHeight;

		StyleValueComputed marginLeft;
		StyleValueComputed marginTop;
		StyleValueComputed marginRight;
		StyleValueComputed marginBottom;

		StyleValueComputed paddingLeft;
		StyleValueComputed paddingTop;
		StyleValueComputed paddingRight;
		StyleValueComputed paddingBottom;

		StyleValueComputed borderColor;
		StyleValueComputed borderTopLeftRadius;
		StyleValueComputed borderTopRightRadius;
		StyleValueComputed borderBottomLeftRadius;
		StyleValueComputed borderBottomRightRadius;
		StyleValueComputed borderLeftWidth;
		StyleValueComputed borderTopWidth;
		StyleValueComputed borderRightWidth;
		StyleValueComputed borderBottomWidth;

		StyleValueComputed flexGrow;
		StyleValueComputed flexShrink;
		StyleValueComputed flexBasis;
		StyleValueComputed flexDirection;
		StyleValueComputed flexWrap;

		StyleValueComputed alignSelf;
		StyleValueComputed alignItems;
		StyleValueComputed alignContent;
		StyleValueComputed justifyContent;

		StyleValueComputed position;
		StyleValueComputed left;
		StyleValueComputed top;
		StyleValueComputed right;
		StyleValueComputed bottom;

		StyleValueComputed color;
		StyleValueComputed backgroundColor;

		StyleValueComputed overflow;
		StyleValueComputed display;

		StyleValueComputed fontSize;
		StyleValueComputed whiteSpace;

		StyleValueComputed cursor;

		void applyProperty(const StyleProperty& property);
		void applyRule(const StyleRule& rule);
		void inherit(const StyleComputed& source);
	};

	constexpr StyleComputed GetDefaultStyleValues()
	{
		StyleComputed defaults = { };
		
		#define SET_LENGTH(prop_name, _value, _unit) \
		defaults.prop_name.populated = true; \
		defaults.prop_name.inherited = true; \
		defaults.prop_name.value.length.value = _value; \
		defaults.prop_name.value.length.unit = _unit;

		#define SET_COLOR(prop_name, _r, _g, _b, _a) \
		defaults.prop_name.populated = true; \
		defaults.prop_name.inherited = true; \
		defaults.prop_name.value.color.r = _r; \
		defaults.prop_name.value.color.g = _g; \
		defaults.prop_name.value.color.b = _b; \
		defaults.prop_name.value.color.a = _a;

		#define SET_ENUM(prop_name, _enum_key, _enum_value) \
		defaults.prop_name.populated = true; \
		defaults.prop_name.inherited = true; \
		defaults.prop_name.value._enum_key = _enum_value;

		SET_COLOR(color, 1, 1, 1, 1); // white
		SET_COLOR(backgroundColor, 0, 0, 0, 0); // transparent
		
		SET_ENUM(overflow, overflow, Overflow::VISIBLE);
		SET_ENUM(display, display, Display::FLEX);
		
		SET_LENGTH(fontSize, 14, StyleLengthUnit::PIXELS);
		SET_ENUM(whiteSpace, whiteSpace, WhiteSpace::NORMAL);

		#undef SET_LENGTH
		#undef SET_COLOR

		return defaults;
	}
	
} }

#endif