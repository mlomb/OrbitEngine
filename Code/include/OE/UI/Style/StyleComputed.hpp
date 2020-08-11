#ifndef UI_STYLE_COMPUTED_HPP
#define UI_STYLE_COMPUTED_HPP

#include "OE/UI/Style/StyleRule.hpp"

namespace OrbitEngine { namespace UI {

	struct StyleValueComputed {
		bool populated = false; // value is ready
		bool inherited = false; // value is being inherited
		StyleValue value;
	};

	struct StyleComputed {
		StyleValueComputed width;
		StyleValueComputed height;
		StyleValueComputed min_width;
		StyleValueComputed min_height;
		StyleValueComputed max_width;
		StyleValueComputed max_height;

		StyleValueComputed margin_left;
		StyleValueComputed margin_top;
		StyleValueComputed margin_right;
		StyleValueComputed margin_bottom;

		StyleValueComputed padding_left;
		StyleValueComputed padding_top;
		StyleValueComputed padding_right;
		StyleValueComputed padding_bottom;

		StyleValueComputed border_color;
		StyleValueComputed border_top_left_radius;
		StyleValueComputed border_top_right_radius;
		StyleValueComputed border_bottom_left_radius;
		StyleValueComputed border_bottom_right_radius;
		StyleValueComputed border_left_width;
		StyleValueComputed border_top_width;
		StyleValueComputed border_right_width;
		StyleValueComputed border_bottom_width;

		StyleValueComputed flex_grow;
		StyleValueComputed flex_shrink;
		StyleValueComputed flex_basis;
		StyleValueComputed flex_direction;
		StyleValueComputed flex_wrap;

		StyleValueComputed align_self;
		StyleValueComputed align_items;
		StyleValueComputed align_content;
		StyleValueComputed justify_content;

		StyleValueComputed position;
		StyleValueComputed left;
		StyleValueComputed top;
		StyleValueComputed right;
		StyleValueComputed bottom;

		StyleValueComputed color;
		StyleValueComputed background_color;

		StyleValueComputed overflow;
		StyleValueComputed display;

		StyleValueComputed font_size;
		StyleValueComputed white_space;

		StyleValueComputed cursor;

		void applyProperty(const StyleProperty& property);
		void applyRule(const StyleRule* rule);
		void inherit(const StyleComputed& source);

	};

	constexpr StyleComputed GetDefaultStyleValues()
	{
		StyleComputed defaults = { };

		return defaults;
	}

} }

#endif