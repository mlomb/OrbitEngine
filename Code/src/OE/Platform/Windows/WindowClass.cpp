#include "OE/Platform/Windows/WindowClass.hpp"

#include "OE/Platform/Windows/WindowWindows.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	void WindowClass::Init() {
		WNDCLASSEX window = { 0 };
		ZeroMemory(&window, sizeof(window));

		window.cbSize = sizeof(window);
		window.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		window.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		window.lpfnWndProc = WindowWindows::DefWndProc;
		window.hCursor = LoadCursor(NULL, IDC_ARROW);
		window.lpszClassName = OE_WNDCLASSNAME;
		window.hInstance = GetModuleHandleW(NULL);

		RegisterClassEx(&window);
	}

	void WindowClass::DeInit() {
		UnregisterClass(OE_WNDCLASSNAME, GetModuleHandle(NULL));
	}
} } }