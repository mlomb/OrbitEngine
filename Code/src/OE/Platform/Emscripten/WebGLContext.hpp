#ifndef APPLICATION_EGLCONTEXT_HPP
#define APPLICATION_EGLCONTEXT_HPP

#include "OE/Platform/OpenGL/GLContext.hpp"
#include "OE/Platform/OpenGL/OpenGL.hpp"

#include "OE/Platform/Emscripten/Emscripten.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	class WindowEmscripten;

	class WebGLContext : public GLContext {
	public:
		WebGLContext(WindowEmscripten* window);
		~WebGLContext();

		void present() override;
		void makeCurrent(bool active = true) override;

		bool enableExtension(const char* extension);

	private:
		EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_ContextHandle;
	};
} } }

#endif