#include "OE/UI/Style/StyleTreeUpdater.hpp"

#include "OE/UI/Element.hpp"
#include "OE/UI/Surface.hpp"

#include "OE/UI/Style/StyleSheet.hpp"

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
		if (m_MatchedSelectors.size() > 0)
			processMatchedSelectors(element);

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

	}


} }
