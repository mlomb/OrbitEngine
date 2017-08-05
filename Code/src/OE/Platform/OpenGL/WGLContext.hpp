#ifndef APPLICATION_WGLCONTEXT_HPP
#define APPLICATION_WGLCONTEXT_HPP

#include "OE/Platform/OpenGL/GLContext.hpp"
#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Application/WindowImpl.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	class WindowWindows;

	class WGLContext : public GLContext {
	public:
		WGLContext(WindowWindows* window = 0, WGLContext* sharedContext = 0);
		~WGLContext();

		bool setUpPixelFormat();
		void makeCurrent(bool active = true) override;
		void present() override;
	private:
		HGLRC m_RenderContext;
		HDC m_DeviceContext;
		HWND m_HWND;
	};
} } }

#endif