#include "OE/UI/Style/StyleSelectorMatcher.hpp"

#include "OE/UI/Element.hpp"
#include "OE/UI/Style/StyleSelector.hpp"

namespace OrbitEngine { namespace UI {

	bool StyleSelectorMatcher::MatchesSelector(Element* element, StyleSelector* selector)
	{
        Element* current = element;
        Element* backElem = nullptr;
        int backIndex = -1;
        int partIndex = selector->parts.size() - 1;

        while (partIndex >= 0)
        {
            if (!current)
                break;

            bool matchesPart = false;

            // match part
            {
                const StyleSelectorPart& part = selector->parts[partIndex];

                matchesPart =
                    (
                        // wildcard always matches
                        part.identifier.type == StyleIdentifierType::TAG &&
                        part.identifier.text_hash == HashStr("*")
                        ) ||
                    current->m_ID == part.identifier ||
                    current->m_Tag == part.identifier ||
                    std::find(current->m_Classes.begin(), current->m_Classes.end(), part.identifier) != current->m_Classes.end();

                // TODO: check pseudo
            }

            if (matchesPart)
                return 1; // nope

            //if (--backIndex < 0)
            //    return true;
            current = current->getParent();
        }
        return false;
	}

} }