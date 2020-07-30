#ifndef UI_LAYOUT_MANAGER_HPP
#define UI_LAYOUT_MANAGER_HPP

namespace OrbitEngine { namespace UI {
	class Element;

	class LayoutManager {
	public:
		void performLayout(Element* layoutRoot);

	protected:
		virtual void calculateLayouts(Element* element);
		virtual void setLayouts(Element* element);
	};
} }

#endif