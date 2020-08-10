#include "OE/UI/Style/StyleSheet.hpp"

namespace OrbitEngine { namespace UI {

	StyleSheet::StyleSheet()
	{
	}

	StyleSheet::~StyleSheet()
	{
		for (StyleRule* rule : m_Rules)
			delete rule;
		for (StyleSelector* selector : m_Selectors)
			delete selector;
	}

	int StyleSheet::addRule(const StyleRule& rule)
	{
		StyleRule* new_rule = new StyleRule();
		*new_rule = rule; // copy
		m_Rules.emplace_back(new_rule);
		return m_Rules.size() - 1;
	}

	void StyleSheet::addSelector(const StyleSelector& selector, int rule_ref)
	{
		StyleSelector* new_selector = new StyleSelector();
		*new_selector = selector; // copy
		new_selector->rule = m_Rules[rule_ref];



		m_Selectors.emplace_back(new_selector);
	}

} }
