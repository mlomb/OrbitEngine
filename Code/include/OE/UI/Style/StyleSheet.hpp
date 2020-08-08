#ifndef UI_STYLE_SHEET_HPP
#define UI_STYLE_SHEET_HPP

#include "StyleRule.hpp"
#include "StyleSelector.hpp"

namespace OrbitEngine { namespace UI {

	class StyleSheet {
	public:

		int addRule(const StyleRule& rule);
		int addSelector(const StyleSelector& selector);

	private:
		std::vector<StyleSelector> m_Selectors;
		std::vector<StyleRule> m_Rules;

	};

} }

#endif