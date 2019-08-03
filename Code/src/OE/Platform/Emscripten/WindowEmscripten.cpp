#include "WindowEmscripten.hpp"

#include "OE/Application/InputManager.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	WindowEmscripten::WindowEmscripten()
		: WindowImpl()
	{
		updateSizeFromHTML();

    	emscripten_set_fullscreenchange_callback("#document", this, 0, EMS_Callback_FullscreenChange);
		emscripten_set_resize_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_ResizeCanvas);
		emscripten_set_focus_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_Focus);
		emscripten_set_blur_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_Focus);

		emscripten_set_keypress_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_KeyPress);
		emscripten_set_keydown_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_Key);
		emscripten_set_keyup_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_Key);
		emscripten_set_wheel_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_Wheel);
		emscripten_set_mousemove_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_MouseMove);
		emscripten_set_mouseup_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_MouseButton);
		emscripten_set_mousedown_callback(NULL, this, false, &WindowEmscripten::EMS_Callback_MouseButton);

		// Do we start with the focus?
		p_Focused = true;
	}
	
	void WindowEmscripten::processEvents()
	{
		m_PointerLock = isPointerLockActive();
		//p_InputManager->m_CursorMode = m_PointerLock ? CursorMode::GRABBED : CursorMode::NORMAL;

		WindowImpl::processEvents();
	}

	WindowEmscripten::~WindowEmscripten()
	{

	}

	bool WindowEmscripten::isMinimized() const
	{
		// TODO: This can be done
		return false;
	}

	bool WindowEmscripten::setDisplayMode(DisplayMode mode)
	{
		// TODO: !
		return true;
	}

	bool WindowEmscripten::setCursor(Cursor cursor)
	{
		std::string script = "Module.canvas.style.cursor = \"" + CursorToCSS(cursor) + "\";";
		emscripten_run_script(script.c_str());
		return true;
	}

	bool WindowEmscripten::setTitle(const std::string& title)
	{
		// TODO: change document.title?
		return false;
	}

	bool WindowEmscripten::setPosition(const Math::Vec2i& position)
	{
		// not valid
		return false;
	}

	bool WindowEmscripten::setSize(const Math::Vec2i& size)
	{
		// not valid
		return false;
	}

	bool WindowEmscripten::setVisibility(bool visible)
	{
		// TODO: set display: none?
		return true;
	}

	bool WindowEmscripten::requestFocus()
	{
		// TODO?
		return true;
	}

	bool WindowEmscripten::setAlpha(float alpha)
	{
		// not valid
		return false;
	}

	WindowNativeHandle WindowEmscripten::getWindowNativeHandle()
	{
		return 0;
	}

	DisplayNativeHandle WindowEmscripten::getDisplayNativeHandle()
	{
		return 0;
	}

	/*
	void WindowEmscripten::requestCursorMode(const CursorMode cursorMode) {
		if(cursorMode == CursorMode::GRABBED){
			emscripten_request_pointerlock(NULL, true);
		}
	}
	*/

	bool WindowEmscripten::isPointerLockActive(){
	    EmscriptenPointerlockChangeEvent pointerlockStatus;
	    emscripten_get_pointerlock_status(&pointerlockStatus);
		return pointerlockStatus.isActive;
	}

	void WindowEmscripten::updateSizeFromHTML(){
		//double pixel_ratio = emscripten_get_device_pixel_ratio(); Use it?
		double w = p_Size.x;
		double h = p_Size.y;

		if(p_DisplayMode != DisplayMode::FULLSCREEN){
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

		p_Size = Math::Vec2i(w, h);
	}

	EM_BOOL WindowEmscripten::EMS_Callback_FullscreenChange(int eventType, const EmscriptenFullscreenChangeEvent *fullscreenChangeEvent, void *userData) {
		WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
		if(fullscreenChangeEvent->isFullscreen)
			window->p_DisplayMode = DisplayMode::FULLSCREEN;
		else
			window->p_DisplayMode = DisplayMode::WINDOWED;
		window->updateSizeFromHTML();
	}

	EM_BOOL WindowEmscripten::EMS_Callback_ResizeCanvas(int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
		WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
		window->updateSizeFromHTML();
	}

	EM_BOOL WindowEmscripten::EMS_Callback_Focus(int eventType, const EmscriptenFocusEvent *focusEvent, void *userData) {
		WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
		window->p_Focused = eventType == EMSCRIPTEN_EVENT_FOCUS;
	}

	EM_BOOL WindowEmscripten::EMS_Callback_Key(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
		if (keyEvent->keyCode < OE_MAX_KEYS) {
			Key key = Key(keyEvent->keyCode);
			InputManager::Get()->onInputKey(key, eventType == EMSCRIPTEN_EVENT_KEYDOWN);
			bool isNavKey = key == Key::BACKSPACE ||
							key == Key::TAB;
			return isNavKey;
		}
	}

	EM_BOOL OrbitEngine::Application::priv::WindowEmscripten::EMS_Callback_KeyPress(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData)
	{
		InputManager::Get()->onInputChar((unsigned int)keyEvent->charCode);
	}

	EM_BOOL WindowEmscripten::EMS_Callback_Wheel(int eventType, const EmscriptenWheelEvent *wheelEvent, void *userData) {
		InputManager::Get()->onInputWheel((float)wheelEvent->deltaY / -120.0f);
	}

	EM_BOOL WindowEmscripten::EMS_Callback_MouseMove(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
		WindowEmscripten* window = static_cast<WindowEmscripten*>(userData);
		if(window->m_PointerLock){
			InputManager::Get()->onInputMouseMove(-mouseEvent->movementX, -mouseEvent->movementY, true);
		} else {
			InputManager::Get()->onInputMouseMove(mouseEvent->canvasX, mouseEvent->canvasY);
		}
	}

	EM_BOOL WindowEmscripten::EMS_Callback_MouseButton(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
		Button btn;
		switch (mouseEvent->button) {
		default:
		case 0: btn = Button::LEFT; break;
		case 1: btn = Button::MIDDLE; break;
		case 2: btn = Button::RIGHT; break;
		}
		InputManager::Get()->onInputMouseButton(btn, eventType == EMSCRIPTEN_EVENT_MOUSEDOWN);
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