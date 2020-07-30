#ifndef UI_CANVAS_HPP
#define UI_CANVAS_HPP

#include "OE/UI/Elements/Element.hpp"
#include "OE/Application/InputEvents.hpp"

#define OE_UI_EVENT_PROPAGATE(componentName, eventName, eventObjName) \
{ \
	Element* __component = (componentName); \
	if(m_CapturedElement) \
		__component = m_CapturedElement; \
	if(__component != nullptr) { \
		eventObjName.target = __component; \
		eventObjName.propagating = true; \
		\
		std::vector<Element*> __path; \
		\
		Element* __it = __component; \
		do { \
			__path.push_back(__it); \
			__it = __it->getParent(); \
		} while (__it); \
		\
		for (std::vector<Element*>::reverse_iterator __itr = __path.rbegin(); __itr != __path.rend(); ++__itr) \
			(*__itr)->eventName.FireEvent(eventObjName); \
	} \
} \
\

namespace OrbitEngine {	namespace Application { namespace priv {
	class WindowImpl;
} } }

namespace OrbitEngine {	namespace UI {
	class Canvas : public Element {
	public:
		Canvas();
		Canvas(Application::priv::WindowImpl* view);

		void captureElement(Element* element);

		Element* getCapturedElement() { return m_CapturedElement; }
		Element* getFocusedElement() { return m_FocusedElement; }

		Application::priv::WindowImpl* getWindow() { return m_Window; };

	private:
		void onUpdate(float deltaTime) override;
		void onLateUpdate() override;
		void onValidate() override;
		
		void input_keyEvent(const Application::KeyInputEvent& keyEvent);
		void input_buttonEvent(const Application::ButtonInputEvent& buttonEvent);
		void input_mouseMoveEvent(const Application::MouseMoveInputEvent& mouseMoveEvent);
		void input_wheelEvent(const Application::MouseWheelInputEvent& wheelEvent);
		
		void changeFocus(Element* element);

		Application::priv::WindowImpl* m_Window;
		LayoutManager* m_LayoutManager;

		/* State vars */
		Application::Cursor m_CurrentCursor;
		Element* m_FocusedElement;
		Element* m_CapturedElement;
		Element* m_PressedElement;
		Element* m_LastClickedElement;
		int m_ClickCount;
		long long m_LastClickTime;

		Canvas* getCanvas() override;
	};
} }

#endif