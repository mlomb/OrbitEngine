#include "WindowWindows.hpp"

#include "OE/Application/Window.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/Application/Context.hpp"
#include "OE/Platform/Windows/Windows.hpp"
#include "OE/Application/InputManager.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	
	WindowClass* WindowWindows::s_WindowClass = new WindowClass();

	WindowWindows::WindowWindows(WindowProperties properties) : WindowImpl(properties)
	{
		// Register class if necessary
		s_WindowClass->Reference();

		// Create window
		RECT windowRect;
		DWORD dwExStyle = 0;
		DWORD dwStyle = 0;

		switch (properties.displayMode) {
		default:
		case DisplayMode::WINDOWED:
			dwExStyle = WS_EX_APPWINDOW;
			break;
		case DisplayMode::WINDOWED_TOOL:
			dwExStyle = WS_EX_TOOLWINDOW;
			break;
		case DisplayMode::BORDERLESS:
			dwStyle = WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
		case DisplayMode::FULLSCREEN:
		case DisplayMode::FULLSCREEN_BORDERLESS:
			dwStyle = WS_POPUP;
			break;
		case DisplayMode::OVERLAPPED:
			dwStyle = WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP;
			break;
		}
		
		if (properties.displayMode == DisplayMode::WINDOWED || properties.displayMode == DisplayMode::WINDOWED_TOOL) {
			dwExStyle |= WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW;
		}

		// WS_EX_APPWINDOW
		// WS_EX_TOOLWINDOW
		dwStyle |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

		// Adjust to the surface size
		windowRect.left = (long)0; windowRect.top = (long)0;
		windowRect.right = (long)p_Properties.resolution.w;
		windowRect.bottom = (long)p_Properties.resolution.h;

		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);
		
		unsigned int wX = (long)GetSystemMetrics(SM_CXSCREEN) / 2 - (windowRect.right - windowRect.left) / 2;
		unsigned int wY = (long)GetSystemMetrics(SM_CYSCREEN) / 2 - (windowRect.bottom - windowRect.top) / 2;

		// Create the window
		m_WindowHandle = CreateWindowEx(dwExStyle, OE_WNDCLASSNAME, properties.title.c_str(), dwStyle, wX, wY, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, properties.parent, NULL, GetModuleHandleW(NULL), NULL);
		if (!m_WindowHandle) {
			OE_LOG_FATAL("Couldn't create the window")
			return;
		}

		if (properties.displayMode == DisplayMode::BORDERLESS) {
			static const MARGINS shadowBorders{ 1,1,1,1 };
			DwmExtendFrameIntoClientArea(m_WindowHandle, &shadowBorders);
		}

		initHandle();

		ShowWindow(m_WindowHandle, SW_SHOW);
		SetFocus(m_WindowHandle);
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
			SetWindowLongPtr(m_WindowHandle, GWLP_WNDPROC, (LONG)DefWindowProc);
			SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, 0);

			if(m_DeviceContext)
				ReleaseDC(m_WindowHandle, m_DeviceContext);
			DestroyWindow(m_WindowHandle);
			s_WindowClass->DeReference();
		}
	}

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
			*/
			m_LastFreeCursorPosition = p_InputManager->getCursorPosition();
		}

		WindowImpl::requestCursorMode(cursorMode);
	}

	void WindowWindows::setTitle(const char* title)
	{
		SetWindowText(m_WindowHandle, title);
	}

	void WindowWindows::setCursorPosition(int x, int y, bool relative)
	{
		POINT pt;
		pt.x = x;
		pt.y = y;
		if (relative)
			ClientToScreen(m_WindowHandle, &pt);
		SetCursorPos(pt.x, pt.y);
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


		if (p_InputManager->hasFocus()) {
			if (p_InputManager->m_CursorMode == CursorMode::GRABBED) {
				Math::Vec2i center = p_Properties.resolution / 2;
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
				*/
			}
		}
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
		if (window) {
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
		case UPARROW:
			return IDC_UPARROW;
		};
	}
	
	LRESULT WindowWindows::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
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
				SetCursor(p_InputManager->m_Cursor == HIDDEN ? NULL : LoadCursor(NULL, CursorToResource(p_InputManager->m_Cursor)));
				return TRUE;
			}
			else
				break;
		}
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
		{
			p_InputManager->onInputWheel(GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
			return TRUE;
		}
		case WM_KEYDOWN:
		{
			if (wParam < MAX_KEYS)
				p_InputManager->onInputKey(Key(wParam), true);
			return TRUE;
		}
		case WM_KEYUP:
		{
			if (wParam < MAX_KEYS)
				p_InputManager->onInputKey(Key(wParam), false);
			return TRUE;
		}
		case WM_MOUSEMOVE:
		{
			p_InputManager->onInputMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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

			p_InputManager->onInputMouseButton(button, state);
			return FALSE;
		}
		case WM_SIZE:
		{
			p_InputManager->onInputResized(LOWORD(lParam), HIWORD(lParam));
			return FALSE;
		}
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		{
			bool focus = uMsg == WM_SETFOCUS;

			p_InputManager->onInputFocus(focus);
			return FALSE;
		}
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
} } }
