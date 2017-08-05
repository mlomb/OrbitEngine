#ifndef APPLICATION_CONTEXT_HPP
#define APPLICATION_CONTEXT_HPP

#include "OE/Config.hpp"
#include "OE/Misc/Pimpl.hpp"
#include "OE/Math/Vec2.hpp"

namespace OrbitEngine {
	enum RenderAPI
	{
		NONE = 0, 
#if OE_OPENGL_ANY
		/*
		 If OpenGL and OpenGLES are avaialable this will target OpenGL
		 If only OpenGLES is available there is no difference to OPENGL_ES
		*/
		OPENGL,
#endif
#if OE_OPENGL_ES
		/*
		You can force OpenGLES if available
		*/
		OPENGL_ES,
#endif
#if OE_VULKAN
		VULKAN,
#endif
#if OE_D3D
		// For now, D3D11
		DIRECT3D,
#endif
	};
	
	namespace Application {
		namespace priv {
			class ContextImpl;
		}
		class Window;

		/* Should Context be in the Graphics namespace? */
		class Context : public Misc::Pimpl<priv::ContextImpl> {
		public:
			Context(RenderAPI renderAPI, Window* window = 0, Context* sharedContext = 0);
			~Context();

			bool isReady();
			void prepare();
			void present();
			void makeCurrent();

			void resizeContext(Math::Vec2i size);

			const char* getName();
			const RenderAPI getAPI();

		public:
			static Context* GetCurrent();
			static RenderAPI GetCurrentAPI();
		};
} }

#endif