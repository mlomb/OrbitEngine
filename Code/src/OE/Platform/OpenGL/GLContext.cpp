#include "OE/Platform/OpenGL/GLContext.hpp"

#include <iterator>

#include "OE/Platform/OpenGL/GLFrameBuffer.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	void GLContext::prepare()
	{
		p_DefaultFramebuffer->clear();
		p_DefaultFramebuffer->m_Width = p_Size.w;
		p_DefaultFramebuffer->m_Height = p_Size.h;
	}

	const std::string GLContext::getName()
	{
		return std::string("OpenGL") + (p_ContextInfo.ES ? " ES" : "");
	}

	const RenderAPI GLContext::getAPI()
	{
#if OE_OPENGL_ES
		if (p_ContextInfo.ES)
			return RenderAPI::OPENGL_ES;
#endif
		return RenderAPI::OPENGL;
	}

	GLContext::GLContext(WindowImpl* window)
		: ContextImpl(window)
	{
	}

	void GLContext::contextInitialized()
	{
		// Just to be sure
		makeCurrent(true);

		p_DefaultFramebuffer = new Graphics::GLFrameBuffer((GLuint)0);
		Graphics::FrameBuffer::Prepare();

		ContextImpl::contextInitialized();

		p_ContextInfo.vendor = glGetString(GL_VENDOR) ? std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))) : "";
		p_ContextInfo.renderer = glGetString(GL_RENDERER) ? std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))) : "";
		p_ContextInfo.version = glGetString(GL_VERSION) ? std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))) : "";

		// Detect if we are in OpenGLES
		size_t versionPos = 0;
		if ((versionPos = p_ContextInfo.version.find("OpenGL ES")) != std::string::npos) {
			versionPos += strlen("OpenGL ES");
			p_ContextInfo.ES = true;
		}
		else if ((versionPos = p_ContextInfo.version.find("WebGL")) != std::string::npos) {
			versionPos += strlen("WebGL");
			p_ContextInfo.ES = true;
		}

		// Extract version
		if (p_ContextInfo.ES) {
			char skipDot;
			std::istringstream iss(p_ContextInfo.version.substr(versionPos));
			iss >> p_ContextInfo.major >> skipDot >> p_ContextInfo.minor;
		} else {
			// OpenGL way
			glGetIntegerv(GL_MAJOR_VERSION, &p_ContextInfo.major);
			glGetIntegerv(GL_MINOR_VERSION, &p_ContextInfo.minor);
		}

		std::string extensions(glGetString(GL_EXTENSIONS) ? reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)) : "");
		std::istringstream iss(extensions);
		std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(p_ContextInfo.extensions));

		OE_LOG_DEBUG("-- OpenGL" << (p_ContextInfo.ES ? " ES" : "") << " Context Info --");
		OE_LOG_DEBUG("Vendor: " << p_ContextInfo.vendor);
		OE_LOG_DEBUG("Renderer: " << p_ContextInfo.renderer);
		OE_LOG_DEBUG("Version: " << p_ContextInfo.version);
		OE_LOG_DEBUG("Major.Minor: " << p_ContextInfo.major << "." << p_ContextInfo.minor);
		OE_LOG_DEBUG("Extensions count: " << p_ContextInfo.extensions.size());
		
		OE_LOG_DEBUG("Shading version: " << std::string(glGetString(GL_SHADING_LANGUAGE_VERSION) ? reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)) : ""));
	}

	void GLContext::makeCurrent(bool active)
	{
		ContextImpl::makeCurrent(active);
	}
} } }