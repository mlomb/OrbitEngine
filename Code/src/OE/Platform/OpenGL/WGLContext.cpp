#include "WGLContext.hpp"

#include "OE/Platform/Windows/WindowWindows.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	WGLContext::WGLContext(WindowWindows* window, WGLContext* sharedContext)
		: GLContext(window)
	{
		m_HWND = window->getWindowNativeHandle();
		m_DeviceContext = window->getDisplayNativeHandle();

		if (!setUpPixelFormat()) {
			OE_LOG_FATAL("Couldn't set up the pixel format.")
			return;
		}

		m_RenderContext = wglCreateContext(m_DeviceContext);
		if (!m_RenderContext) {
			OE_LOG_FATAL("Couldn't create the WGL render context.");
			return;
		}

		if (sharedContext) {
			sharedContext->makeCurrent(false);
			if (!wglShareLists(sharedContext->m_RenderContext, m_RenderContext))
				OE_LOG_FATAL("Couldn't share context lists.");
		}

		makeCurrent();
		OpenGLEXTENSIONS_Load();

		// load WGL extensions
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		contextInitialized();
	}

	WGLContext::~WGLContext()
	{
		wglMakeCurrent(NULL, NULL);
		if (m_RenderContext)
			wglDeleteContext(m_RenderContext);
	}

	bool WGLContext::setUpPixelFormat()
	{
		PIXELFORMATDESCRIPTOR pdf = { 0 };
		pdf.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pdf.nVersion = 1;
		pdf.cColorBits = 32;
		pdf.cDepthBits = 24;
		pdf.cStencilBits = 8;
		pdf.cAuxBuffers = 0;
		pdf.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pdf.iPixelType = PFD_TYPE_RGBA;
		pdf.iLayerType = PFD_MAIN_PLANE;

		unsigned int pixelFormat = ChoosePixelFormat(m_DeviceContext, &pdf);
		if (!pixelFormat) {
			OE_LOG_FATAL("Can't choose the pixel format.");
			return false;
		}

		if (!SetPixelFormat(m_DeviceContext, pixelFormat, &pdf)) {
			OE_LOG_FATAL("Can't set pixel format.");
			return false;
		}

		return true;
	}

	void WGLContext::makeCurrent(bool active)
	{
		if (m_DeviceContext) {
			if (active) {
				if (m_RenderContext) {
					wglMakeCurrent(m_DeviceContext, m_RenderContext);
				}
			}
			else
				wglMakeCurrent(m_DeviceContext, NULL);
		}

		GLContext::makeCurrent(active);
	}

	void WGLContext::present()
	{
		if (m_DeviceContext){
			if (SwapBuffers(m_DeviceContext)) {
				GLContext::present();
				return;
			}
		}

		OE_LOG_FATAL("Couldn't swap the buffers.")
	}

	void WGLContext::setSwapInterval(int sync)
	{
		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(sync);
	}
} } }
