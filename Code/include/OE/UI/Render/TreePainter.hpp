#ifndef UI_TREE_PAINTER_HPP
#define UI_TREE_PAINTER_HPP

#include "OE/UI/TreeProcessor.hpp"

namespace OrbitEngine { namespace UI {

	class Painter;

	class TreePainter : public TreeProcessor {
	public:
		TreePainter(Surface* surface);
		virtual ~TreePainter();

		void process() override;

	private:
		Painter* m_Painter;

		void rec(Element* el);
	};

} }

#endif