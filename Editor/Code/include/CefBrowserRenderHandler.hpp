#ifndef CEF_BROWSER_RENDER_HANDLER_HPP
#define CEF_BROWSER_RENDER_HANDLER_HPP

#include "Cef.hpp"

namespace OrbitEngine { namespace Editor {
	class CefBrowserRenderHandler : public CefRenderHandler {
	public:
		CefBrowserRenderHandler();
		~CefBrowserRenderHandler();

		void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;

		void OnPaint(CefRefPtr<CefBrowser> browser,
					 PaintElementType type,
					 const RectList& dirtyRects,
					 const void* buffer,
					 int width,
					 int height) override;

	private:
		IMPLEMENT_REFCOUNTING(CefBrowserRenderHandler);
	};
} }

#endif