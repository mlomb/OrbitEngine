#include "OE/UI/Style/StyleSelectorMatcher.hpp"

#include "OE/UI/Element.hpp"
#include "OE/UI/Style/StyleEnums.hpp"
#include "OE/UI/Style/StyleSheet.hpp"

namespace OrbitEngine { namespace UI {

    bool UI::SelectorMatch::operator<(const SelectorMatch& b) const
    {
        if (selector->specificity != b.selector->specificity)
            return selector->specificity < b.selector->specificity; // prefer higher specificity
        if (stylesheetIndex != b.stylesheetIndex)
            return stylesheetIndex < b.stylesheetIndex; // prefer stylesheet precedence
        if (selector->order != b.selector->order)
            return selector->order < b.selector->order; // prefer precedence in the same stylesheet
        return false; // tie
    }

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

    void StyleSelectorMatcher::FindMatches(Element* element, std::vector<StyleSheet*>& sheets, std::vector<SelectorMatch>& matches)
    {
        const StyleHash empty_id = HashStr("");

        SelectorMatch match;

        for (int i = 0; i < sheets.size(); i++) {
            StyleSheet* sheet = sheets[i];

            match.stylesheetIndex = i;
            match.sheet = sheet;

            #define FAST_LOOKUP(hash, lookupTable) \
            { \
                auto it = sheet->lookupTable.find(hash); \
                if (it != sheet->lookupTable.end()) { \
                    const auto& relevant = (*it).second; \
                    for (StyleSelector* selector : relevant) { \
                        if (MatchesSelector(element, selector)) { \
                            match.selector = selector; \
                            matches.push_back(match); \
                        } \
                    } \
                } \
            }

            FAST_LOOKUP(HashStr("*"), m_OrderedTags); // match wildcard
            FAST_LOOKUP(element->m_Tag.text_hash, m_OrderedTags);

            if (element->m_ID.text_hash != empty_id)
                FAST_LOOKUP(element->m_ID.text_hash, m_OrderedIDs);

            for (auto& klass : element->m_Classes)
                FAST_LOOKUP(klass.text_hash, m_OrderedClasses);
        }
    }

} }