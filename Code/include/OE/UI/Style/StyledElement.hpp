#ifndef UI_STYLED_ELEMENT_HPP
#define UI_STYLED_ELEMENT_HPP

#include <vector>
#include <string>

#include "OE/UI/HierarchyElement.hpp"
#include "OE/UI/Style/StyleSelector.hpp"
#include "OE/UI/Style/StyleEnums.hpp"

namespace OrbitEngine { namespace UI {
	class StyledElement : public HierarchyElement {
	public:
		StyledElement();
		virtual ~StyledElement();

		bool matchesSelector(const StyleSelector* selector);

		void setID(const std::string& id);
		void addClass(const std::string& klass);

	protected:
		void setTag(const std::string& tag);

	private:
		StyleIdentifier m_ID, m_Tag;
		std::vector<StyleIdentifier> m_Classes;
		StylePseudoStates m_PseudoStates;
	};

} }

#endif