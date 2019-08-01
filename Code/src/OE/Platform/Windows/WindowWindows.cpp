#include "WindowWindows.hpp"

#include "OE/Application/Window.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/Application/Context.hpp"
#include "OE/Platform/Windows/Windows.hpp"
#include "OE/Application/InputManager.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	
	WindowClass* WindowWindows::s_WindowClass = new WindowClass();

	WindowWindows::WindowWindows()
		: WindowImpl(), m_dwStyle(0), m_dwExStyle(0)
	{
		// Register class if necessary
		s_WindowClass->Reference();

		// Create the window
		m_WindowHandle = CreateWindowEx(0, OE_WNDCLASSNAME, "OrbitEngine", 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandleW(NULL), NULL);
		if (!m_WindowHandle) {
			OE_LOG_FATAL("Couldn't create the window")
			return;
		}

		initHandle();
	}

	WindowWindows::WindowWindows(HWND handle)
	{
		m_WindowHandle = handle;
		initHandle();
	}

	void WindowWindows::initHandle()
	{
		m_DeviceContext = GetDC(m_WindowHandle);

		SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, (LONG_PTR)this);
		SetWindowLongPtr(m_WindowHandle, GWLP_WNDPROC, (LONG_PTR)WindowWindows::DefWndProc);
	}
	
	WindowWindows::~WindowWindows()
	{
		if (m_WindowHandle) {
			SetWindowLongPtr(m_WindowHandle, GWLP_WNDPROC, 0);
			SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, 0);

			if(m_DeviceContext)
				ReleaseDC(m_WindowHandle, m_DeviceContext);
			DestroyWindow(m_WindowHandle);
			s_WindowClass->DeReference();
		}
	}

	/*
	void WindowWindows::requestCursorMode(const CursorMode cursorMode)
	{
		if (cursorMode == p_InputManager->getCursorMode())
			return;

		if (cursorMode == CursorMode::NORMAL) {
			setCursorPosition(m_LastFreeCursorPosition.x, m_LastFreeCursorPosition.y, true);
		}
		else {
			/*
			TODO Cancel delta
			Math::Vec2i grabDelta = p_Properties.resolution / 2 - p_InputManager->m_CursorPos;
			p_InputManager->m_CursorDelta += grabDelta;
			* /
			m_LastFreeCursorPosition = p_InputManager->getCursorPosition();
		}

		WindowImpl::requestCursorMode(cursorMode);
	}
	*/

	void WindowWindows::setCursorPosition(int x, int y, bool relative)
	{
		POINT pt;
		pt.x = x;
		pt.y = y;
		if (relative)
			ClientToScreen(m_WindowHandle, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	bool WindowWindows::isMinimized() const
	{
		return ::IsIconic(m_WindowHandle) != NULL;
	}
	
	bool WindowWindows::setDisplayMode(DisplayMode mode)
	{
		// Create window
		RECT windowRect;
		m_dwExStyle = 0;
		m_dwStyle = 0;

		switch (mode) {
		default:
		case DisplayMode::WINDOWED:
			m_dwExStyle = WS_EX_APPWINDOW;
			break;
		case DisplayMode::WINDOWED_TOOL:
			m_dwExStyle = WS_EX_TOOLWINDOW;
			break;
		case DisplayMode::BORDERLESS:
			m_dwStyle = WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
		case DisplayMode::FULLSCREEN:
		case DisplayMode::FULLSCREEN_BORDERLESS:
			m_dwStyle = WS_POPUP;
			break;
		case DisplayMode::OVERLAPPED:
			m_dwStyle = WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP;
			break;
		}

		if (mode == DisplayMode::WINDOWED || mode == DisplayMode::WINDOWED_TOOL) {
			m_dwExStyle |= WS_EX_WINDOWEDGE;
			m_dwStyle = WS_OVERLAPPEDWINDOW;
		}

		// WS_EX_APPWINDOW
		// WS_EX_TOOLWINDOW
		m_dwStyle |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

		::SetWindowLong(m_WindowHandle, GWL_STYLE, m_dwStyle);
		::SetWindowLong(m_WindowHandle, GWL_EXSTYLE, m_dwExStyle);

		if (isVisible()) {
			// required when changing display modes
			setVisibility(true);
		}

		p_DisplayMode = mode;
		return true;

		/*
		// Adjust to the surface size
		windowRect.left = (long)0; windowRect.top = (long)0;
		windowRect.right = (long)p_Size.x;
		windowRect.bottom = (long)p_Size.y;

		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

		unsigned int wX = (long)GetSystemMetrics(SM_CXSCREEN) / 2 - (windowRect.right - windowRect.left) / 2;
		unsigned int wY = (long)GetSystemMetrics(SM_CYSCREEN) / 2 - (windowRect.bottom - windowRect.top) / 2;
		
		if (p_DisplayMode == DisplayMode::BORDERLESS) {
			static const MARGINS shadowBorders{ 1,1,1,1 };
			DwmExtendFrameIntoClientArea(m_WindowHandle, &shadowBorders);
		}
		*/
	}

	bool WindowWindows::setCursor(Cursor cursor)
	{
		m_CursorTarget = cursor;
		return true;
	}

	bool WindowWindows::setTitle(const std::string& title)
	{
		return ::SetWindowText(m_WindowHandle, title.c_str()) != NULL;
	}

	bool WindowWindows::setPosition(const Math::Vec2i& position)
	{
		RECT rect = { (LONG)position.x, (LONG)position.y, (LONG)position.x, (LONG)position.y };
		if (::AdjustWindowRectEx(&rect, m_dwStyle, FALSE, m_dwExStyle) != NULL &&
			::SetWindowPos(m_WindowHandle, NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE) != NULL) {
			p_Position = position;
			return true;
		}
		return false;
	}

	bool WindowWindows::setSize(const Math::Vec2i& size)
	{
		RECT rect = { 0, 0, (LONG)size.x, (LONG)size.y };
		if (::AdjustWindowRectEx(&rect, m_dwStyle, FALSE, m_dwExStyle) != NULL && // Client to Screen
			::SetWindowPos(m_WindowHandle, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE) != NULL) {
			p_Size = size;
			return true;
		}
		return false;
	}

	bool WindowWindows::setVisibility(bool visible)
	{
		::ShowWindow(m_WindowHandle, visible ? SW_SHOW : SW_HIDE);
		return true;
	}

	bool WindowWindows::requestFocus()
	{
		if (::BringWindowToTop(m_WindowHandle) != NULL &&
			::SetForegroundWindow(m_WindowHandle) != NULL &&
			::SetFocus(m_WindowHandle) != NULL) {
			p_Focused = true;
			return true;
		}
		return false;
	}

	bool WindowWindows::setAlpha(float alpha)
	{
		if (alpha < 1.0f)
		{
			DWORD style = ::GetWindowLongW(m_WindowHandle, GWL_EXSTYLE) | WS_EX_LAYERED;
			::SetWindowLongW(m_WindowHandle, GWL_EXSTYLE, style);
			return ::SetLayeredWindowAttributes(m_WindowHandle, 0, (BYTE)(255 * alpha), LWA_ALPHA) != NULL;
		}
		else
		{
			DWORD style = ::GetWindowLongW(m_WindowHandle, GWL_EXSTYLE) & ~WS_EX_LAYERED;
			::SetWindowLongW(m_WindowHandle, GWL_EXSTYLE, style);
		}
		return true;
	}

	void WindowWindows::processEvents()
	{
		WindowImpl::processEvents();

		MSG message;
		while (PeekMessage(&message, m_WindowHandle, NULL, NULL, PM_REMOVE) > 0)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		/*
		if (p_Focused) {
			if (p_InputManager->m_CursorMode == CursorMode::GRABBED) {
				Math::Vec2i center = p_Size / 2;
				p_InputManager->m_CursorPos = center;
				setCursorPosition((int)center.x, (int)center.y, true);
			}
			else {
				/*
				RECT currentRect, innerRect;
				GetClipCursor(&currentRect);
				GetWindowInnerRect(&m_WindowHandle, &innerRect);
				if (currentRect.top == innerRect.top && currentRect.left == innerRect.left && currentRect.bottom == innerRect.bottom && currentRect.right == innerRect.right) {
				if (p_InputManager->m_CursorMode == CursorMode::LOCKED)
				ClipCursor(&innerRect);
				}
				else
				if (p_InputManager->m_CursorMode != CursorMode::LOCKED)
				ClipCursor(NULL);
				* /
			}
		}
		*/
	}

	DisplayNativeHandle WindowWindows::getDisplayNativeHandle()
	{
		return m_DeviceContext;
	}

	WindowNativeHandle WindowWindows::getWindowNativeHandle()
	{
		return m_WindowHandle;
	}
	
	LRESULT CALLBACK WindowWindows::DefWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowWindows* window = reinterpret_cast<WindowWindows*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (window && window->m_WindowHandle) {
			/*
			switch (uMsg)
			{
			case WM_NCCALCSIZE: {
				if (window->m_Window) {
					if (window->m_Window->GetDisplayMode() == DisplayMode::BORDERLESS)
						return 0;
				}
				break;
			}
			}
			*/
			if (!window->destroyRequested())
				return window->WndProc(hwnd, uMsg, wParam, lParam);
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	void WindowWindows::GetWindowInnerRect(HWND* windowHandle, RECT* r)
	{
		GetClientRect(*windowHandle, r);
		POINT p1 = { r->left, r->top };
		POINT p2 = { r->right, r->bottom };
		ClientToScreen(*windowHandle, &p1);
		ClientToScreen(*windowHandle, &p2);
		SetRect(r, p1.x, p1.y, p2.x, p2.y);
	}

	LPSTR WindowWindows::CursorToResource(Cursor cursor)
	{
		switch (cursor) {
		default:
		case DEFAULT:
		case ARROW:
			return IDC_ARROW;
		case CROSS:
			return IDC_CROSS;
		case HOURGLASS:
			return IDC_WAIT;
		case HAND:
			return IDC_HAND;
		case IBEAM:
			return IDC_IBEAM;
		case NO:
			return IDC_NO;
		case SIZEALL:
			return IDC_SIZEALL;
		case SIZENS:
			return IDC_SIZENS;
		case SIZEWE:
			return IDC_SIZEWE;
		case SIZENWSE:
			return IDC_SIZENWSE;
		case SIZENESW:
			return IDC_SIZENESW;
		case UPARROW:
			return IDC_UPARROW;
		};
	}
	
	LRESULT WindowWindows::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		InputManager* input_manager = InputManager::Get();

		switch (uMsg)
		{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			requestDestroy();
			return FALSE;
		}
		case WM_ACTIVATE:
		{
			if (!HIWORD(wParam))
			{
				//active = TRUE;
			}
			else
			{
				//active = FALSE;
			}
			return TRUE;
		}
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return FALSE;
			}
			break;
		}
		case WM_SETCURSOR:
		{
			if (LOWORD(lParam) == HTCLIENT) {
				SetCursor(m_CursorTarget == HIDDEN ? NULL : LoadCursor(NULL, CursorToResource(m_CursorTarget)));
				return TRUE;
			}
			else
				break;
		}
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
		{
			input_manager->onInputWheel((float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
			return TRUE;
		}
		case WM_KEYDOWN:
		{
			input_manager->onInputKey(Key(wParam), true);
			return TRUE;
		}
		case WM_KEYUP:
		{
			input_manager->onInputKey(Key(wParam), false);
			return TRUE;
		}
		case WM_MOUSEMOVE:
		{
			input_manager->onInputMouseMove(GET_X_LPARAM(lParam) + p_Position.x, GET_Y_LPARAM(lParam) + p_Position.y);
			return FALSE;
		}
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		{
			Button button;

			switch (uMsg) {
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			{
				button = LEFT;
				break;
			}
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			{
				button = RIGHT;
				break;
			}
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			{
				button = MIDDLE;
				break;
			}
			default:
			{
				if (GET_XBUTTON_WPARAM(wParam))
					button = N4;
				else
					button = N5;
				break;
			}
			}

			bool state = uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_XBUTTONDOWN;

			if (state) SetCapture(hWnd);
			else ReleaseCapture();

			input_manager->onInputMouseButton(button, state);
			return FALSE;
		}
		case WM_SIZE:
		{
			p_Size = Math::Vec2i(LOWORD(lParam), HIWORD(lParam));
			return FALSE;
		}
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		{
			bool focus = uMsg == WM_SETFOCUS;
			//if (p_Focused != focus)
			//	m_CursorMode = CursorMode::NORMAL;
			p_Focused = focus;
			return FALSE;
		}
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
} } }
