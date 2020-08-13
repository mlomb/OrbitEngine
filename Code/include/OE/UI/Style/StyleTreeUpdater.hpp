#ifndef UI_STYLE_TREE_UPDATER_HPP
#define UI_STYLE_TREE_UPDATER_HPP

#include <vector>

#include "OE/UI/TreeProcessor.hpp"

#include "OE/UI/Style/StyleSelectorMatcher.hpp"

namespace OrbitEngine { namespace UI {

	class StyleSheet;
	class StyleSelector;

	class StyleTreeUpdater : public TreeProcessor {
	public:
		StyleTreeUpdater(Surface* surface);
		virtual ~StyleTreeUpdater();

		void process() override;

	private:
		std::vector<StyleSheet*> m_SheetsStack;
		std::vector<SelectorMatch> m_MatchedSelectors;

		void applyStyles(Element* element);
		void processMatchedSelectors(Element* element);
		void syncWithLayout(Element* element);
	};

} }

#endif