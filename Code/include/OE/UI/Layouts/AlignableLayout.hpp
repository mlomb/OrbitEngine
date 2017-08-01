#ifndef UI_ALIGNABLE_LAYOUT_HPP
#define UI_ALIGNABLE_LAYOUT_HPP

#include "OE/UI/Elements/Element.hpp"
#include "OE/UI/Layouts/Layout.hpp"

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec4.hpp"

namespace OrbitEngine {	namespace UI {
	class AlignableLayout : virtual public Layout {
	public:

		RectAlignment getAlignment() { return m_Alignment; };
		void setAlignment(RectAlignment alignment) { m_Alignment = alignment; };

		Math::Vec2f getAlignedPosition(Math::Vec2f innerSize, Math::Vec2f outerSize);

		static Math::Vec2f GetAlignedPosition(RectAlignment alignment, Math::Vec2f innerSize, Math::Vec2f outerSize);
		static float AlignAxis(Alignment alignment, float inner, float outer);
		static Alignment RectAlignmentToAlignment(RectAlignment rectAlignment, bool axis);
	private:
		RectAlignment m_Alignment;
	};
} }

#endif