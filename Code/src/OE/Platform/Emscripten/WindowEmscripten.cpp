#include "WindowEmscripten.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	WindowEmscripten::WindowEmscripten(WindowProperties properties)
		: WindowImpl(properties)
	{
		updateSizeFromHTML();

    	emscripten_set_fullscreenchange_callback("#document", this, 0, EMS_Callback_FullscreenChange);
		emscripten_set_resize_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_ResizeCanvas);
		emscripten_set_focus_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_Focus);
		emscripten_set_blur_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_Focus);

		emscripten_set_keydown_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_Key);
		emscripten_set_keyup_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_Key);
		emscripten_set_wheel_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_Wheel);
		emscripten_set_mousemove_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_MouseMove);
		emscripten_set_mouseup_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_MouseButton);
		emscripten_set_mousedown_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_MouseButton);

		// Do we start with the focus?
		p_InputManager->onInputFocus(true);
	}
	
	void WindowEmscripten::processEvents()
	{
		m_PointerLock = isPointerLockActive();
		p_InputManager->m_CursorMode = m_PointerLock ? CursorMode::GRABBED : CursorMode::NORMAL;

		std::string script = "Module.canvas.style.cursor = \"" + CursorToCSS(p_InputManager->m_Cursor) + "\";";
		emscripten_run_script(script.c_str());

		WindowImpl::processEvents();
	}

	WindowEmscripten::~WindowEmscripten()
	{

	}

	WindowNativeHandle WindowEmscripten::getWindowNativeHandle()
	{
		return 0;
	}

	DisplayNativeHandle WindowEmscripten::getDisplayNativeHandle()
	{
		return 0;
	}

	void WindowEmscripten::requestCursorMode(const CursorMode cursorMode) {
		if(cursorMode == CursorMode::GRABBED){
			emscripten_request_pointerlock(NULL, true);
		}
	}

	bool WindowEmscripten::isPointerLockActive(){
	    EmscriptenPointerlockChangeEvent pointerlockStatus;
	    emscripten_get_pointerlock_status(&pointerlockStatus);
		return pointerlockStatus.isActive;
	}

	void WindowEmscripten::updateSizeFromHTML(){
		//double pixel_ratio = emscripten_get_device_pixel_ratio(); Use it?
		double w = p_Properties.resolution.w;
		double h = p_Properties.resolution.h;

		if(p_Properties.displayMode != DisplayMode::FULLSCREEN){
			// If we are not in fullscreen, we use the css size values
			emscripten_get_element_css_size(NULL, &w, &h);
		} else {
			// If we are in fullscreen we use the window inner dimensions
			w = EM_ASM_INT_V({
				return window.innerWidth;
			});

			h = EM_ASM_INT_V({
				return window.innerHeight;
			});
		}

		emscripten_set_canvas_size(w, h);

		p_InputManager->onInputResized(w, h);
	}

	void WindowEmscripten::setTitle(const char* title)
	{
		// What
	}

	EM_BOOL WindowEmscripten::EMS_Callback_FullscreenChange(int eventType, const EmscriptenFullscreenChangeEvent *fullscreenChangeEvent, void *userData) {
		WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
		if(fullscreenChangeEvent->isFullscreen)
			window->p_Properties.displayMode = DisplayMode::FULLSCREEN;
		else
			window->p_Properties.displayMode = DisplayMode::WINDOWED;
		window->updateSizeFromHTML();
	}

	EM_BOOL WindowEmscripten::EMS_Callback_ResizeCanvas(int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
		WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
		window->updateSizeFromHTML();
	}

	EM_BOOL WindowEmscripten::EMS_Callback_Focus(int eventType, const EmscriptenFocusEvent *focusEvent, void *userData) {
		WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
		window->p_InputManager->onInputFocus(eventType == EMSCRIPTEN_EVENT_FOCUS);
	}

	EM_BOOL WindowEmscripten::EMS_Callback_Key(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
		if (keyEvent->keyCode < MAX_KEYS) {
			Key key = Key(keyEvent->keyCode);
			WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
			window->p_InputManager->onInputKey(key, eventType == EMSCRIPTEN_EVENT_KEYDOWN);
			bool isNavKey = key == Key::BACKSPACE ||
							key == Key::TAB;
			return isNavKey;
		}
	}

	EM_BOOL WindowEmscripten::EMS_Callback_Wheel(int eventType, const EmscriptenWheelEvent *wheelEvent, void *userData) {
		WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
		window->p_InputManager->onInputWheel(wheelEvent->deltaY);
	}

	EM_BOOL WindowEmscripten::EMS_Callback_MouseMove(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
		WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
		if(window->m_PointerLock){
			window->p_InputManager->onInputMouseMove(-mouseEvent->movementX, -mouseEvent->movementY, true);
		} else {
			window->p_InputManager->onInputMouseMove(mouseEvent->canvasX, mouseEvent->canvasY);
		}
	}

	EM_BOOL WindowEmscripten::EMS_Callback_MouseButton(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
		WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
		window->p_InputManager->onInputMouseButton(Button(mouseEvent->button), eventType == EMSCRIPTEN_EVENT_MOUSEDOWN);
	}

	std::string WindowEmscripten::CursorToCSS(const Cursor cursor){
		switch(cursor){
		default:
		case DEFAULT:
		case ARROW:
			return "";
		case CROSS:
			return "crosshair";
		case HOURGLASS:
			return "wait";
		case HAND:
			return "pointer";
		case NO:
			return "not-allowed";
		case SIZEALL:
			return "move";
		case SIZENS:
			return "n-resize";
		case SIZEWE:
			return "w-resize";
		case SIZENWSE:
			return "se-resize";
		}
	}

} } }