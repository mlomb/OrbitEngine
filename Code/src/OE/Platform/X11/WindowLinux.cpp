#include "WindowLinux.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	WindowLinux::WindowLinux(WindowProperties properties)
		: WindowImpl(properties)
	{
		// Open connection
		m_Display = X11Display::Instance()->GetAndReference();
		int XScreen = DefaultScreen(m_Display);
		::Window XRoot = RootWindow(m_Display, XScreen);

		// Define the attributes
		XSetWindowAttributes attrs;
		attrs.background_pixmap = None;
		attrs.border_pixmap = None;
		attrs.border_pixel = 0;
		attrs.event_mask = StructureNotifyMask | EnterWindowMask | LeaveWindowMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | OwnerGrabButtonMask | KeyPressMask | KeyReleaseMask | FocusChangeMask | PointerMotionMask;

#if OE_OPENGL && OE_LINUX
		/* Get A visual */
		static int attribs[] = {
			GLX_RENDER_TYPE, GLX_RGBA_BIT,
			GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
			GLX_DOUBLEBUFFER, True,
			GLX_RED_SIZE, 8,
			GLX_GREEN_SIZE, 8,
			GLX_BLUE_SIZE, 8,
			GLX_ALPHA_SIZE, 8,
			GLX_DEPTH_SIZE, 16,
			None
		};

		int nconfigs;
		XVisualInfo* visual = NULL;
		GLXFBConfig* config = NULL;
		GLXFBConfig* configs = glXChooseFBConfig(m_Display, XScreen, attribs, &nconfigs);

		for(int i = 0; i < nconfigs; i++) {
			visual = (XVisualInfo*) glXGetVisualFromFBConfig(m_Display, configs[i]);
			if(visual){
				config = &configs[i];
				break;
			}
		}

		if(configs)
			XFree(configs);

		if(!visual || !config) {
			OE_LOG_FATAL("Not matching FB config found!")
			return;
		}

		m_VisualInfo = visual;

		attrs.colormap = XCreateColormap(m_Display, XRoot, m_VisualInfo->visual, AllocNone);
#else
		m_VisualInfo = NULL;
		OE_LOG_FATAL("Platform unsupported?")
#endif

		// Create the window
		m_Window = XCreateWindow(m_Display, XRoot, 0, 0, 800, 600, 0, m_VisualInfo->depth, InputOutput, m_VisualInfo->visual, CWBorderPixel | CWColormap | CWEventMask, &attrs);
		if(!m_Window){
			OE_LOG_FATAL("Couldn't create the window.")
			return;
		}

		setTitle(p_Properties.title.c_str());

		loadAtoms();
		
		// Show the window
		XMapWindow(m_Display, m_Window);
	}

	void WindowLinux::processEvents() {
		XEvent event;

		while (XPending(m_Display))
		{
			XNextEvent(m_Display, &event);

			switch (event.type)
			{
				case ClientMessage:
					if (event.xclient.data.l[0] == m_ATOM_WM_DELETE_WINDOW){
						requestDestroy();
						return;
					}
				break;
				case ConfigureNotify:
				{
					XConfigureEvent xce = event.xconfigure;
					p_Properties.resolution = Math::Vec2i(xce.width, xce.height);
				}
				break;
				case KeyPress:
				case KeyRelease:
				{
					Key key = TranslateKey(event.xkey.keycode);
					//OE_LOG_DEBUG("keycode: " << event.xkey.keycode << " -> " << (char)event.xkey.keycode << "  Key: " << key << " -> " << (char)key);
					p_InputManager->onInputKey(key, event.type == KeyPress);
				}
				break;
				case ButtonPress:
				case ButtonRelease:
					//OE_LOG_DEBUG("button: " << event.xbutton.button);
					p_InputManager->onInputMouseButton(Button(event.xbutton.button - 1), event.type == ButtonPress);
				break;
				case FocusIn:
				case FocusOut:
					p_InputManager->onInputFocus(event.type == FocusIn);
				break;
				case MotionNotify:
					p_InputManager->onInputMouseMove(event.xmotion.x, event.xmotion.y);
				break;
			}
		}

		bool shouldBeGrabbed = p_InputManager->hasFocus() && p_InputManager->m_CursorMode == CursorMode::GRABBED;
		if(shouldBeGrabbed && !m_Grabbed){
			XGrabPointer(m_Display, m_Window, True, 0, GrabModeAsync, GrabModeAsync, m_Window, None, CurrentTime);
			m_Grabbed = true;
		}
		else if(!shouldBeGrabbed && m_Grabbed){
			XUngrabPointer(m_Display, CurrentTime);
			m_Grabbed = false;
		}
		if(shouldBeGrabbed){
			Math::Vec2i center = p_Properties.resolution / 2;
			p_InputManager->m_CursorPos = center;
			XWarpPointer(m_Display, None, m_Window, 0, 0, 0, 0, center.x, center.y);
			// TODO Not working...
		}

		WindowImpl::processEvents();
	}

	void WindowLinux::loadAtoms(){
		#define LOAD_ATOM(X) \
		m_ATOM_##X = XInternAtom(m_Display, #X, false); \
		XSetWMProtocols(m_Display, m_Window, &m_ATOM_##X, true);

		LOAD_ATOM(WM_DELETE_WINDOW)
	}

	WindowLinux::~WindowLinux()
	{
		// Close
		if(m_VisualInfo)
        	XFree(m_VisualInfo);
		if (m_Display){
			XDestroyWindow(m_Display, m_Window);
			X11Display::Instance()->DeReference();
			m_Display = NULL;
		}
	}

	WindowNativeHandle WindowLinux::getWindowNativeHandle(){
		return m_Window;
	}

	DisplayNativeHandle WindowLinux::getDisplayNativeHandle(){
		return m_Display;
	}

	XVisualInfo* WindowLinux::getVisualInfo(){
		return m_VisualInfo;
	}

	void WindowLinux::setTitle(const char* title)
	{
		// Set the window properties
		XTextProperty titleTextProp;
		titleTextProp.value = (unsigned char*)title;
		titleTextProp.encoding = XA_STRING;
		titleTextProp.format = 8;
		titleTextProp.nitems = strlen(title);

		XSetWMProperties(m_Display, m_Window, &titleTextProp, &titleTextProp, NULL, 0, NULL, NULL, NULL);
	}

	Key WindowLinux::TranslateKey(int key) {
		// TODO
		// Please do this right
		// Ultra-Temporary
		switch(key){
			case 22: return Key::BACKSPACE;
			case 9: return Key::ESCAPE;
			case 25: return Key::W;
			case 38: return Key::A;
			case 39: return Key::S;
			case 40: return Key::D;
			default: return (Key)0;
		}
	}
} } }
