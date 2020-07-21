#include "CefBrowserClient.hpp"

#include "CefBrowserRenderHandler.hpp"

namespace OrbitEngine { namespace Editor {

	CefBrowserClient::CefBrowserClient(EditorWindow* window)
		: m_Window(window)
	{
		m_RenderHandler = new CefBrowserRenderHandler(window);
	}

	CefBrowserClient::~CefBrowserClient()
	{
	}

	CefRefPtr<CefRenderHandler> CefBrowserClient::GetRenderHandler()
	{
		return m_RenderHandler;
	}
} }