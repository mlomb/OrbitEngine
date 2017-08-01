#ifndef D3D_HPP
#define D3D_HPP

#include "OE/Config.hpp"

#if OE_D3D
	#include <D3D11.h>
	#include "D3DCompiler.h"

	#define OE_D3D_RELEASE(x) \
	{ \
		if(x) { \
			x->Release(); \
			x = 0; \
		} \
	}
#endif

#endif