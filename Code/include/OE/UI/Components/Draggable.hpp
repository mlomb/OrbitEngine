#ifndef UI_COMPONENT_DRAGGABLE_HPP
#define UI_COMPONENT_DRAGGABLE_HPP

#include "OE/UI/Element.hpp"
#include "OE/UI/Events/EventHandler.hpp"

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace UI {

	class Draggable : public Element {
	public:
		Draggable();
		virtual ~Draggable();

		void executeDefault(EventBase* evt) override;

	private:
		bool m_Dragging;
		Math::Vec2f m_DragStart, m_DragOffset;
	};

} }

#endif