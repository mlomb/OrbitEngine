#ifndef APPLICATION_WGLCONTEXT_HPP
#define APPLICATION_WGLCONTEXT_HPP

#include "OE/Platform/OpenGL/GLContext.hpp"
#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Application/Window.hpp"

#include "OE/Platform/X11/WindowLinux.hpp"
#include "OE/Platform/X11/X11Display.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	class GLXContext : public GLContext {
	public:
		GLXContext(WindowLinux* window = 0, GLXContext* sharedContext = 0);
		~GLXContext();

		void present() override;
		void makeCurrent(bool active = true) override;
		GLXFBConfig* getFBConfigFromVisual(XVisualInfo* visual);

		const RenderAPI getAPI() override { return RenderAPI::OPENGL; };
	private:
		::Display* m_Display;
		::GLXContext m_Context;
		::Window m_Window;
	};
} } }

#endif