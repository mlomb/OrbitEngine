#ifndef CEF_BROWSER_RENDER_HANDLER_HPP
#define CEF_BROWSER_RENDER_HANDLER_HPP

#include "Cef.hpp"

#include <OE/Graphics/API/Texture.hpp>

namespace OrbitEngine { namespace Editor {
	using namespace OrbitEngine::Graphics;

	class EditorWindow;

	class CefBrowserRenderHandler : public CefRenderHandler {
	public:
		CefBrowserRenderHandler(EditorWindow* window);
		~CefBrowserRenderHandler();

		void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;

		void OnPaint(CefRefPtr<CefBrowser> browser,
					 PaintElementType type,
					 const RectList& dirtyRects,
					 const void* buffer,
					 int width,
					 int height) override;

	private:
		EditorWindow* m_Window;

		IMPLEMENT_REFCOUNTING(CefBrowserRenderHandler);
	};
} }

#endif