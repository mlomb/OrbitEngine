#include "OE/UI/Style/StyleComputed.hpp"

namespace OrbitEngine { namespace UI {

	void StyleComputed::applyProperty(const StyleProperty& property)
	{
		using ID = StylePropertyID;

		switch (property.id) {
		#define SAVE_PROP(id, name) \
		case id: \
			name.populated = true; \
			name.inherited = false; \
			name.value = property.value; \
			break;

		SAVE_PROP(ID::WIDTH,      width);
		SAVE_PROP(ID::HEIGHT,     height);
		SAVE_PROP(ID::MIN_WIDTH,  min_width);
		SAVE_PROP(ID::MIN_HEIGHT, min_height);
		SAVE_PROP(ID::MAX_WIDTH,  max_width);
		SAVE_PROP(ID::MAX_HEIGHT, max_height);
		
		SAVE_PROP(ID::MARGIN_LEFT,   margin_left);
		SAVE_PROP(ID::MARGIN_TOP,    margin_top);
		SAVE_PROP(ID::MARGIN_RIGHT,  margin_right);
		SAVE_PROP(ID::MARGIN_BOTTOM, margin_bottom);

		SAVE_PROP(ID::PADDING_LEFT,   padding_left);
		SAVE_PROP(ID::PADDING_TOP,    padding_top);
		SAVE_PROP(ID::PADDING_RIGHT,  padding_right);
		SAVE_PROP(ID::PADDING_BOTTOM, padding_bottom);

		SAVE_PROP(ID::BORDER_COLOR, border_color);
		SAVE_PROP(ID::BORDER_TOP_LEFT_RADIUS,     border_top_left_radius);
		SAVE_PROP(ID::BORDER_TOP_RIGHT_RADIUS,    border_top_right_radius);
		SAVE_PROP(ID::BORDER_BOTTOM_LEFT_RADIUS,  border_bottom_left_radius);
		SAVE_PROP(ID::BORDER_BOTTOM_RIGHT_RADIUS, border_bottom_right_radius);
		SAVE_PROP(ID::BORDER_LEFT_WIDTH,   border_left_width);
		SAVE_PROP(ID::BORDER_TOP_WIDTH,    border_top_width);
		SAVE_PROP(ID::BORDER_RIGHT_WIDTH,  border_right_width);
		SAVE_PROP(ID::BORDER_BOTTOM_WIDTH, border_bottom_width);

		SAVE_PROP(ID::FLEX_GROW,      flex_grow);
		SAVE_PROP(ID::FLEX_SHRINK,    flex_shrink);
		SAVE_PROP(ID::FLEX_BASIS,     flex_basis);
		SAVE_PROP(ID::FLEX_DIRECTION, flex_direction);
		SAVE_PROP(ID::FLEX_WRAP,      flex_wrap);

		SAVE_PROP(ID::ALIGN_SELF,      align_self);
		SAVE_PROP(ID::ALIGN_ITEMS,     align_items);
		SAVE_PROP(ID::ALIGN_CONTENT,   align_content);
		SAVE_PROP(ID::JUSTIFY_CONTENT, justify_content);

		SAVE_PROP(ID::POSITION, position);
		SAVE_PROP(ID::LEFT,   left);
		SAVE_PROP(ID::TOP,    top);
		SAVE_PROP(ID::RIGHT,  right);
		SAVE_PROP(ID::BOTTOM, bottom);

		SAVE_PROP(ID::COLOR, color);
		SAVE_PROP(ID::BACKGROUND_COLOR, background_color);

		SAVE_PROP(ID::OVERFLOW, overflow);
		SAVE_PROP(ID::DISPLAY, display);

		SAVE_PROP(ID::FONT_SIZE, font_size);
		SAVE_PROP(ID::WHITE_SPACE, white_space);
		}
	}

	void StyleComputed::applyRule(const StyleRule* rule)
	{
		for (const StyleProperty& prop : rule->properties)
			applyProperty(prop);
	}

	void StyleComputed::inherit(const StyleComputed& source)
	{
		#define INHERIT(prop_name) \
		if (!this->prop_name.populated || this->prop_name.inherited) { \
			this->prop_name.inherited = true; \
			this->prop_name.populated = true; \
			this->prop_name.value = source.prop_name.value; \
		}

		// only some properties are inherited

		INHERIT(color);
		INHERIT(font_size);
		INHERIT(white_space);
	}

} }
