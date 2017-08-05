#ifndef APPLICATION_GLCONTEXT_HPP
#define APPLICATION_GLCONTEXT_HPP

#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Application/ContextImpl.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	struct GLContextInfo {
		GLint major = -1, minor = -1;
		bool ES = false;
		std::string vendor;
		std::string renderer;
		std::string version;
		std::vector<std::string> extensions;
	};

	class GLContext : public ContextImpl {
	public:
		~GLContext() {};

		void prepare() override;
		virtual void present() override {};
		virtual void makeCurrent(bool active) override;
		virtual const RenderAPI getAPI() override = 0;
		virtual void setDefaultBackbuffer() override;

		const char* getName() override { return "OpenGL"; }
		const GLContextInfo& getInfo() { return p_ContextInfo; }

		static GLContext* GetCurrent() { return (GLContext*)ContextImpl::GetCurrent(); };
	protected:
		GLContext(WindowImpl* window) : ContextImpl(window) {};

		void contextInitialized() override;

		GLContextInfo p_ContextInfo;

	private:

		void setViewport();
	};
} } }

#endif