#include "OE/UI/Elements/Window.hpp"

#include "OE/UI/Elements/Resizer.hpp"
#include "OE/Graphics/2D/PathRenderer2D.hpp"

namespace OrbitEngine { namespace UI {
	Window::Window()
		: m_MinSize(Math::Vec2f(100, 100))
	{
		setPivot(Math::Vec2f());
		
		m_Header = new WindowHeader();
		m_Header->setAnchor(Anchor::STRETCH_TOP_HORIZONTAL);
		m_Header->setSize(Math::Vec2f(0, 20));
		m_Header->setPivot(Math::Vec2f(0, 0));
		m_Header->onDrag.AddListener(this, &Window::headerDrag);
		m_Header->setDraggingCursor(Application::Cursor::SIZEALL);
		add(m_Header);
		
		m_Footer = new WindowFooter();
		m_Footer->setAnchor(Anchor::STRETCH_BOTTOM_HORIZONTAL);
		m_Footer->setSizeAlongAxis(20, Axis::VERTICAL);
		m_Footer->setPivot(Math::Vec2f(0, 1));
		m_Footer->getCornerResizer()->onDrag.AddListener(this, &Window::footerCornerDrag);
		add(m_Footer);

		onFocusGain.AddListener(this, &Window::focusGain);
	}

	void Window::setContent(Element* element, bool borders)
	{
		m_Content = element;

		m_Content->setAnchor(Anchor::STRETCH_BOTH);
		if(borders)
			m_Content->setMargins(Math::Vec4f(3, 21, 3, 21));
		else
			m_Content->setMargins(Math::Vec4f(1, 20, 1, 20));
		m_Content->setPivot(Math::Vec2f(0, 0));
		add(m_Content);
	}

	void Window::headerDrag(const DragEvent& dragEvent)
	{
		Math::Vec2f newPosition = dragEvent.baseEvent.position - dragEvent.relativeDragStart;
		newPosition = Math::Vec2f::Max(newPosition, Math::Vec2f());
		if(getParent())
			newPosition = Math::Vec2f::Min(newPosition, getParent()->getRenderSize() - getRenderSize());
		setPosition(newPosition);
	}

	void Window::footerCornerDrag(const DragEvent& dragEvent)
	{
		Math::Vec2f newSize = dragEvent.dragStart + dragEvent.totalDelta - getRenderPosition() + (dragEvent.target->getRenderSize() - dragEvent.relativeDragStart);
		newSize = Math::Vec2f::Max(newSize, m_MinSize);
		setSize(newSize);
	}

	void Window::focusGain(const Event& event)
	{
		if(getParent())
			getParent()->bringToFront(this);
	}

	void Window::onRender(Graphics::PathRenderer2D& r2d)
	{
		float cornerRadius = 3;
		float dropShadowSize = 10;
		
		/* Drop Shadow */
		/*
		r2d.beginPath();
		r2d.rect(getRenderPosition() - Vec2(dropShadowSize, dropShadowSize), getRenderSize() + Vec2(dropShadowSize * 2, dropShadowSize * 2));
		r2d.roundedRect(getRenderPosition(), getRenderSize(), cornerRadius);
		r2d.closePath();
		r2d.fillStyle(r2d.BoxGradient(getRenderPosition(), getRenderSize(), cornerRadius * 2, dropShadowSize * 2, Vec4(0, 0, 0, 0.5f), Vec4()));
		r2d.fill();
		*/

		/* Window */
		r2d.beginPath();
		r2d.roundedRect(getRenderPosition(), getRenderSize(), cornerRadius);
		r2d.closePath();
		r2d.fillColor(Math::Vec4f(0.176f, 0.176f, 0.176f, 0.9f));
		r2d.fill();
	}
} }