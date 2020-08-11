#include "OE/UI/Layout/LayoutTreeUpdater.hpp"

#include "OE/UI/Surface.hpp"
#include "OE/UI/Element.hpp"

#include "OE/UI/Layout/Yoga.hpp"

namespace OrbitEngine { namespace UI {

	LayoutTreeUpdater::LayoutTreeUpdater(Surface* surface)
		: TreeUpdater(surface)
	{
	}

	LayoutTreeUpdater::~LayoutTreeUpdater()
	{
	}

	void LayoutTreeUpdater::update()
	{
		auto size = m_Surface->getSize();
		Element* root = m_Surface->getRoot();
		YGNodeRef rootYogaNode = root->m_YogaNode;

		YGNodeStyleSetWidth(rootYogaNode, size.x);
		YGNodeStyleSetHeight(rootYogaNode, size.y);

		if (YGNodeIsDirty(rootYogaNode)) {
			YGNodeCalculateLayout(rootYogaNode, size.w, size.h, YGDirectionLTR);
			updateLayout(root);
		}
	}

	void LayoutTreeUpdater::onVersionChange(Element* element, VersionChangeType changes)
	{
		// check type

		if (element->m_YogaNode && YGNodeHasMeasureFunc(element->m_YogaNode))
			YGNodeMarkDirty(element->m_YogaNode);
	}

	void LayoutTreeUpdater::updateLayout(Element* element)
	{
		YGNodeRef yogaNode = element->m_YogaNode;

		Math::Rectf layoutRect = Math::Rectf(
			YGNodeLayoutGetLeft(yogaNode), YGNodeLayoutGetTop(yogaNode),
			YGNodeLayoutGetWidth(yogaNode), YGNodeLayoutGetHeight(yogaNode)
		);

		bool rectChanged = element->m_LayoutRect != layoutRect;

		if (rectChanged) {
			element->m_LayoutRect = layoutRect;

			if (element->m_LayoutRect.position != layoutRect.position) {
				element->m_BoundingBox.position = element->m_Parent ? element->m_Parent->m_BoundingBox.position : Math::Vec2f(0, 0);
				element->m_BoundingBox.position += layoutRect.position;

				// TODO: trigger position change
			}
			element->m_BoundingBox.size = layoutRect.size;

			// TODO: trigger size change
		}

		bool hasNewLayout = YGNodeGetHasNewLayout(yogaNode);
		if (hasNewLayout) {
			for (Element* e : element->m_Childrens)
				updateLayout(e);

			YGNodeSetHasNewLayout(yogaNode, false);

			// TODO: trigger repaint
		}

	}

} }
