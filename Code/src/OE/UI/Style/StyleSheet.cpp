#include "OE/UI/Style/StyleSheet.hpp"

namespace OrbitEngine { namespace UI {

	int StyleSheet::addRule(const StyleRule& rule)
	{
		m_Rules.emplace_back(rule);
		return m_Rules.size() - 1;
	}

	int StyleSheet::addSelector(const StyleSelector& selector)
	{
		m_Selectors.emplace_back(selector);
		return m_Selectors.size() - 1;
	}

} }
