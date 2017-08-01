#include "OE/Platform/OpenGL/OpenGLEXT.hpp"

#if OE_OPENGL

#if OE_UNIX
	#include <dlfcn.h>
#endif

#define GLE(ret, name, ...) name##proc * gl##name;
	OE_OPENGL_EXTENSIONS
	OE_OPENGL_EXTENSIONS_OS
#undef GLE

bool OpenGLEXTENSIONS_Load() {
#if OE_WINDOWS
	HINSTANCE dll = LoadLibraryA("opengl32.dll");
	typedef PROC WINAPI wglGetProcAddressproc(LPCSTR lpszProc);
	if (!dll) {
		OE_LOG_FATAL("Couldn't load opengl32.dll")
		return false;
	}
	wglGetProcAddressproc* wglGetProcAddress = (wglGetProcAddressproc*)GetProcAddress(dll, "wglGetProcAddress");
	#define GetGLProcAddress(x) wglGetProcAddress(#x)
#elif OE_LINUX || OE_ANDROID
	#if OE_LINUX
	const char* glLibName =	"libGL.so";
	#elif OE_ANDROID
	const char* glLibName = "libGLESv2.so";
	#endif
	void*  libGL = dlopen(glLibName, RTLD_LAZY);
	if (!libGL) {
		OE_LOG_FATAL("Couldn't load " << glLibName)
		return false;
	}
	#define GetGLProcAddress(x) dlsym(libGL, #x)
#else
	#error "Platform unsupported by the GL loader"
#endif

#define GLE(ret, name, ...) \
	gl##name = (name##proc *) GetGLProcAddress(gl##name); \
	/* OE_LOG_DEBUG("Loaded function gl" << #name << " at " << gl##name) */ \
	if(!gl##name){ \
		OE_LOG_ERROR("Couldn't load GL function gl" << #name) \
		/* return false; */ \
	}
	OE_OPENGL_EXTENSIONS
	OE_OPENGL_EXTENSIONS_OS
#undef GLE
	return true;
}

#endif