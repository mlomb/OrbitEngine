#include "OE/UI/HierarchyElement.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace UI {

	HierarchyElement::HierarchyElement()
		: m_Root(this), m_Parent(nullptr), m_Depth(0)
	{
	}

	HierarchyElement::~HierarchyElement()
	{
		OE_ASSERT_MSG(m_Parent == nullptr, "Element is still attached to the tree");
	}

	void HierarchyElement::addElement(HierarchyElement* child, uint32_t index)
	{
		if (child->m_Parent != nullptr) {
			if (child->m_Parent == this) {
				// TODO: handle reposition
				return;
			}
			else {
				child->m_Parent->removeElement(child);
			}
		}

		child->m_Root = m_Root;
		child->m_Parent = this;
		child->m_Depth = m_Depth + 1;
		m_Childrens.insert(m_Childrens.begin() + index, child);
	}

	void HierarchyElement::removeElement(HierarchyElement* child)
	{
		OE_ASSERT(child->m_Parent == this); // TODO: maybe do a runtime check?

		child->m_Root = child; // itself
		child->m_Parent = nullptr;
		child->m_Depth = m_Depth + 1;
		m_Childrens.erase(std::find(m_Childrens.begin(), m_Childrens.end(), child));
		
		// TODO: mark dirty root (and depth) on childrens of child
	}

	HierarchyElement* HierarchyElement::getParent() const
	{
		return m_Parent;
	}

} }
