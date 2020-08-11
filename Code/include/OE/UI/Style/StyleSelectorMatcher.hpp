#ifndef UI_STYLE_SELECTOR_MATCHER_HPP
#define UI_STYLE_SELECTOR_MATCHER_HPP

#include <vector>

namespace OrbitEngine { namespace UI {

	class Element;
	class StyleSheet;
	class StyleSelector;

	struct SelectorMatch {
		StyleSheet* sheet;
		StyleSelector* selector;
		int stylesheetIndex;

		bool operator<(const SelectorMatch& b) const;
	};

	class StyleSelectorMatcher {
	public:

		static bool MatchesSelector(Element* element, StyleSelector* selector);

		/// May insert duplicates
		static void FindMatches(Element* element, std::vector<StyleSheet*>& sheets, std::vector<SelectorMatch>& matches);

	private:
		StyleSelectorMatcher(); // don't instance
	};

} }

#endif