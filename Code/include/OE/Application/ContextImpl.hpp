#ifndef APPLICATION_CONTEXT_IMPL_HPP
#define APPLICATION_CONTEXT_IMPL_HPP

#include <vector>

#include "OE/Application/Context.hpp"

namespace OrbitEngine { namespace Graphics {
	class States;
	class FrameBuffer;
} }

namespace OrbitEngine {
	namespace Application { namespace priv {
	class WindowImpl;

	class ContextImpl {
	public:
		virtual ~ContextImpl();

		virtual void prepare() {};
		virtual void present() = 0;
		virtual void makeCurrent(bool active = true);
		virtual void resizeContext(Math::Vec2i size);
		virtual void setSwapInterval(int sync);

		virtual bool isReady() { return true; };
		virtual const std::string getName() = 0;
		virtual const RenderAPI getAPI() = 0;

		Math::Vec2i getSize() { return p_Size; };
		WindowImpl* getWindowImpl();
		/*
		In the future we should remove this
		because some APIs doesn't have
		global states i.e. Vulkan.
		*/
		Graphics::States* getGlobalStates();
		Graphics::FrameBuffer* getDefaultFramebuffer();

		static ContextImpl* GetCurrent();
		static RenderAPI GetCurrentAPI();

	private:
		Graphics::States* m_States;

		friend class Application::Context;
		Application::Context* m_Wrapper;

	protected:
		ContextImpl(WindowImpl* window);

		Math::Vec2i p_Size;
		WindowImpl* p_Window;
		Graphics::FrameBuffer* p_DefaultFramebuffer;

		// Should be called right after the context was created
		virtual void contextInitialized();

		// TODO Make this per-context
		static ContextImpl* s_CurrentContext;

	private:
		// Keep track of the framebuffers
		// This is per-context
		// The pointer to the default FB should not be here
		// as it can be recreated
		friend class Graphics::FrameBuffer;
		std::vector<Graphics::FrameBuffer*> m_FrameBufferStack;
	};
} } }

#endif