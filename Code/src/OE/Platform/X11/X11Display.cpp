#include "OE/Platform/X11/X11Display.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	::Display* X11Display::Create()
	{
		::Display* display = XOpenDisplay(NULL);
		if(display == NULL)
			OE_LOG_FATAL("Couldn't connect to the X-server");
		return display;
	}

	void X11Display::Destroy(::Display* object)
	{
		XCloseDisplay(object);
	}
} } }