#include "OE/Application/Context.hpp"

#include "OE/Config.hpp"
#include "OE/Application/Window.hpp"

#if OE_OPENGL
	#if OE_WINDOWS
		#include "OE/Platform/OpenGL/WGLContext.hpp"
	#elif OE_LINUX
		#include "OE/Platform/OpenGL/GLXContext.hpp"
	#endif
#endif

#if OE_OPENGL_ES
	#if OE_EMSCRIPTEN
		#include "OE/Platform/Emscripten/WebGLContext.hpp"
	#else
		#include "OE/Platform/OpenGL/EGLContext.hpp"
	#endif
#endif

#if OE_D3D
	#include "OE/Platform/Direct3D/D3DContext.hpp"
#endif

#if OE_VULKAN
	#include "OE/Platform/Vulkan/VKContext.hpp"
#endif

namespace OrbitEngine {	namespace Application {
	Context::Context(RenderAPI renderAPI, Window* window, Context* sharedContext)
		: m_Window(window)
	{
		if (sharedContext) {
			OE_ASSERT_MSG(renderAPI == sharedContext->getAPI(), Trying to share context of differents render APIs)
		}

		priv::ContextImpl* sharedContextImpl = sharedContext ? reinterpret_cast<priv::ContextImpl*>(sharedContext->getPimpl()) : 0;
		priv::WindowImpl* windowImpl = window ? window->getPimpl() : 0;

#if OE_WINDOWS
		priv::WindowWindows* winWindow = window ? reinterpret_cast<priv::WindowWindows*>(windowImpl) : 0;
#elif OE_LINUX
		priv::WindowLinux* linuxWindow = window ? reinterpret_cast<priv::WindowLinux*>(windowImpl) : 0;
#elif OE_EMSCRIPTEN
		priv::WindowEmscripten* emscriptenWindow = window ? reinterpret_cast<priv::WindowEmscripten*>(windowImpl) : 0;
#endif

		switch (renderAPI)
		{
#if OE_D3D
		case RenderAPI::DIRECT3D:
			// We can be sure that we are on Windows
			setPimpl(new priv::D3DContext(winWindow, reinterpret_cast<priv::D3DContext*>(sharedContextImpl)));
			break;
#endif
#if OE_OPENGL_ANY
		case RenderAPI::OPENGL:
	#if OE_OPENGL
		#if OE_WINDOWS
			setPimpl(new priv::WGLContext(winWindow, reinterpret_cast<priv::WGLContext*>(sharedContextImpl)));
		#elif OE_LINUX
			setPimpl(new priv::GLXContext(linuxWindow, reinterpret_cast<priv::GLXContext*>(sharedContextImpl)));
		#endif
		break;
	#else
			// Fall to OpenGLES
	#endif
#endif
#if OE_OPENGL_ES
		case RenderAPI::OPENGL_ES:
	#if OE_EMSCRIPTEN
			if(sharedContext)
				OE_LOG_ERROR("Sharing context is not possible in WebGL!");
			setPimpl(new priv::WebGLContext(emscriptenWindow));
	#else
			setPimpl(new priv::EGLContext(windowImpl, reinterpret_cast<priv::EGLContext*>(sharedContextImpl)));
	#endif
			break;
#endif
#if OE_VULKAN
		case RenderAPI::VULKAN:
			setPimpl(new priv::VKContext(windowImpl, reinterpret_cast<priv::VKContext*>(sharedContextImpl)));
			break;
#endif
		default:
			OE_LOG_FATAL("Invalid render API.")
			break;
		}
	}

	Context::~Context()
	{
		//
	}

	void Context::prepare()
	{
		p_Pimpl->prepare();
	}

	void Context::present()
	{
		p_Pimpl->present();
	}

	void Context::makeCurrent()
	{
		p_Pimpl->makeCurrent();
	}

	void Context::resizeContext(Math::Vec2i size)
	{
		p_Pimpl->resizeContext(size);
	}

	bool Context::isReady()
	{
		return p_Pimpl->isReady();
	}

	Window* Context::getWindow()
	{
		return m_Window;
	}

	const std::string Context::getName()
	{
		return p_Pimpl->getName();
	}

	const RenderAPI Context::getAPI()
	{
		return p_Pimpl->getAPI();
	}

	Context* Context::GetCurrent()
	{
		// Somehow wrap the ContextImpl* into a Context*
		//return priv::ContextImpl::GetCurrent();
		return 0;
	}

	RenderAPI Context::GetCurrentAPI()
	{
		return priv::ContextImpl::GetCurrentAPI();
	}
} }