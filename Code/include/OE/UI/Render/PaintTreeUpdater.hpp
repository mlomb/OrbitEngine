#ifndef UI_PAINT_TREE_UPDATER_HPP
#define UI_PAINT_TREE_UPDATER_HPP

#include "OE/UI/TreeUpdater.hpp"

namespace OrbitEngine { namespace UI {

	class Painter;

	class PaintTreeUpdater : public TreeUpdater {
	public:
		PaintTreeUpdater(Surface* surface);
		virtual ~PaintTreeUpdater();

		void update() override;
		void onVersionChange(Element* element, VersionChangeType changes) override;

	private:
		Painter* m_Painter;

		void rec(Element* el);
	};

} }

#endif