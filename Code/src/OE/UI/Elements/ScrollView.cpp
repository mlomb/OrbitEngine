#include "OE/UI/Elements/ScrollView.hpp"

#include "OE/Application/WindowImpl.hpp"
#include "OE/Application/InputManager.hpp"
#include "OE/UI/Elements/Canvas.hpp"

namespace OrbitEngine {	namespace UI {
	ScrollView::ScrollView()
	{
		onDrag.AddListener(this, &ScrollView::drag);
		onWheelDelta.AddListener(this, &ScrollView::wheelDelta);
	}

	void ScrollView::drag(const DragEvent& dragEvent)
	{
		m_ScrollPosition -= dragEvent.baseEvent.delta;
		invalidate();
	}

	void ScrollView::wheelDelta(const WheelEvent& wheelEvent)
	{
		bool shiftDown = getCanvas()->getWindow()->getInputManager()->isKeyDown(Application::Key::SHIFT);
		m_ScrollPosition[shiftDown ? 0 : 1] += wheelEvent.direction * 7.0f;
		invalidate();
	}

	void ScrollView::onValidate()
	{
		updateScrolling();
	}

	void ScrollView::updateScrolling() {
		m_ScrollPosition = Math::Vec2f::Max(m_ScrollPosition, Math::Vec2f(-1, -1) * m_Content->getSize() + m_Viewport->getSize());
		m_ScrollPosition = Math::Vec2f::Min(m_ScrollPosition, Math::Vec2f());
		Math::Vec2f position = m_ScrollPosition;

		Math::Vec2f sizeDiff = m_Content->getSize() - m_Viewport->getSize();
		if (sizeDiff.x < 0) position.x += m_Viewport->getSize().x / 2.0f - m_Content->getSize().x / 2.0f;
		if (sizeDiff.y < 0) position.y += m_Viewport->getSize().y / 2.0f - m_Content->getSize().y / 2.0f;

		m_Content->setPosition(position);
	}

	void ScrollView::setViewport(Element* element)
	{
		m_Viewport = element;
		m_Viewport->setLayout(nullptr);
		m_Viewport->setClip(true);
		add(m_Viewport);
	}

	void ScrollView::setContent(Element* element)
	{
		m_Content = element;
		m_Content->setAnchor(Anchor::TOP_LEFT);
		m_Content->setPivot(Math::Vec2f(0, 0));
		m_Viewport->add(m_Content);
	}
} }