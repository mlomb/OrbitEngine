#ifndef UI_STYLE_TREE_UPDATER_HPP
#define UI_STYLE_TREE_UPDATER_HPP

#include "OE/UI/TreeUpdater.hpp"

namespace OrbitEngine { namespace UI {

	class StyleTreeUpdater : public TreeUpdater {
	public:
		StyleTreeUpdater(Surface* surface);
		virtual ~StyleTreeUpdater();

		void update() override;
		void onVersionChange(Element* element, VersionChangeType changes) override;

	private:

	};

} }

#endif