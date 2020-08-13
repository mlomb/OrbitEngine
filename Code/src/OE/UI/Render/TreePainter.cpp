#include "OE/UI/Render/TreePainter.hpp"

#include "OE/UI/Render/Painter.hpp"
#include "OE/UI/Surface.hpp"
#include "OE/UI/Element.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace UI {

	TreePainter::TreePainter(Surface* surface)
		: TreeProcessor(surface)
	{
		m_Painter = new Painter();
	}

	TreePainter::~TreePainter()
	{
		delete m_Painter;
	}

	void TreePainter::process()
	{
		auto size = m_Surface->getSize();
		auto transform = Math::Mat4::Orthographic(0, size.x, size.y, 0, 0, 1);

		m_Painter->setProjection(transform);
		m_Painter->begin();
		rec(m_Surface->getRoot());
		m_Painter->end();
	}

	// this will be removed
	void TreePainter::rec(Element* el)
	{
		el->paintContent(m_Painter);

		for (Element* e : el->getChildrens())
			rec(e);
	}

} }
