#include "CefBrowserRenderHandler.hpp"

namespace OrbitEngine { namespace Editor {
	CefBrowserRenderHandler::CefBrowserRenderHandler()
	{
	}

	CefBrowserRenderHandler::~CefBrowserRenderHandler()
	{
	}

	void CefBrowserRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
	{
		rect = CefRect(0, 0, 1024, 1024);
	}

	void CefBrowserRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
	{
		printf("frame %p\n", this);
	}
} }