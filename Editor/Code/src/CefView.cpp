#include "CefView.hpp"

#include <iostream>
#include <thread>

#include <OE/Misc/Log.hpp>

namespace OrbitEngine { namespace Editor {

	CefView::CefView()
		: m_View(0, 0, 0, 0)
	{
		resize(1, 1);
	}

	CefView::~CefView()
	{
		OE_ASSERT_MSG(!m_Browser, "The CefView must be closed before being destroyed");
	}

	void CefView::close()
	{
		m_Browser->GetHost()->CloseBrowser(true);
		m_Browser = nullptr;
	}

	void CefView::resize(int w, int h)
	{
		w = std::max(1, w);
		h = std::max(1, h);
		m_View = CefRect(0, 0, w, h);
		if (m_Browser)
			m_Browser->GetHost()->WasResized();
	}

	void CefView::OnAfterCreated(CefRefPtr<CefBrowser> browser)
	{
		m_Browser = browser;
	}

	void CefView::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
	{
		rect = m_View;
	}

	void CefView::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
	{
		// TODO: FIXME
		//		 HACK: Cef seems to send frames with the correct size but incorrect "zoom"
		//             after fast resizing. Seems to be a problem of Cef 72+
		//             The hack reads 4 pixels and check if those are transparent,
		//             if any of them are, discard the frame.
		//
		//       Hopefully this will be gone when they fix OnAcceleratedPaint,
		//       which is broken in Cef 84
		#define TEST_PX(x, y) \
		if (((uint32_t*)buffer)[std::max(0, y-1) * width + std::max(0, x-1)] == 0) \
			return; \

		TEST_PX(width / 2, 0);
		TEST_PX(width / 2, height);
		TEST_PX(0,     height / 2);
		TEST_PX(width, height / 2);

		#undef TEST_PX

		switch (type) {
		case PaintElementType::PET_VIEW:
			m_ViewBuffer.update(buffer, width, height);
			break;
		case PaintElementType::PET_POPUP:
			m_PopupBuffer.update(buffer, width, height);
			break;
		}
	}

	CefElementBuffer& CefView::getViewBuffer()
	{
		return m_ViewBuffer;
	}

	CefElementBuffer& CefView::getPopupBuffer()
	{
		return m_PopupBuffer;
	}

	CefRefPtr<CefRenderHandler> CefView::GetRenderHandler()
	{
		return  this;
	}

	CefRefPtr<CefLifeSpanHandler> CefView::GetLifeSpanHandler()
	{
		return  this;
	}

	CefRefPtr<CefLoadHandler> CefView::GetLoadHandler()
	{
		return  this;
	}

} }