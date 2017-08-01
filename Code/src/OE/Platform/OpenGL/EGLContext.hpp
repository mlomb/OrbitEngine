#ifndef APPLICATION_EGLCONTEXT_HPP
#define APPLICATION_EGLCONTEXT_HPP

#include "OE/Platform/OpenGL/GLContext.hpp"
#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Application/Window.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	class EGLContext : public GLContext {
	public:
		EGLContext(WindowImpl* window, EGLContext* sharedContext);
		~EGLContext();

		bool isReady() override;
		void present() override;
		void makeCurrent(bool active = true) override;

		void createSurface(WindowNativeHandle handle);
		void destroySurface();

		const RenderAPI getAPI() override { return RenderAPI::OPENGL_ES; };
		
	private:
		::EGLConfig m_EGLConfig;
		::EGLDisplay m_EGLDisplay = 0;
		::EGLContext m_EGLContext = 0;
		::EGLSurface m_EGLSurface = 0;
	};
} } }

#endif