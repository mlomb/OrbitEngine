#include "OE/UI/Elements/Element.hpp"

#include "OE/Graphics/2D/PathRenderer2D.hpp"
#include "OE/UI/Fitters/Fitter.hpp"
#include "OE/UI/Elements/Canvas.hpp"
#include "OE/UI/Layouts/Layout.hpp"

namespace OrbitEngine { namespace UI {
	Element::Element()
		: m_Parent(nullptr),
		m_Layout(nullptr),
		m_Fitter(nullptr),
		m_Valid(false),
		m_Cursor(Application::Cursor::DEFAULT),
		m_Pivot(0.5f, 0.5f),
		m_ClipToBounds(false)
	{
		setAnchor(Anchor::STRETCH_BOTH);
		invalidate();
	}

	void Element::add(Element* element)
	{
		m_Childrens.push_back(element);
		element->m_Parent = this;
	}

	void Element::removeAll()
	{
		for (auto e : m_Childrens) {
			e->dispose();
			delete e;
		}
		m_Childrens.clear();
		// Do something with the parents?
	}

	void Element::dispose()
	{
		removeAll();

		if (m_Layout)
			delete m_Layout;
		if (m_Fitter)
			delete m_Fitter;
	}

	void Element::render(Graphics::PathRenderer2D& r2d)
	{
		m_RenderRect = getRect();
		if (m_Parent)
			m_RenderRect.xy += m_Parent->getRenderPosition();
		
		// - Debugging
		bool renderDebug = false;

		if (renderDebug) {
			float debugAlpha = 0.1f;
			bool isValidSize = m_RenderRect.z != 0 && m_RenderRect.w != 0;

			if (isValidSize)
				r2d.fillColor(Math::Vec4f(0, 1, 0, debugAlpha));
			else
				r2d.fillColor(Math::Vec4f(1, 0, 0, debugAlpha));
			r2d.beginPath();
			r2d.circle(m_RenderRect.xy, 3);
			r2d.closePath();
			r2d.fill();

			if (isValidSize) {
				if (m_RenderRect.z < 0 || m_RenderRect.w < 0)
					r2d.fillColor(Math::Vec4f(1, 0, 0, debugAlpha * 2));
				else if(getCanvas()->getCapturedElement() == this)
					r2d.fillColor(Math::Vec4f(0, 1, 0, debugAlpha));
				else
					r2d.fillColor(Math::Vec4f(0, 0, 1, debugAlpha));
				r2d.beginPath();
				r2d.rect(m_RenderRect.xy, m_RenderRect.zw);
				r2d.closePath();
				r2d.fill();
			}
		}
		// - Debugging

		m_ClipToBounds = false;
		if (m_ClipToBounds) {
			Math::Scissor scissor(Math::Vec4i(m_RenderRect.xy, m_RenderRect.zw));
			r2d.getScissorStack()->pushStack(scissor);
		}

		onRender(r2d);

		for (std::vector<Element*>::reverse_iterator rit = m_Childrens.rbegin(); rit != m_Childrens.rend(); ++rit)
			(*rit)->render(r2d);

		if (m_ClipToBounds)
			r2d.getScissorStack()->popStack();
	}

	void Element::update(float deltaTime)
	{
		onUpdate(deltaTime);

		for (Element* e : m_Childrens)
			e->update(deltaTime);
	}

	void Element::lateUpdate()
	{
		onLateUpdate();

		for (Element* e : m_Childrens)
			e->lateUpdate();
	}

	void Element::invalidate()
	{
		if (!m_Valid)
			return;

		m_Valid = false;

		if (m_Parent)
			m_Parent->invalidate();
	}

	void Element::calcAlongAxis(Axis axis)
	{
		if (m_Fitter)
			m_Fitter->fit(this, axis);

		bool strech = !Math::approximatelyEqual(m_AnchorMin[axis], m_AnchorMax[axis], 0.01f);

		float pivot = m_Pivot[axis];
		float parentSize = m_Parent ? m_Parent->getSize()[axis] : 0.0f;
		float anchoredPosition = parentSize * m_AnchorMin[axis];

		Math::Vec2f axisResult;

		if (strech) {
			axisResult.x += m_Offsets.offsets[axis * 2];
			axisResult.y = (m_AnchorMax[axis] - m_AnchorMin[axis]) * parentSize;
			axisResult.y -= (m_Offsets.offsets[axis * 2] + m_Offsets.offsets[axis * 2 + 1]);
		}
		else {
			float position = m_Offsets.offsets[axis * 2];
			float size = m_Offsets.offsets[axis * 2 + 1];

			axisResult.x = position;// +size * pivot;
			axisResult.y = size;
		}

		axisResult.x += anchoredPosition;
		axisResult.x -= m_Pivot[axis] * axisResult.y;

		m_Rect[axis] = axisResult.x;
		m_Rect[axis + 2] = axisResult.y;
	}

