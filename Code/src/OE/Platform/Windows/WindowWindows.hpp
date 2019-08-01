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
		WindowWindows();
		WindowWindows(HWND handle);
		~WindowWindows();

		void setCursorPosition(int x, int y, bool relative);

		bool isMinimized() const override;
		bool setDisplayMode(DisplayMode mode) override;
		bool setCursor(Cursor cursor) override;
		bool setTitle(const std::string& title) override;
		bool setPosition(const Math::Vec2i& position) override;
		bool setSize(const Math::Vec2i& size) override;
		bool setVisibility(bool visible) override;
		bool requestFocus() override;
		bool setAlpha(float alpha) override;

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
		DWORD m_dwExStyle = 0;
		DWORD m_dwStyle = 0;

		Math::Vec2i m_LastFreeCursorPosition;
		Cursor m_CursorTarget;

		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
} } }

#endif
