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
		
		#define SET_NUMBER(prop_name, _value) \
		defaults.prop_name.populated = true; \
		defaults.prop_name.inherited = true; \
		defaults.prop_name.value.number = _value;

		#define SET_LENGTH(prop_name, _value, _unit) \
		defaults.prop_name.populated = true; \
		defaults.prop_name.inherited = true; \
		defaults.prop_name.value.length.number = _value; \
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

		#define SET_AUTO(prop_name) \
		defaults.prop_name.populated = true; \
		defaults.prop_name.inherited = true; \
		defaults.prop_name.value.set_auto = true;

		SET_AUTO(width);
		SET_AUTO(height);
		SET_AUTO(minWidth);
		SET_AUTO(minHeight);
		SET_AUTO(maxWidth);
		SET_AUTO(maxHeight);

		SET_LENGTH(marginLeft, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(marginTop, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(marginRight, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(marginBottom, 0, StyleLengthUnit::PIXELS);

		SET_LENGTH(paddingLeft, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(paddingTop, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(paddingRight, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(paddingBottom, 0, StyleLengthUnit::PIXELS);

		SET_COLOR(borderColor, 0, 0, 0, 0); // transparent
		SET_LENGTH(borderTopLeftRadius, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(borderTopRightRadius, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(borderBottomLeftRadius, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(borderBottomRightRadius, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(borderLeftWidth, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(borderTopWidth, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(borderRightWidth, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(borderBottomWidth, 0, StyleLengthUnit::PIXELS);

		SET_NUMBER(flexGrow, 0);
		SET_NUMBER(flexShrink, 1);
		SET_AUTO(flexBasis);

		SET_ENUM(flexDirection, direction, FlexDirection::ROW);
		SET_ENUM(flexWrap, wrap, FlexWrap::NOWRAP);
		SET_ENUM(alignSelf, align, Align::AUTO);
		SET_ENUM(alignItems, align, Align::STRETCH);
		SET_ENUM(alignContent, align, Align::FLEX_START);
		SET_ENUM(justifyContent, justify, Justify::FLEX_START);

		SET_ENUM(position, position, (Position)0); // Position::RELATIVE
		SET_LENGTH(left, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(top, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(right, 0, StyleLengthUnit::PIXELS);
		SET_LENGTH(bottom, 0, StyleLengthUnit::PIXELS);

		SET_COLOR(color, 1, 1, 1, 1); // white
		SET_COLOR(backgroundColor, 0, 0, 0, 0); // transparent
		
		SET_ENUM(overflow, overflow, Overflow::VISIBLE);
		SET_ENUM(display, display, Display::FLEX);
		
		SET_LENGTH(fontSize, 14, StyleLengthUnit::PIXELS);
		SET_ENUM(whiteSpace, whiteSpace, WhiteSpace::NORMAL);

		SET_ENUM(cursor, cursor, StyleCursor::AUTO);

		#undef SET_NUMBER
		#undef SET_LENGTH
		#undef SET_COLOR
		#undef SET_ENUM
		#undef SET_AUTO

		return defaults;
	}
	
} }

#endif