	Application::Cursor Element::getFinalCursorAtMe()
	{
		Application::Cursor currentCursor = getCurrentCursor();
		if (currentCursor != Application::Cursor::DEFAULT)
			return currentCursor;

		if (m_Parent) {
			Application::Cursor parentCursor = m_Parent->getFinalCursorAtMe();
			if (parentCursor != Application::Cursor::DEFAULT)
				return parentCursor;
		}

		return Application::Cursor::DEFAULT;
	}

	void Element::bringToFront(Element* element)
	{
		auto it = std::find(m_Childrens.begin(), m_Childrens.end(), element);
		if (it != m_Childrens.end()) {
			m_Childrens.erase(it);
			m_Childrens.insert(m_Childrens.begin(), element);
		}
		if (m_Parent)
			m_Parent->bringToFront(this);
	}

	void Element::setLayout(Layout* layout) {
		if (m_Layout != layout) {
			m_Layout = layout;
			invalidate();
		}
	}

	Canvas* Element::getCanvas()
	{
		if (m_Parent)
			return m_Parent->getCanvas();
		return nullptr;
	}

	Element* Element::getComponentAt(Math::Vec2i relPosition)
	{
		if (getRect().isInside(relPosition)) {
			Math::Vec2f newRelPosition = relPosition - getPosition();
			for (Element* e : m_Childrens) {
				Element* atComponent = e->getComponentAt(newRelPosition);
				if (atComponent)
					return atComponent;
			}
			return this;
		}
		return nullptr;
	}

	void Element::setAnchor(Anchor anchor)
	{
		switch (anchor)
		{
		default:
		case Anchor::TOP_LEFT:      setAnchors(Math::Vec4f(0, 0, 0, 0)); break;
		case Anchor::TOP_CENTER:    setAnchors(Math::Vec4f(0.5f, 0, 0.5f, 0)); break;
		case Anchor::TOP_RIGHT:     setAnchors(Math::Vec4f(1, 0, 1, 0)); break;
		case Anchor::MIDDLE_LEFT:   setAnchors(Math::Vec4f(0, 0.5f, 0, 0.5f)); break;
		case Anchor::MIDDLE_CENTER: setAnchors(Math::Vec4f(0.5f, 0.5f, 0.5f, 0.5f)); break;
		case Anchor::MIDDLE_RIGHT:  setAnchors(Math::Vec4f(1, 0.5f, 1, 0.5f)); break;
		case Anchor::BOTTOM_LEFT:   setAnchors(Math::Vec4f(0, 1, 0, 1)); break;
		case Anchor::BOTTOM_CENTER: setAnchors(Math::Vec4f(0.5f, 1, 0.5f, 1)); break;
		case Anchor::BOTTOM_RIGHT:  setAnchors(Math::Vec4f(1, 1, 1, 1)); break;

		case Anchor::STRETCH_LEFT_VERTICAL:   setAnchors(Math::Vec4f(0, 0, 0, 1)); break;
		case Anchor::STRETCH_CENTER_VERTICAL: setAnchors(Math::Vec4f(0.5f, 0, 0.5f, 1)); break;
		case Anchor::STRETCH_RIGHT_VERTICAL:  setAnchors(Math::Vec4f(1, 0, 1, 1)); break;

		case Anchor::STRETCH_TOP_HORIZONTAL:    setAnchors(Math::Vec4f(0, 0, 1, 0)); break;
		case Anchor::STRETCH_MIDDLE_HORIZONTAL: setAnchors(Math::Vec4f(0, 0.5f, 1, 0.5f)); break;
		case Anchor::STRETCH_BOTTOM_HORIZONTAL: setAnchors(Math::Vec4f(0, 1, 1, 1)); break;

		case Anchor::STRETCH_BOTH: setAnchors(Math::Vec4f(0, 0, 1, 1)); break;
		}
	}
} }