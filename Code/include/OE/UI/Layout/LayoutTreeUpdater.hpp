#ifndef UI_LAYOUT_TREE_UPDATER_HPP
#define UI_LAYOUT_TREE_UPDATER_HPP

#include "OE/UI/TreeUpdater.hpp"

namespace OrbitEngine { namespace UI {

	class LayoutTreeUpdater : public TreeUpdater {
	public:
		LayoutTreeUpdater(Surface* surface);
		virtual ~LayoutTreeUpdater();

		void update() override;
		void onVersionChange(Element* element, VersionChangeType changes) override;

	private:
		void updateLayout(Element* element);
	};

} }

#endif