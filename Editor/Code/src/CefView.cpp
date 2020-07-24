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
		/*
		std::cout << "THREAD " << std::this_thread::get_id() << " -- ";
		std::cout << "FRAME " << width << "x" << height << " T: " << type << " R: ";
		for (CefRect r : dirtyRects) {
			std::cout << "(" << r.x << "," << r.y << "  " << r.width << "," << r.height << ") ";
		}
		std::cout << std::endl;
		if (dirtyRects[0].width != width || dirtyRects[0].height != height) {
			std::cout << " ======================================= " << std::endl;
		}
		*/

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