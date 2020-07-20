#include "CefBrowserClient.hpp"

#include "CefBrowserRenderHandler.hpp"

namespace OrbitEngine { namespace Editor {
	CefBrowserClient::CefBrowserClient()
	{
		m_RenderHandler = new CefBrowserRenderHandler();
	}

	CefBrowserClient::~CefBrowserClient()
	{
	}

	CefRefPtr<CefRenderHandler> CefBrowserClient::GetRenderHandler()
	{
		return m_RenderHandler;
	}
} }