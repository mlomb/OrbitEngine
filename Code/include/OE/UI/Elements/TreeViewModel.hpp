#ifndef UI_TREE_VIEW_MODEL_HPP
#define UI_TREE_VIEW_MODEL_HPP

namespace OrbitEngine { namespace UI {
	class Element;

	class TreeViewModel {
	public:
		TreeViewModel() {};

		virtual Element* Build(int depth);

		void setRoot(void* root) { m_Root = root; }

	private:
		void* m_Root;
	};
} }

#endif