#include "OE/UI/Elements/WindowHeader.hpp"

#include "OE/UI/Elements/Element.hpp"
#include "OE/UI/Elements/Text.hpp"

#include "OE/Graphics/2D/PathRenderer2D.hpp"

namespace OrbitEngine {	namespace UI {
	WindowHeader::WindowHeader()
	{
		m_Title = new Text();
		m_Title->setAnchor(Anchor::STRETCH_TOP_HORIZONTAL);
		m_Title->setSize(Math::Vec2f(0, 20));
		m_Title->setPivot(Math::Vec2f(0, 0));
		m_Title->setText(L"Title");
		add(m_Title);
	}

	void WindowHeader::onRender(Graphics::PathRenderer2D& r2d)
	{
		Math::Vec2f start = getRenderPosition() + Math::Vec2f(1, 1);
		Math::Vec2f size = Math::Vec2f(getRenderSize().x - 2, getRenderSize().y);
		
		/* Header */
		r2d.beginPath();
		r2d.roundedRect(start, size, Math::Vec4f(2,2,0,0));
		r2d.closePath();
		r2d.fillColor(Math::Vec4f(0.4f, 0.4f, 0.4f, 1.0f));
		r2d.fill();
	}
} }