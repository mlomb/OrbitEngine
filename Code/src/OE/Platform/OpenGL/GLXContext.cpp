#include "GLXContext.hpp"

#define GLX_CONTEXT_MAJOR_VERSION_ARB		0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB		0x2092
typedef ::GLXContext(*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, ::GLXContext, Bool, const int*);

namespace OrbitEngine { namespace Application { namespace priv {
	GLXContext::GLXContext(WindowLinux* window, GLXContext* sharedContext)
		: GLContext(window)
	{
		m_Display = X11Display::Instance()->GetAndReference();
		m_Window = window->getWindowNativeHandle();

		/* ---- */
		// Get visual from the window
		XWindowAttributes windowAttributes;
		XGetWindowAttributes(m_Display, m_Window, &windowAttributes);

		XVisualInfo tpl;
		tpl.screen = DefaultScreen(m_Display);
		tpl.visualid = XVisualIDFromVisual(windowAttributes.visual);
		int nbVisuals = 0;
		XVisualInfo* visualInfo = XGetVisualInfo(m_Display, VisualIDMask | VisualScreenMask, &tpl, &nbVisuals);

		// Get the FBConfig from the visual we got from the window
		GLXFBConfig* config = getFBConfigFromVisual(visualInfo);

		glXCreateContextAttribsARBProc glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
		if(glXCreateContextAttribsARB){
			int attribs[] = {
				GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
				GLX_CONTEXT_MINOR_VERSION_ARB, 3,
				0
			};
			m_Context = glXCreateContextAttribsARB(m_Display, *config, 0, true, attribs);			
		} else {
			OE_LOG_WARNING("Couldn't retrive glXCreateContextAttribsARB")
			// Fallback
			m_Context = glXCreateNewContext(m_Display, *config, GLX_RGBA_TYPE, 0, true);
		}

		if(!m_Context){
			OE_LOG_FATAL("Couldn't create context")
			return;
		}
		
		makeCurrent();
		OpenGLEXTENSIONS_Load();
		contextInitialized();
	}

	GLXContext::~GLXContext()
	{
		if (m_Context){
			glXDestroyContext(m_Display, m_Context);
			m_Context = NULL;
		}
		X11Display::Instance()->DeReference();
		m_Display = NULL;
	}
	
	void GLXContext::present()
	{
		if(m_Window){
			glXSwapBuffers(m_Display, m_Window);

			GLContext::present();
		}
	}

	void GLXContext::makeCurrent(bool active)
	{
		if(active) {
			if(m_Context)
				glXMakeCurrent(m_Display, m_Window, m_Context);
		} else
			glXMakeCurrent(m_Display, None, NULL);
        
		GLContext::makeCurrent(active);
	}

	GLXFBConfig* GLXContext::getFBConfigFromVisual(XVisualInfo* visual)
	{
		int configsCount = 0;
		GLXFBConfig* config = NULL;
		GLXFBConfig* configs = glXChooseFBConfig(m_Display, DefaultScreen(m_Display), NULL, &configsCount);

		if (configs) {
			for (int i = 0; i < configsCount; ++i)
			{
				XVisualInfo* vi = glXGetVisualFromFBConfig(m_Display, configs[i]);

				if (!vi)
					continue;

				if (vi->visualid == visual->visualid) {
					config = &configs[i];
					XFree(vi);
					break;
				}

				XFree(vi);
			}
		}

		return config;
	}
} } }