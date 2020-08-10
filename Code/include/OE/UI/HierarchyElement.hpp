#ifndef UI_HIERARCHY_ELEMENT_HPP
#define UI_HIERARCHY_ELEMENT_HPP

#include <vector>

namespace OrbitEngine { namespace UI {

	class HierarchyElement {
	public:
		HierarchyElement();
		virtual ~HierarchyElement();

		void addElement(HierarchyElement* child, uint32_t index);
		void removeElement(HierarchyElement* child);
		HierarchyElement* getParent() const;

	private:
		HierarchyElement* m_Root;
		HierarchyElement* m_Parent;
		std::vector<HierarchyElement*> m_Childrens;
		int m_Depth;
	};

} }

#endif