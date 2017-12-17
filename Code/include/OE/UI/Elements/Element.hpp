#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include <vector>

#include "OE/UI/Event.hpp"
#include "OE/UI/Layouts/LayoutManager.hpp"
#include "OE/UI/Layouts/LayoutElement.hpp"

#include "OE/Application/EventHandler.hpp"
#include "OE/Application/InputEvents.hpp"

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec4.hpp"

namespace OrbitEngine { namespace Graphics {
	class PathRenderer2D;
} }

namespace OrbitEngine { namespace UI {
	class Canvas;
	class Fitter;
	class Layout;

	// Predefined anchors
	enum class Anchor {
		TOP_LEFT,              TOP_CENTER,              TOP_RIGHT,              STRETCH_TOP_HORIZONTAL,
		MIDDLE_LEFT,           MIDDLE_CENTER,           MIDDLE_RIGHT,           STRETCH_MIDDLE_HORIZONTAL,
		BOTTOM_LEFT,		   BOTTOM_CENTER,           BOTTOM_RIGHT,           STRETCH_BOTTOM_HORIZONTAL,
		STRETCH_LEFT_VERTICAL, STRETCH_CENTER_VERTICAL, STRETCH_RIGHT_VERTICAL, STRETCH_BOTH
	};

	enum class RectAlignment {
		UPPER_LEFT, UPPER_CENTER, UPPER_RIGHT,
		MIDDLE_LEFT, MIDDLE_CENTER, MIDDLE_RIGHT,
		LOWER_LEFT, LOWER_CENTER, LOWER_RIGHT
	};

	enum class Alignment {
		UPPER, MIDDLE, LOWER
	};

	enum AccessSpecifier {
		COLLAPSED = 0,
		VISIBLE,
		HIDDEN,
	};
	
	struct Offsets {
		union {
			Math::Vec4f offsets;
			struct {
				float positionX, width;
				float positionY, height;
			};
			struct {
				float left, right;
				float top, bottom;
			};
		};

		Offsets() : offsets(Math::Vec4f()) { }
	};

	class Element : public LayoutElement {
	public:
		virtual ~Element() {};

		void add(Element* element);
		void removeAll();
		void dispose();

		void render(Graphics::PathRenderer2D& r2d);
		void update(float deltaTime);
		void lateUpdate();

		void invalidate();

		void setAnchor(Anchor anchor);
		void setLayout(Layout* layout);
		void setFitter(Fitter* fitter) { m_Fitter = fitter; invalidate(); };

		void setOffsetsAlongAxis(Math::Vec2f offsets, Axis axis) { m_Offsets.offsets[axis * 2] = offsets.x; m_Offsets.offsets[axis * 2 + 1] = offsets.y;  invalidate(); };
		void setOffsets(Offsets offsets) { m_Offsets = offsets; invalidate(); }
		void setPositionAlongAxis(float position, Axis axis) { m_Offsets.offsets[axis * 2] = position; invalidate(); };
		void setSizeAlongAxis(float size, Axis axis) { m_Offsets.offsets[axis * 2 + 1] = size;  invalidate(); };
		void setPosition(Math::Vec2f position) { setPositionAlongAxis(position.x, Axis::HORIZONTAL); setPositionAlongAxis(position.y, Axis::VERTICAL); };
		void setSize(Math::Vec2f size) { setSizeAlongAxis(size.x, Axis::HORIZONTAL); setSizeAlongAxis(size.y, Axis::VERTICAL); };
		void setPositionAndSizeOnAxis(Math::Vec2f data, Axis axis) { setOffsetsAlongAxis(data, axis); };
		void setMarginsAlongAxis(Math::Vec2f margins, Axis axis) { setOffsetsAlongAxis(margins, axis); };
		void setMargins(Math::Vec4f margins) { setMarginsAlongAxis(margins.xz(), Axis::HORIZONTAL); setMarginsAlongAxis(margins.yw(), Axis::VERTICAL); }; // Left, Top, Right, Bottom
		void setAnchors(Math::Vec4f anchors) { setAnchorMin(anchors.xy); setAnchorMax(anchors.zw); };
		void setAnchorMin(Math::Vec2f anchorMin) { m_AnchorMin = anchorMin; invalidate(); };
		void setAnchorMax(Math::Vec2f anchorMax) { m_AnchorMax = anchorMax; invalidate(); };
		void setPivot(Math::Vec2f pivot) { m_Pivot = pivot; invalidate(); };
		void setCursor(Application::Cursor cursor) { m_Cursor = cursor; }
		void setClip(bool clip) { m_ClipToBounds = clip; }

		Math::Vec2f getAnchorMin() { return m_AnchorMin; };
		Math::Vec2f getAnchorMax() { return m_AnchorMax; };
		Math::Vec2f getPivot() { return m_Pivot; };
		Math::Vec2f getPosition() { return m_Rect.xy; };
		Math::Vec2f getSize() { return m_Rect.zw; };
		Math::Vec4f getRect() { return m_Rect; };
		Math::Vec4f getRenderRect() { return m_RenderRect; }
		Math::Vec2f getRenderPosition() { return m_RenderRect.xy; }
		Math::Vec2f getRenderSize() { return m_RenderRect.zw; }
		Application::Cursor getCursor() { return m_Cursor; }
		Element* getParent() { return m_Parent; }
		std::vector<Element*>& getChildrens() { return m_Childrens; }
		virtual Application::Cursor getCurrentCursor() { return m_Cursor; }
		virtual Layout* getLayout() { return m_Layout; }

		Element* getComponentAt(Math::Vec2i relPosition);
		Application::Cursor getFinalCursorAtMe();
		void bringToFront(Element* element);

	public:
		Application::EventHandler<MouseBaseEvent> onClick;
		Application::EventHandler<MouseBaseEvent> onDblClick;
		Application::EventHandler<MouseBaseEvent> onMouseEnter;
		Application::EventHandler<MouseBaseEvent> onMouseLeave;
		Application::EventHandler<MouseBaseEvent> onMouseUp;
		Application::EventHandler<MouseBaseEvent> onMouseDown;
		Application::EventHandler<MouseMoveEvent> onMouseMove;

		Application::EventHandler<KeyBaseEvent> onKeyUp;
		Application::EventHandler<KeyBaseEvent> onKeyDown;
		Application::EventHandler<KeyBaseEvent> onKeyPress;

		Application::EventHandler<Event> onFocusGain;
		Application::EventHandler<Event> onFocusLost;

		Application::EventHandler<WheelEvent> onWheelDelta;

	private:
		friend class Canvas;
		friend class LayoutManager;

		void calcAlongAxis(Axis axis);

		Math::Vec2f m_Pivot; // Pivot W, H
		Math::Vec2f m_AnchorMin, m_AnchorMax; // MinX, MinY, MaxX, MaxY
		Offsets m_Offsets; // X/Left, Width/Right, Y/Top, Height/Bottom
		Math::Vec4f m_Rect; // X, Y, W, H (anchored)
		Math::Vec4f m_RenderRect; // absolute screen position

		Application::Cursor m_Cursor;
		bool m_Valid, m_ClipToBounds;
		std::vector<Element*> m_Childrens;

		Element* m_Parent;
		Layout* m_Layout;
		Fitter* m_Fitter;
	protected:
		Element();

		virtual void onRender(Graphics::PathRenderer2D& r2d) {};
		virtual void onUpdate(float deltaTime) {};
		virtual void onLateUpdate() {};
		virtual void onValidate() {};

		virtual Canvas* getCanvas();
	};
} }

#endif