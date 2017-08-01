#ifndef UI_TREE_VIEW_ITEM_HPP
#define UI_TREE_VIEW_ITEM_HPP

#include <string>
#include <vector>

namespace OrbitEngine { namespace UI {
	class Element;

	class TreeViewItem {
	public:
		virtual std::vector<TreeViewItem*> getChildrens() { return std::vector<TreeViewItem*>(); };
		virtual std::string getDisplayText() { return "TreeItem"; };
		
		virtual Element* Build(int depth);

		void invalidate() { m_Updated = false; }
		void expand() { m_IsExpanded = true; invalidate(); }

		bool isExpanded() { return m_IsExpanded; }
		bool isUpdated() { return m_Updated; }
		int countChilds(bool expanded);
	private:
		friend class TreeView;

		bool m_IsExpanded = true;
		bool m_Updated = false;
	};
} }

#endif