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

		auto blit_texture = m_Window->getBrowserBlitTexture();

		if (blit_texture) {
			auto props = blit_texture->getProperties();
			rect = CefRect(0, 0, props.width, props.height);
		}
		else {
			rect = CefRect(0, 0, 100, 100);
		}
	}

	void CefBrowserRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
	{
		printf("%i > frame %p %p\n", GetCurrentThreadId(), this, buffer);

		auto blit_texture = m_Window->getBrowserBlitTexture();
		if (blit_texture) {
			if (blit_texture->getProperties().width != width || blit_texture->getProperties().height != height) {
				// WARNING: sizes doesn't match
				printf("%i > SIZES DONT MATCH\n", GetCurrentThreadId());
				return;
			}

			blit_texture->setData(const_cast<void*>(buffer)); // should be const void*
		}
	}
} }