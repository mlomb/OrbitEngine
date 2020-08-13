#ifndef UI_LAYOUT_TREE_UPDATER_HPP
#define UI_LAYOUT_TREE_UPDATER_HPP

#include "OE/UI/TreeProcessor.hpp"

namespace OrbitEngine { namespace UI {

	class LayoutTreeUpdater : public TreeProcessor {
	public:
		LayoutTreeUpdater(Surface* surface);
		virtual ~LayoutTreeUpdater();

		void process() override;

	private:
		void updateLayout(Element* element);
	};

} }

#endif