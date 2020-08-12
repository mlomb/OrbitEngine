#include "OE/UI/Style/StyleTreeUpdater.hpp"

#include "OE/UI/Element.hpp"
#include "OE/UI/Surface.hpp"

#include "OE/UI/Style/StyleSheet.hpp"
#include "OE/UI/Style/StyleComputed.hpp"

#include "OE/UI/Layout/Yoga.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace UI {

	StyleTreeUpdater::StyleTreeUpdater(Surface* surface)
		: TreeUpdater(surface)
	{
	}

	StyleTreeUpdater::~StyleTreeUpdater()
	{
	}

	void StyleTreeUpdater::update()
	{
		OE_ASSERT(m_SheetsStack.size() == 0);
		applyStyles(m_Surface->getRoot());
	}

	void StyleTreeUpdater::onVersionChange(Element* element, VersionChangeType changes)
	{
	}

	void StyleTreeUpdater::applyStyles(Element* element)
	{
		// TODO: check if we can skip this element

		// add this element's sheets to the sheets stack
		for (StyleSheet* ss : element->getStyleSheets())
			m_SheetsStack.push_back(ss);

		// find all selector matches
		m_MatchedSelectors.clear();
		StyleSelectorMatcher::FindMatches(element, m_SheetsStack, m_MatchedSelectors);

		// process matches
		processMatchedSelectors(element);

		// apply inherited (important: before traversing the subtree)
		if(element->m_Parent)
			element->m_ComputedStyle->inherit(*element->m_Parent->m_ComputedStyle);

		// sync with yoga
		syncWithLayout(element);

		// recurse
		for (Element* child : element->getChildrens())
			applyStyles(child);

		// remove sheets from stack, sheets only affect their subtree
		for (StyleSheet* ss : element->getStyleSheets())
			m_SheetsStack.pop_back();
	}

	void StyleTreeUpdater::processMatchedSelectors(Element* element)
	{
		std::sort(m_MatchedSelectors.begin(), m_MatchedSelectors.end()); // sorted by precedence

		// TODO: calculate the hash of the matched selectors
		//       and reuse the StyleComputed*
		//       (apply inline later, use same pointer if no inline rules)
		/*StyleHash selectorsHash = 0;
		for (const SelectorMatch& match : m_MatchedSelectors) {
			match.
		}*/

		StyleComputed* computedStyle = new StyleComputed();
		*computedStyle = GetDefaultStyleValues(); // copy defaults
		for (const SelectorMatch& match : m_MatchedSelectors) {
			OE_ASSERT(match.selector->rule);
			computedStyle->applyRule(*match.selector->rule);
		}

		computedStyle->applyRule(element->m_InlineRules);

		if (element->m_ComputedStyle)
			delete element->m_ComputedStyle;
		element->m_ComputedStyle = computedStyle;
	}

	void StyleTreeUpdater::syncWithLayout(Element* element)
	{
		const StyleComputed* computedStyle = element->m_ComputedStyle;
		const YGNodeRef yogaNode = element->m_YogaNode;

		// all values *should* be populated by now

		//YGNodeStyleSetFlex(yogaNode, YGUndefined);
		//
		//YGNodeStyleSetFlexGrow(yogaNode, computedStyle->flex_grow.value.length.value);
		//YGNodeStyleSetFlexShrink(yogaNode, computedStyle->flex_shrink.value.length.value);
		//YGNodeStyleSetFlexBasis(yogaNode, computedStyle->flex_basis.value.length.value);
		//
		//YGNodeStyleSetPositionType(yogaNode, (YGPositionType)computedStyle->position.value.enum_index);
		//YGNodeStyleSetPosition(yogaNode, YGEdge::YGEdgeLeft, computedStyle->left.value.length.value);
		//YGNodeStyleSetPosition(yogaNode, YGEdge::YGEdgeTop, computedStyle->top.value.length.value);
		//YGNodeStyleSetPosition(yogaNode, YGEdge::YGEdgeRight, computedStyle->right.value.length.value);
		//YGNodeStyleSetPosition(yogaNode, YGEdge::YGEdgeBottom, computedStyle->bottom.value.length.value);

		YGNodeStyleSetPositionType(yogaNode, YGPositionType::YGPositionTypeRelative);

		if (!element->m_Parent) {
			YGNodeStyleSetFlexDirection(yogaNode, YGFlexDirectionRow);
			//YGNodeStyleSetFlexGrow(yogaNode, 0);
			//YGNodeStyleSetFlexShrink(yogaNode, 1);
			//YGNodeStyleSetFlexBasisAuto(yogaNode);
			//YGNodeStyleSetFlexWrap(yogaNode, YGWrap::YGWrapWrap);
			//YGNodeStyleSetAlignItems(yogaNode, YGAlign::YGAlignCenter);
		}
		else {
			YGNodeStyleSetAlignSelf(yogaNode, YGAlignCenter);
			if (!YGNodeHasMeasureFunc(yogaNode)) {
				YGNodeStyleSetPadding(yogaNode, YGEdge::YGEdgeLeft, 5);
				YGNodeStyleSetPadding(yogaNode, YGEdge::YGEdgeTop, 5);
				YGNodeStyleSetPadding(yogaNode, YGEdge::YGEdgeRight, 5);
				YGNodeStyleSetPadding(yogaNode, YGEdge::YGEdgeBottom, 5);
			}
			else {
				YGNodeStyleSetMaxWidth(yogaNode, 500);
			}
		}

		//YGNodeStyleSetMargin(yogaNode, YGEdge::YGEdgeLeft, computedStyle->margin_left.value.length.value);
		//YGNodeStyleSetMargin(yogaNode, YGEdge::YGEdgeTop, computedStyle->margin_top.value.length.value);
		//YGNodeStyleSetMargin(yogaNode, YGEdge::YGEdgeRight, computedStyle->margin_right.value.length.value);
		//YGNodeStyleSetMargin(yogaNode, YGEdge::YGEdgeBottom, computedStyle->margin_bottom.value.length.value);
		//
		//YGNodeStyleSetPadding(yogaNode, YGEdge::YGEdgeLeft, computedStyle->padding_left.value.length.value);
		//YGNodeStyleSetPadding(yogaNode, YGEdge::YGEdgeTop, computedStyle->padding_top.value.length.value);
		//YGNodeStyleSetPadding(yogaNode, YGEdge::YGEdgeRight, computedStyle->padding_right.value.length.value);
		//YGNodeStyleSetPadding(yogaNode, YGEdge::YGEdgeBottom, computedStyle->padding_bottom.value.length.value);
	}

} }
