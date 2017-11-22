#ifndef APPLICATION_CONTEXT_IMPL_HPP
#define APPLICATION_CONTEXT_IMPL_HPP

#include <vector>

#include "OE/Application/Context.hpp"

namespace OrbitEngine { namespace Graphics {
	class States;
	class FrameBuffer;
	class FrameBufferStack;
} }

namespace OrbitEngine {
	namespace Application { namespace priv {
	class WindowImpl;

	class ContextImpl {
	public:
		~ContextImpl();

		virtual void prepare() {};
		virtual void present() = 0;
		virtual void setDefaultBackbuffer() = 0;
		virtual void makeCurrent(bool active = true);
		virtual void resizeContext(Math::Vec2i size);

		virtual bool isReady() { return true; };
		virtual const std::string getName() = 0;
		virtual const RenderAPI getAPI() = 0;

		WindowImpl* getWindowImpl();
		/*
		In the future we should remove this
		because some APIs doesn't have
		global states i.e. Vulkan.
		*/
		Graphics::States* getGlobalStates();

		static ContextImpl* GetCurrent();
		static RenderAPI GetCurrentAPI();

	private:
		Graphics::States* m_States = 0;

	protected:
		ContextImpl(WindowImpl* window);

		Math::Vec2i p_Size;
		WindowImpl* p_Window;

		// Should be called right after the context was created
		virtual void contextInitialized() {};

		// TODO Make this per-context
		static ContextImpl* s_CurrentContext;

	private:
		// Keep track of the framebuffers
		// This is per-context
		friend class Graphics::FrameBuffer;
		std::vector<Graphics::FrameBuffer*> m_FrameBufferStack;
	};
} } }

#endif