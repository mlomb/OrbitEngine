#ifndef X11_HPP
#define X11_HPP

#include "OE/Config.hpp"

#if OE_UNIX
	#include <X11/Xlib.h>
	#include <X11/Xatom.h>
	#include <X11/Xresource.h>
	#include <X11/Xutil.h>
#endif

#endif