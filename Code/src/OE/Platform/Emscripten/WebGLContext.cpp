#include "OE/Platform/Emscripten/WebGLContext.hpp"

#include "OE/Platform/Emscripten/WindowEmscripten.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	WebGLContext::WebGLContext(WindowEmscripten* window)
		: GLContext(window),
		m_ContextHandle(0)
	{
		EmscriptenWebGLContextAttributes attributes;
		emscripten_webgl_init_context_attributes(&attributes);

		// Request GLES2
		attributes.majorVersion = 2;
		attributes.minorVersion = 0;
		attributes.enableExtensionsByDefault = true;

		attributes.stencil = true;

		m_ContextHandle = emscripten_webgl_create_context(0, &attributes);
		if(m_ContextHandle == 0) {
			OE_LOG_FATAL("An error ocurred creating the WebGL context!");
			return;
		}

		makeCurrent();
		contextInitialized();

		// Enable some extensions that are not enabled by default
		#define WEBGL_ENABLE_EXT(ext) if(!enableExtension(ext)) OE_LOG_WARNING("Couldn't enable the extension " << ext)

		WEBGL_ENABLE_EXT("EXT_color_buffer_float");
	}

	WebGLContext::~WebGLContext()
	{
		if(m_ContextHandle != 0)
			emscripten_webgl_destroy_context(m_ContextHandle);
	}

	bool WebGLContext::enableExtension(const char* extension)
	{
		return emscripten_webgl_enable_extension(m_ContextHandle, extension);
	}

	void WebGLContext::present()
	{

	}

	void WebGLContext::makeCurrent(bool active)
	{
		if(!active)
			OE_LOG_WARNING("A WebGL Context can't be deactivated!");

		if(m_ContextHandle != 0){
			EMSCRIPTEN_RESULT result = emscripten_webgl_make_context_current(m_ContextHandle);
			if(result == EMSCRIPTEN_RESULT_SUCCESS)
				GLContext::makeCurrent(true);
			else
				OE_LOG_WARNING("There was a problem making the WebGL current! " << result);
		}
	}
} } }