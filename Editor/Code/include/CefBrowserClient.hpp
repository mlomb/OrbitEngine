#ifndef CEF_BROWSER_CLIENT_HPP
#define CEF_BROWSER_CLIENT_HPP

#include "Cef.hpp"

namespace OrbitEngine { namespace Editor {
	class CefBrowserClient : public CefClient {
	public:
		CefBrowserClient();
		~CefBrowserClient();

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler();

	private:
		CefRefPtr<CefRenderHandler> m_RenderHandler;

		IMPLEMENT_REFCOUNTING(CefBrowserClient);
	};
} }

#endif