#ifndef APPLICATION_WINDOWIMPL_HPP
#define APPLICATION_WINDOWIMPL_HPP

#include "OE/Application/NativeHandlers.hpp"
#include "OE/Application/Window.hpp"
#include "OE/Application/View.hpp"
#include "OE/Application/InputManager.hpp"

namespace OrbitEngine {	namespace Application {
	struct WindowProperties;

	namespace priv {
	class ContextImpl;

	class WindowImpl : public View {
	public:
		virtual ~WindowImpl();

		virtual void processEvents();
		virtual void requestCursorMode(const CursorMode cursorMode);
		virtual void setTitle(const char* title) = 0;
		virtual bool destroyRequested();
		virtual WindowNativeHandle getWindowNativeHandle() = 0;
		virtual DisplayNativeHandle getDisplayNativeHandle() = 0;

		WindowProperties& getProperties();
		InputManager* getInputManager();

	protected:
		WindowImpl(WindowProperties properties = WindowProperties());
		
		void requestDestroy();

		friend class ContextImpl;
		friend class InputManager;

		WindowProperties p_Properties;
		InputManager* p_InputManager;
		ContextImpl* p_Context = 0;
	private:
		bool m_DestroyRequested = false;
	};
} } }

#endif