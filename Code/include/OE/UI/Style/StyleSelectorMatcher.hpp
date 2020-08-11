#ifndef UI_STYLE_SELECTOR_MATCHER_HPP
#define UI_STYLE_SELECTOR_MATCHER_HPP

namespace OrbitEngine { namespace UI {

	class Element;
	class StyleSelector;

	class StyleSelectorMatcher {
	public:

		static bool MatchesSelector(Element* element, StyleSelector* selector);

	private:
		StyleSelectorMatcher(); // don't instance
	};

} }

#endif