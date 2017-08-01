#ifndef APPLICATION_GLCONTEXT_HPP
#define APPLICATION_GLCONTEXT_HPP

#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Application/ContextImpl.hpp"

namespace OrbitEngine { namespace Application { namespace priv {

	class GLContext : public ContextImpl {
	public:
		~GLContext() {};

		void prepare() override;
		virtual void present() override {};
		virtual void makeCurrent(bool active) override;
		virtual const RenderAPI getAPI() override = 0;
		virtual void setDefaultBackbuffer() override;

		const char* getName() override { return "OpenGL"; }

		static GLContext* GetCurrent() { return (GLContext*)ContextImpl::GetCurrent(); };
	protected:
		GLContext(WindowImpl* window) : ContextImpl(window) {};

	private:

		void setViewport();
	};
} } }

#endif