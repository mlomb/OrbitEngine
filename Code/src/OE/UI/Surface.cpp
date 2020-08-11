#include "OE/UI/Surface.hpp"

#include "OE/UI/Element.hpp"
#include "OE/UI/Style/StyleTreeUpdater.hpp"
#include "OE/UI/Layout/LayoutTreeUpdater.hpp"
#include "OE/UI/Render/PaintTreeUpdater.hpp"

namespace OrbitEngine { namespace UI {
	
	Surface::Surface()
		: m_Size(100, 100)
	{
		m_StyleUpdater = new StyleTreeUpdater(this);
		m_LayoutUpdater = new LayoutTreeUpdater(this);
		m_PaintUpdater = new PaintTreeUpdater(this);
		m_Root = new Element();
		m_Root->m_Surface = this;
	}

	Surface::~Surface()
	{
		delete m_StyleUpdater;
		delete m_LayoutUpdater;
		delete m_PaintUpdater;
		delete m_Root;
	}

	void Surface::updateTree()
	{
		m_StyleUpdater->update();
		m_LayoutUpdater->update();
		m_PaintUpdater->update();
	}

	Element* Surface::getRoot() const
	{
		return m_Root;
	}

	void Surface::setSize(const Math::Vec2i& size)
	{
		m_Size = size;
	}

	Math::Vec2i Surface::getSize() const
	{
		return m_Size;
	}
} }
