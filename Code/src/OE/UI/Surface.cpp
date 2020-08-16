#include "OE/UI/Surface.hpp"

#include "OE/UI/Element.hpp"
#include "OE/UI/Style/StyleTreeUpdater.hpp"
#include "OE/UI/Style/StyleComputed.hpp"
#include "OE/UI/Layout/LayoutTreeUpdater.hpp"
#include "OE/UI/Render/TreePainter.hpp"
#include "OE/UI/Events/EventsController.hpp"

namespace OrbitEngine { namespace UI {
	
	Surface::Surface()
		: m_Size(100, 100)
	{
		m_EventsController = new EventsController(this);
		m_StyleUpdater = new StyleTreeUpdater(this);
		m_LayoutUpdater = new LayoutTreeUpdater(this);
		m_TreePainter = new TreePainter(this);
		m_Root = new Element();
		m_Root->m_Depth = 1;
		m_Root->m_Surface = this;
	}

	Surface::~Surface()
	{
		delete m_EventsController;
		delete m_StyleUpdater;
		delete m_LayoutUpdater;
		delete m_TreePainter;
		delete m_Root;
	}

	void Surface::updateTree()
	{
		m_EventsController->process();
		m_StyleUpdater->process();
		m_LayoutUpdater->process();
		m_TreePainter->process();
	}

	Element* Surface::getRoot() const
	{
		return m_Root;
	}

	void Surface::setSize(const Math::Vec2i& size)
	{
		m_Size = size;
	}

	Element* Surface::findElementsAt(Element* root, const Math::Vec2f& point, std::vector<Element*>* found)
	{
		if (!root->isVisible())
			return nullptr;

		const auto& bbox = root->getBoundingRect();

		if (bbox.contains(point)) {
			if (found)
				found->push_back(root);
			
			const auto& childs = root->getChildrens();

			// iterate in reverse order (z-index)
			for (auto it = childs.rbegin(); it != childs.rend(); it++) {
				Element* result = findElementsAt(*it, point, found);
				if (result)
					return result;
			}
			return root;
		}

		// no hit
		return nullptr;
	}

	Math::Vec2i Surface::getSize() const
	{
		return m_Size;
	}

	EventsController* Surface::getEventsController() const
	{
		return m_EventsController;
	}

	StyleCursor Surface::getCurrentCusor() const
	{
		Element* underCursor = m_EventsController->getElementOverMouse();
		return underCursor ? underCursor->m_ComputedStyle->cursor.value : StyleCursor::AUTO;
	}
} }
