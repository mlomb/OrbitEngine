#include "CefBrowserRenderHandler.hpp"

#include <OE/Graphics/API/Texture.hpp>
#include <OE/Misc/Log.hpp>

#include "EditorWindow.hpp"

namespace OrbitEngine { namespace Editor {

	CefBrowserRenderHandler::CefBrowserRenderHandler(EditorWindow* window)
		: m_Window(window)
	{
	}

	CefBrowserRenderHandler::~CefBrowserRenderHandler()
	{
	}
	
	void CefBrowserRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
	{
		printf("%i > ASKED FOR SIZE\n", GetCurrentThreadId());
		auto size = m_Window->getSize();
		rect = CefRect(0, 0, size.x, size.y);
	}

	void CefBrowserRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
	{
		m_Window->blitBrowser(buffer, width, height);
	}
} }