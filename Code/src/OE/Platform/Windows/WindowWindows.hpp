#ifndef APPLICATION_WINDOW_WINDOWS_HPP
#define APPLICATION_WINDOW_WINDOWS_HPP

#include "OE\Application\WindowImpl.hpp"
#include "OE\Platform\Windows\WindowClass.hpp"

namespace OrbitEngine {
	enum RenderAPI;

	namespace Application {
	struct WindowProperties;
		
	namespace priv {
	class WindowWindows : public WindowImpl {
	public:
		WindowWindows(WindowProperties windowProperties);
		WindowWindows(HWND handle);
		~WindowWindows();

		void requestCursorMode(const CursorMode cursorMode) override;
		void setTitle(const char* title) override;
		void setCursorPosition(int x, int y, bool relative);
		void processEvents() override;

		DisplayNativeHandle getDisplayNativeHandle() override;
		WindowNativeHandle getWindowNativeHandle() override;

		static LRESULT CALLBACK DefWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static void GetWindowInnerRect(HWND* windowHandle, RECT* r);
		static LPSTR CursorToResource(Cursor cursor);
	private:
		static WindowClass* s_WindowClass;

		void initHandle();

		HWND m_WindowHandle;
		HDC m_DeviceContext;
		Math::Vec2i m_LastFreeCursorPosition;

		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
} } }

#endif
