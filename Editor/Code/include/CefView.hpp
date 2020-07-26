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

		bool isViewOpen() const;
		void resizeView(int w, int h);
		void closeView();

		void sendMessage(const std::string& data);
		virtual void onMessage(const std::string& data) {};

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

		/// Receives the messages sent by CefController
		bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
									  CefRefPtr<CefFrame> frame,
									  CefProcessId source_process,
									  CefRefPtr<CefProcessMessage> message) override;

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