#ifndef UI_COMPONENT_DRAGGABLE_HPP
#define UI_COMPONENT_DRAGGABLE_HPP

#include "OE/UI/Element.hpp"

namespace OrbitEngine { namespace UI {

	class Draggable : public Element {
	public:
		Draggable();
		virtual ~Draggable();

		void executeDefault(EventBase* evt) override;

	private:
		bool m_Dragging;
	};

} }

#endif