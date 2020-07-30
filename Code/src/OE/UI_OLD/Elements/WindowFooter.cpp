#include "OE/UI/Elements/WindowFooter.hpp"

#include "OE/UI/Elements/Resizer.hpp"

namespace OrbitEngine {	namespace UI {
	WindowFooter::WindowFooter()
	{
		m_CornerResizer = new Resizer();
		m_CornerResizer->setAnchor(Anchor::BOTTOM_RIGHT);
		m_CornerResizer->setMargins(Math::Vec4f(0, 0, 0, 0));
		m_CornerResizer->setPivot(Math::Vec2f(1, 1));
		m_CornerResizer->setSize(Math::Vec2f(20,20));
		m_CornerResizer->setCursor(Application::Cursor::SIZENWSE);
		add(m_CornerResizer);
	}
} }