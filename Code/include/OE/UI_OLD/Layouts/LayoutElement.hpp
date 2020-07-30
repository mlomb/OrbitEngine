#ifndef UI_LAYOUT_ELEMENT_HPP
#define UI_LAYOUT_ELEMENT_HPP

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace UI {
	enum LayoutProperty {
		MINIMUM_SIZE = 0,
		PREFERRED_SIZE,
		FLEXIBLE_SIZE
	};

	enum Axis : int {
		HORIZONTAL = 0, VERTICAL = 1
	};

	typedef Axis Orientation;

	class LayoutElement {
	public:
		virtual float getLayoutPropertyAlongAxis(LayoutProperty property, Axis axis) { return m_LayoutProperties[property][axis]; };
		void setLayoutPropertyAlongAxis(float value, LayoutProperty property, Axis axis) { m_LayoutProperties[property][axis] = value; };

		Math::Vec2f getLayoutProperty(LayoutProperty property) { return Math::Vec2f(getLayoutPropertyAlongAxis(property, (Axis)0), getLayoutPropertyAlongAxis(property, (Axis)1)); };
		void setLayoutProperty(Math::Vec2f value, LayoutProperty property) { m_LayoutProperties[property] = value; };

	private:
		Math::Vec2f m_LayoutProperties[3];
	protected:
		LayoutElement() {};
	};
} }

#endif