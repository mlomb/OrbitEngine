#ifndef CEF_BROWSER_CLIENT_HPP
#define CEF_BROWSER_CLIENT_HPP

#include "Cef.hpp"

namespace OrbitEngine { namespace Editor {
	class EditorWindow;

	class CefBrowserClient : public CefClient {
	public:
		CefBrowserClient(EditorWindow* window);
		~CefBrowserClient();

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler();

	private:
		EditorWindow* m_Window;

		CefRefPtr<CefRenderHandler> m_RenderHandler;

		IMPLEMENT_REFCOUNTING(CefBrowserClient);
	};
} }

#endif