#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include <vector>

#include "OE/UI/Style/StyleEnums.hpp"

#include "OE/Math/Rect.hpp"

// forward def to avoid include
class YGNode;

namespace OrbitEngine { namespace UI {

	class Painter;
	class StyleSheet;
	class StyleComputed;
	class EventBase;
	class Surface;
	class EventsController;

	enum class MeasureMode {
		UNDEFINED,
		EXACTLY,
		AT_MOST
	};

	/// The base of all UI controls
	class Element {
	public:
		Element();
		virtual ~Element();

		void addElement(Element* child, uint32_t index);
		void removeElement(Element* child);
		void setID(const std::string& id);
		void addClass(const std::string& klass);
		void addStyleSheet(StyleSheet* stylesheet);
		void setStyleProperty(const StyleProperty& property);
		void setPseudoStates(const StylePseudoStates states);
		void removePseudoStates(const StylePseudoStates states);

		Element* getParent() const;
		const std::vector<Element*>& getChildrens() const;
		const std::vector<StyleSheet*>& getStyleSheets() const;
		Math::Rectf getBoundingRect() const;
		Math::Rectf getContentRect() const;
		bool isVisible() const;
		int getDepth() const;
		Surface* getSurface() const;
		EventsController* getEventsController() const;

		virtual void paintContent(Painter* painter);
		virtual Math::Vec2f measureContent(float width, MeasureMode widthMode, float height, MeasureMode heightMode);
		virtual void executeDefault(EventBase* evt);
		
	protected:
		StyleComputed* m_ComputedStyle;

		void setTag(const std::string& tag);

		void enableMeasurement();
		void setAsTextType();

	private:
		friend class Surface;
		friend class StyleTreeUpdater;
		friend class LayoutTreeUpdater;
		friend class PaintTreeUpdater;

		void updateHierarchy();

		Element* m_Parent;
		std::vector<Element*> m_Childrens;
		int m_Depth;

		friend class StyleSelectorMatcher;
		StyleIdentifier m_ID, m_Tag;
		std::vector<StyleIdentifier> m_Classes;
		StylePseudoStates m_PseudoStates;
		std::vector<StyleSheet*> m_StyleSheets;
		StyleRule m_InlineRules;

		YGNode* m_YogaNode;
		Math::Rectf m_LayoutRect;
		Math::Rectf m_BoundingRect;

		Surface* m_Surface;
	};
} }

#endif