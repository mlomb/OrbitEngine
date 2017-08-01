#include "OE/UI/Elements/TreeViewItem.hpp"

#include "OE/UI/Elements/Panel.hpp"
#include "OE/UI/Elements/Text.hpp"

namespace OrbitEngine { namespace UI {
	Element* TreeViewItem::Build(int depth)
	{
		Panel* p = new Panel();
		p->setLayoutPropertyAlongAxis(20, LayoutProperty::MINIMUM_SIZE, Axis::VERTICAL);

		Text* displayText = new Text();
		displayText->setText(getDisplayText());
		displayText->setPivot(Math::Vec2f());
		displayText->setTextAlignament(RectAlignment::LOWER_RIGHT); // test
		displayText->setAnchor(Anchor::STRETCH_BOTH);
		displayText->setMarginsAlongAxis(Math::Vec2f(20 * depth, 0), Axis::HORIZONTAL);
		displayText->setFontSize(14.0f);

		p->add(displayText);
		return p;
	}

	int UI::TreeViewItem::countChilds(bool expanded)
	{
		int count = 0;
		for (TreeViewItem* i : getChildrens()) {
			if (i->isExpanded() == expanded) {
				count++;
				count += i->countChilds(expanded);
			}
		}
		return count;
	}
} }