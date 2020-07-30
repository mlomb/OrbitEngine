#ifndef UI_TREE_VIEW_HPP
#define UI_TREE_VIEW_HPP

#include "OE/UI/Elements/Element.hpp"
#include "OE/UI/Elements/TreeViewItem.hpp"

namespace OrbitEngine { namespace UI {
	class TreeView : public Element {
	public:
		TreeView();

		void setRoot(TreeViewItem* root) { m_Root = root; }
		/*
		void setModel(TreeViewModel* model) { m_Model = model; }
		TreeViewModel* m_Model;
		*/
	private:
		void onUpdate(float deltaTime) override;

		void generateTree(TreeViewItem* item, int depth);
		
		TreeViewItem* m_Root;
	};
} }

#endif