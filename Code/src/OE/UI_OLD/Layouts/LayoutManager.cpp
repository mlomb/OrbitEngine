#include "OE/UI/Layouts/LayoutManager.hpp"

#include "OE/UI/Elements/Element.hpp"
#include "OE/UI/Layouts/Layout.hpp"

namespace OrbitEngine { namespace UI {
	void LayoutManager::performLayout(Element* layoutRoot)
	{
		// 1. Calculate from bottom to top the preferred and minimum size of everything (element + layouts)
		// 2. Set the layouts, determinate positions

		calculateLayouts(layoutRoot);
		setLayouts(layoutRoot);
	}

	void LayoutManager::calculateLayouts(Element* element)
	{
		//if (element->m_Valid)
		//	return;

		for (Element* e : element->getChildrens())
			calculateLayouts(e);

		if(Layout* layout = element->getLayout())
			layout->calculateLayout(element);
	}

	void LayoutManager::setLayouts(Element* element)
	{
		//if (element->m_Valid)
		//	return;

		element->onValidate();

		element->calcAlongAxis(Axis::HORIZONTAL);
		element->calcAlongAxis(Axis::VERTICAL);

		if (Layout* layout = element->getLayout())
			layout->setLayout(element);

		for (Element* e : element->getChildrens())
			setLayouts(e);

		element->m_Valid = true;
	}
} }