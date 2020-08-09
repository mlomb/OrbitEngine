#ifndef UI_STYLE_HPP
#define UI_STYLE_HPP

#include <yoga/YGEnums.h>

#include <OE/Math/Color.hpp>

#undef RELATIVE
#undef ABSOLUTE

namespace OrbitEngine { namespace UI {

	enum class Position {
		RELATIVE = YGPositionType::YGPositionTypeRelative,
		ABSOLUTE = YGPositionType::YGPositionTypeAbsolute
	};

	enum class FlexDirection {
		COLUMN = YGFlexDirection::YGFlexDirectionColumn,
		COLUMN_REVERSE = YGFlexDirection::YGFlexDirectionColumnReverse,
		ROW = YGFlexDirection::YGFlexDirectionRow,
		ROW_REVERSE = YGFlexDirection::YGFlexDirectionRowReverse
	};

	enum class Wrap {
		NOWRAP = YGWrap::YGWrapNoWrap,
		WRAP = YGWrap::YGWrapWrap,
		WRAP_REVERSE = YGWrap::YGWrapWrapReverse,
	};

	enum class Align {
		AUTO = YGAlign::YGAlignAuto,
		FLEX_START = YGAlign::YGAlignFlexStart,
		CENTER = YGAlign::YGAlignCenter,
		FLEX_END = YGAlign::YGAlignFlexEnd,
		STRETCH = YGAlign::YGAlignStretch,
		BASELINE = YGAlign::YGAlignBaseline,
		SPACE_BETWEEN = YGAlign::YGAlignSpaceBetween,
		SPACE_AROUND = YGAlign::YGAlignSpaceAround
	};

	enum class Justify {
		FLEX_START = YGJustify::YGJustifyFlexStart,
		CENTER = YGJustify::YGJustifyCenter,
		FLEX_END = YGJustify::YGJustifyFlexEnd,
		SPACE_BETWEEN = YGJustify::YGJustifySpaceBetween,
		SPACE_AROUND = YGJustify::YGJustifySpaceAround,
		SPACE_EVENLY = YGJustify::YGJustifySpaceEvenly
	};

	enum class WhiteSpace {
		NORMAL = 0,
		NOWRAP = 1,
	};

	enum class Display {
		FLEX = 0,
		NONE = 1,
	};

	struct ResolvedStyle {

		Math::Color4f background = Math::Color4f(0, 0, 0 ,0); // transparent
	};

} }

#endif