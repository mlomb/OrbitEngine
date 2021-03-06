#ifndef UI_SURFACE_HPP
#define UI_SURFACE_HPP

#include <vector>

#include "OE/Math/Vec2.hpp"

#include "OE/UI/Style/StyleEnums.hpp"

namespace OrbitEngine { namespace UI {

	class Element;
	class StyleTreeUpdater;
	class LayoutTreeUpdater;
	class TreePainter;
	class EventsController;

	class Surface {
	public:
		Surface();
		~Surface();

		void updateTree();

		void setSize(const Math::Vec2i& size);
		Element* findElementsAt(Element* root, const Math::Vec2f& point, std::vector<Element*>* found = nullptr);
		EventsController* getEventsController() const;
		StyleCursor getCurrentCusor() const;

		Element* getRoot() const;
		Math::Vec2i getSize() const;

	private:
		Element* m_Root;

		StyleTreeUpdater* m_StyleUpdater;
		LayoutTreeUpdater* m_LayoutUpdater;
		TreePainter* m_TreePainter;
		EventsController* m_EventsController;

		Math::Vec2i m_Size;
	};

} }

#endif