#include "OE/UI/Render/PaintTreeUpdater.hpp"

#include "OE/UI/Render/Painter.hpp"
#include "OE/UI/Surface.hpp"
#include "OE/UI/Element.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace UI {

	PaintTreeUpdater::PaintTreeUpdater(Surface* surface)
		: TreeUpdater(surface)
	{
		m_Painter = new Painter();
	}

	PaintTreeUpdater::~PaintTreeUpdater()
	{
		delete m_Painter;
	}

	void PaintTreeUpdater::update()
	{
		auto size = m_Surface->getSize();
		auto transform = Math::Mat4::Orthographic(0, size.x, size.y, 0, 0, 1);

		m_Painter->setProjection(transform);
		m_Painter->begin();
		rec(m_Surface->getRoot());
		m_Painter->end();
	}

	void PaintTreeUpdater::onVersionChange(Element* element, VersionChangeType changes)
	{
	}

	// this will be removed
	void PaintTreeUpdater::rec(Element* el)
	{
		el->paintContent(m_Painter);

		for (Element* e : el->getChildrens())
			rec(e);
	}

} }
