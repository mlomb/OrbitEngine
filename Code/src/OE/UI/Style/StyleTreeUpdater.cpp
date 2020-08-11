#include "OE/UI/Style/StyleTreeUpdater.hpp"

#include "OE/UI/Element.hpp"
#include "OE/UI/Surface.hpp"

#include "OE/UI/Style/StyleSheet.hpp"
#include "OE/UI/Style/StyleComputed.hpp"

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
		else // load defaults
			element->m_ComputedStyle->inherit(GetDefaultStyleValues());

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
		/*StyleHash selectorsHash = 0;
		for (const SelectorMatch& match : m_MatchedSelectors) {
			match.
		}*/

		StyleComputed* computedStyle = new StyleComputed();
		for (const SelectorMatch& match : m_MatchedSelectors) {
			OE_ASSERT(match.selector->rule);
			computedStyle->applyRule(match.selector->rule);
		}

		if (element->m_ComputedStyle)
			delete element->m_ComputedStyle;
		element->m_ComputedStyle = computedStyle;
	}


} }
