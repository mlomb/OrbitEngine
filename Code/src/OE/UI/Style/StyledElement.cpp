#include "OE/UI/Style/StyledElement.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace UI {

	StyledElement::StyledElement()
        : m_PseudoStates(StylePseudoStates::NONE)
	{
        setID("");
        setTag("StyledElement"); // TODO: do this in Element
	}

	StyledElement::~StyledElement()
	{
	}

	bool StyledElement::matchesSelector(const StyleSelector* selector)
	{
        StyledElement* current = this;
        StyledElement* backElem = nullptr;
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
                    m_ID == part.identifier ||
                    m_Tag == part.identifier ||
                    std::find(m_Classes.begin(), m_Classes.end(), part.identifier) != m_Classes.end();
                
                // TODO: check pseudo
            }

            if (--backIndex < 0)
                return true;
            current = static_cast<StyledElement*>(current->getParent());
        }
		return false;
	}

    void StyledElement::setID(const std::string& id)
    {
        m_ID = { StyleIdentifierType::ID, id };
        m_ID.computeHash();
    }

    void StyledElement::setTag(const std::string& tag)
    {
        m_ID = { StyleIdentifierType::TAG, tag };
        m_ID.computeHash();
    }

    void StyledElement::addClass(const std::string& klass)
    {
        StyleIdentifier si = { StyleIdentifierType::CLASS, klass };
        si.computeHash();

        for (auto it = m_Classes.begin(); it != m_Classes.end(); it++) {
            if ((*it).text_hash == si.text_hash)
                return; // already present
        }

        m_Classes.emplace_back(si);
    }

} }
