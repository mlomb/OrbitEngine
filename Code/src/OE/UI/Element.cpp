#include "OE/UI/Element.hpp"

#include "OE/Misc/Log.hpp"

#include "OE/UI/Style/StyleComputed.hpp"
#include "OE/UI/Render/Painter.hpp"
#include "OE/UI/Layout/Yoga.hpp"
#include "OE/UI/Events/Events.hpp"

namespace OrbitEngine { namespace UI {
	Element::Element() :
		m_Surface(nullptr),
		m_Parent(nullptr),
		m_Depth(0),
		m_ComputedStyle(nullptr),
		m_PseudoStates(StylePseudoStates::NONE)
	{
		setID("");
		setTag("Element");

		test = rand()% 255;

		m_YogaNode = YGNodeNew();
		YGNodeSetContext(m_YogaNode, this);
	}

	Element::~Element()
	{
		OE_ASSERT_MSG(m_Parent == nullptr, "Element is still attached to the tree");

		YGNodeFreeRecursive(m_YogaNode);
	}

	void Element::addElement(Element* child, uint32_t index)
	{
		// this function sets the correct owner in yoga land
		YGNodeInsertChild(m_YogaNode, child->m_YogaNode, index);

		if (child->m_Parent != nullptr) {
			if (child->m_Parent == this) {
				// TODO: handle reposition
				return;
			}
			else {
				child->m_Parent->removeElement(child);
			}
		}

		child->m_Parent = this;
		child->m_Depth = m_Depth + 1;
		m_Childrens.insert(m_Childrens.begin() + index, child);
	}

	void Element::removeElement(Element* child)
	{
		OE_ASSERT(child->m_Parent == this); // TODO: maybe do a runtime check?

		child->m_Parent = nullptr;
		child->m_Depth = m_Depth + 1;
		m_Childrens.erase(std::find(m_Childrens.begin(), m_Childrens.end(), child));

		// TODO: mark dirty root (and depth) on childrens of child
	}

	void Element::paintContent(Painter* painter)
	{
		if (!m_ComputedStyle)
			return;

		auto color = m_ComputedStyle->backgroundColor.value.color;
		if (color.a > 0.01) {
			//painter->drawRectangle(m_BoundingBox, color);

			// TODO: % unit
			RoundedRectParams roundedParams;
			roundedParams.cornerRadii[0].x = roundedParams.cornerRadii[0].y = m_ComputedStyle->borderTopLeftRadius.value.length.number;
			roundedParams.cornerRadii[1].x = roundedParams.cornerRadii[1].y = m_ComputedStyle->borderTopRightRadius.value.length.number;
			roundedParams.cornerRadii[2].x = roundedParams.cornerRadii[2].y = m_ComputedStyle->borderBottomLeftRadius.value.length.number;
			roundedParams.cornerRadii[3].x = roundedParams.cornerRadii[3].y = m_ComputedStyle->borderBottomRightRadius.value.length.number;
			painter->drawRoundedRectangle(m_BoundingBox, color, roundedParams);
		}
	}

	Math::Vec2f Element::measureContent(float width, MeasureMode widthMode, float height, MeasureMode heightMode)
	{
		OE_ASSERT_MSG(false, "measureContent is not overrided");
		return { 0, 0 };
	}

	void Element::executeDefault(EventBase* evt)
	{
		switch (evt->type) {
		case EventTypeID::MOUSE_ENTER:
			setPseudoStates(StylePseudoStates::HOVER);
			break;
		case EventTypeID::MOUSE_LEAVE:
			removePseudoStates(StylePseudoStates::HOVER);
			break;
		}
	}

	void Element::setID(const std::string& id)
	{
		m_ID = { StyleIdentifierType::ID, id };
		m_ID.computeHash();
	}

	void Element::setTag(const std::string& tag)
	{
		m_Tag = { StyleIdentifierType::TAG, tag };
		m_Tag.computeHash();
	}

	void Element::addClass(const std::string& klass)
	{
		StyleIdentifier si = { StyleIdentifierType::CLASS, klass };
		si.computeHash();

		for (auto it = m_Classes.begin(); it != m_Classes.end(); it++) {
			if ((*it).text_hash == si.text_hash)
				return; // already present
		}

		m_Classes.emplace_back(si);
	}

	void Element::addStyleSheet(StyleSheet* stylesheet)
	{
		m_StyleSheets.emplace_back(stylesheet);
	}

	void Element::setStyleProperty(const StyleProperty& property)
	{
		for (StyleProperty& p : m_InlineRules.properties) {
			if (p.id == property.id) {
				// replace
				p = property;
				return;
			}
		}
		// not found, insert
		m_InlineRules.properties.emplace_back(property);
	}

	void Element::setPseudoStates(const StylePseudoStates states)
	{
		m_PseudoStates |= states;
	}

	void Element::removePseudoStates(const StylePseudoStates states)
	{
		m_PseudoStates &= ~states;
	}

	Element* Element::getParent() const
	{
		return m_Parent;
	}

	const std::vector<Element*>& Element::getChildrens() const
	{
		return m_Childrens;
	}

	const std::vector<StyleSheet*>& Element::getStyleSheets() const
	{
		return m_StyleSheets;
	}

	Math::Rectf Element::getBoundingBox() const
	{
		return m_BoundingBox;
	}

	bool Element::isVisible() const
	{
		return m_ComputedStyle && m_ComputedStyle->display.value.display == Display::FLEX;
	}

	int Element::getDepth() const
	{
		// TODO: cache or something
		int depth = 0;
		const Element* e = this;
		while (e) {
			depth++;
			e = e->getParent();
		}
		return depth;
	}

	MeasureMode YogaMeasureModeToMeasureMode(YGMeasureMode mode) {
		switch (mode)
		{
		default:
		case YGMeasureModeUndefined: return MeasureMode::UNDEFINED;
		case YGMeasureModeExactly: return MeasureMode::EXACTLY;
		case YGMeasureModeAtMost: return MeasureMode::AT_MOST;
		}
	}

	YGSize YogaMeasureCallback(YGNode* yogaNode, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
	{
		Element* element_ptr = static_cast<Element*>(yogaNode->getContext());
		auto size = element_ptr->measureContent(width, YogaMeasureModeToMeasureMode(widthMode), height, YogaMeasureModeToMeasureMode(heightMode));
		return { size.x, size.y };
	}

	void Element::enableMeasurement()
	{
		YGNodeSetMeasureFunc(m_YogaNode, YogaMeasureCallback);
	}

	void Element::setAsTextType()
	{
		YGNodeSetNodeType(m_YogaNode, YGNodeType::YGNodeTypeText);
	}

} }
