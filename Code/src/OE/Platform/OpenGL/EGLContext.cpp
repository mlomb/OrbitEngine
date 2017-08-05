#include "OE/Platform/OpenGL/EGLContext.hpp"

#include <stdio.h>

#if OE_ANDROID
	#include "OE/Platform/Android/SystemAndroid.hpp"
#endif

#include "OE/Application/WindowImpl.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	EGLContext::EGLContext(WindowImpl* window, EGLContext* sharedContext)
		: GLContext(window)
	{
		EGLint numConfigs;
		EGLint contextAttribs[] = {
#if OE_EMSCRIPTEN
			EGL_CONTEXT_CLIENT_VERSION, 2,
#else
			EGL_CONTEXT_CLIENT_VERSION, 3,
#endif
			EGL_NONE, EGL_NONE
		};
		
		m_EGLDisplay = eglGetDisplay((EGLNativeDisplayType)window->getDisplayNativeHandle());
		if (m_EGLDisplay == EGL_NO_DISPLAY) {
			OE_LOG_FATAL("Couldn't get display. EGL_NO_DISPLAY")
			return;
		}

		EGLint majorVersion, minorVersion;
		if (!eglInitialize(m_EGLDisplay, &majorVersion, &minorVersion)) {
			OE_LOG_FATAL("Can't initialize EGL")
			return;
		}

		// We're using EGL for OpenGLES	
		eglBindAPI(EGL_OPENGL_ES_API);

		// Get configs
		if (!eglGetConfigs(m_EGLDisplay, NULL, 0, &numConfigs)) {
			OE_LOG_FATAL("Can't retrieve EGL configs")
			return;
		}

		int flags = 0;
		EGLint attribList[] =
		{
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
    		EGL_DEPTH_SIZE, 24,
			EGL_NONE
		};

		// Choose config
		if (!eglChooseConfig(m_EGLDisplay, attribList, &m_EGLConfig, 1, &numConfigs)) {
			OE_LOG_FATAL("Couldn't choose EGL config")
			return;
		}

		// Android create and destroy the surface async
#if !OE_ANDROID
		createSurface(window->getWindowNativeHandle());
#endif

		// Create a GL context
		m_EGLContext = eglCreateContext(m_EGLDisplay, m_EGLConfig, /*EGL_NO_CONTEXT*/0, contextAttribs);
		if (m_EGLContext == 0) {
			OE_LOG_FATAL("Can't create EGL context")
			return;
		}
		
		makeCurrent();
#if OE_OPENGL
		// If not GLES...
		OpenGLEXTENSIONS_Load();
#endif
#if !OE_ANDROID
		contextInitialized();
#endif
	}

	EGLContext::~EGLContext()
	{
		if (m_EGLDisplay != NULL) {
			eglMakeCurrent(m_EGLDisplay, NULL, NULL, 0);
			if (m_EGLContext != NULL)
				eglDestroyContext(m_EGLDisplay, m_EGLContext);
			destroySurface();
			eglTerminate(m_EGLDisplay);
		}
		m_EGLDisplay = NULL;
		m_EGLContext = NULL;
		m_EGLSurface = NULL;
	}

	void EGLContext::createSurface(WindowNativeHandle handle)
	{
		// Ensure the old surface is destroyed
		destroySurface();
		// Create a surface
		m_EGLSurface = eglCreateWindowSurface(m_EGLDisplay, m_EGLConfig, handle, NULL);
		if (m_EGLSurface == EGL_NO_SURFACE)
			OE_LOG_FATAL("Can't create EGL surface")
	}

	void EGLContext::destroySurface()
	{
		if (m_EGLSurface != NULL)
			eglDestroySurface(m_EGLDisplay, m_EGLSurface);
		m_EGLSurface = NULL;
	}

	void EGLContext::present()
	{
		if(m_EGLSurface) {
			eglSwapBuffers(m_EGLDisplay, m_EGLSurface);

			GLContext::present();
		}
	}

	void EGLContext::makeCurrent(bool active)
	{
		if(m_EGLDisplay){
			if(active) {
				if(m_EGLContext && m_EGLSurface){
					eglMakeCurrent(m_EGLDisplay, m_EGLSurface, m_EGLSurface, m_EGLContext);
				}
			} else
				eglMakeCurrent(m_EGLDisplay, NULL, NULL, NULL);
		}
        
		GLContext::makeCurrent(active);
	}

	bool EGLContext::isReady(){
		return m_EGLContext && m_EGLSurface;
	}
} } }