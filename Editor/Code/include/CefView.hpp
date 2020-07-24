#ifndef CEF_VIEW_HPP
#define CEF_VIEW_HPP

#include "Cef.hpp"
#include "CefElementBuffer.hpp"

namespace OrbitEngine { namespace Editor {

	class CefView :
		public CefClient,
		public CefRenderHandler,
		public CefLifeSpanHandler,
		public CefLoadHandler {
	public:
		CefView();
		~CefView();

		void close();
		void resize(int w, int h);
		CefElementBuffer& getViewBuffer();
		CefElementBuffer& getPopupBuffer();

		void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
		void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
		void OnPaint(CefRefPtr<CefBrowser> browser,
			PaintElementType type,
			const RectList& dirtyRects,
			const void* buffer,
			int width,
			int height) override;

		CefRefPtr<CefRenderHandler> GetRenderHandler() override;
		CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
		CefRefPtr<CefLoadHandler> GetLoadHandler() override;

	private:
		CefRect m_View;
		CefRefPtr<CefBrowser> m_Browser;
		CefElementBuffer m_ViewBuffer, m_PopupBuffer;

		IMPLEMENT_REFCOUNTING(CefView);
	};
} }

#endif