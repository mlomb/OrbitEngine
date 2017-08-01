#include "OE/UI/Layouts/AlignableLayout.hpp"


namespace OrbitEngine {	namespace UI {
	Math::Vec2f AlignableLayout::getAlignedPosition(Math::Vec2f innerSize, Math::Vec2f outerSize)
	{
		return GetAlignedPosition(m_Alignment, innerSize, outerSize);
	}

	Math::Vec2f AlignableLayout::GetAlignedPosition(RectAlignment alignment, Math::Vec2f innerSize, Math::Vec2f outerSize)
	{
		Math::Vec2f position = Math::Vec2f();

		Alignment xAlignment = RectAlignmentToAlignment(alignment, false);
		Alignment yAlignment = RectAlignmentToAlignment(alignment, true);

		position.x = AlignAxis(xAlignment, innerSize.x, outerSize.x);
		position.y = AlignAxis(yAlignment, innerSize.y, outerSize.y);

		return position;
	}

	float AlignableLayout::AlignAxis(Alignment alignment, float inner, float outer)
	{
		switch (alignment)
		{
		default:
		case Alignment::UPPER:
			return 0;
		case Alignment::MIDDLE:
			return outer / 2.0f - inner / 2.0f;
		case Alignment::LOWER:
			return outer - inner;
		}
	}

	Alignment AlignableLayout::RectAlignmentToAlignment(RectAlignment rectAlignment, bool axis)
	{
		if (axis) {
			switch (rectAlignment)
			{
			default:
			case RectAlignment::UPPER_LEFT:
			case RectAlignment::UPPER_CENTER:
			case RectAlignment::UPPER_RIGHT:
				return Alignment::UPPER;
			case RectAlignment::MIDDLE_LEFT:
			case RectAlignment::MIDDLE_CENTER:
			case RectAlignment::MIDDLE_RIGHT:
				return Alignment::MIDDLE;
			case RectAlignment::LOWER_LEFT:
			case RectAlignment::LOWER_CENTER:
			case RectAlignment::LOWER_RIGHT:
				return Alignment::LOWER;
			}
		}
		else {
			switch (rectAlignment)
			{
			default:
			case RectAlignment::UPPER_LEFT:
			case RectAlignment::MIDDLE_LEFT:
			case RectAlignment::LOWER_LEFT:
				return Alignment::UPPER;
			case RectAlignment::UPPER_CENTER:
			case RectAlignment::MIDDLE_CENTER:
			case RectAlignment::LOWER_CENTER:
				return Alignment::MIDDLE;
			case RectAlignment::UPPER_RIGHT:
			case RectAlignment::MIDDLE_RIGHT:
			case RectAlignment::LOWER_RIGHT:
				return Alignment::LOWER;
			}
		}
	}
} }