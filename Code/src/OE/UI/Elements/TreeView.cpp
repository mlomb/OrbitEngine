#include "OE/UI/Elements/TreeView.hpp"

#include "OE/UI/Layouts/LinearLayout.hpp"

namespace OrbitEngine { namespace UI {
	TreeView::TreeView()
	{
		LinearLayout* linearLayout = new LinearLayout();
		linearLayout->setControlChildrensSize(true, true);
		linearLayout->setExpandChildrens(true, false);
		linearLayout->setOrientation(Orientation::VERTICAL);
		linearLayout->setSpacing(3.0f);
		linearLayout->setPadding(Math::Vec4f(1, 10, 1, 10));
		linearLayout->setAlignment(UI::RectAlignment::UPPER_RIGHT);
		setLayout(linearLayout);
	}

	void TreeView::onUpdate(float deltaTime)
	{
		if (m_Root) {
			if (!m_Root->isUpdated()) {
				removeAll();
				if (!m_Root->isExpanded())
					m_Root->expand();
				generateTree(m_Root, 0);
				m_Root->m_Updated = true;
			}
		}
	}

	void TreeView::generateTree(TreeViewItem* item, int depth)
	{
		add(item->Build(depth));

		if (item->isExpanded())
			for (TreeViewItem* childItem : item->getChildrens())
				generateTree(childItem, depth + 1);
	}
} }