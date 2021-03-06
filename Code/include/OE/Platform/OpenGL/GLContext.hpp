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

		bool ubo_support;
	};

	class GLContext : public ContextImpl {
	public:
		virtual ~GLContext() {};

		void prepare() override;
		virtual void present() override {};
		virtual void makeCurrent(bool active) override;

		const std::string getName() override;
		const RenderAPI getAPI() override;
		const GLContextInfo& getInfo() { return p_ContextInfo; }
		bool isExtensionAvailable(const std::string& ext) const;

		static GLContext* GetCurrent() { return (GLContext*)ContextImpl::GetCurrent(); };
	protected:
		GLContext(WindowImpl* window);

		void contextInitialized() override;

		GLContextInfo p_ContextInfo;
	};
} } }

#endif