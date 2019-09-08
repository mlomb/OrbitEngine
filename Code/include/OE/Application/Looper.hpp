#ifndef APPLICATION_LOOPER_HPP
#define APPLICATION_LOOPER_HPP

#include "OE/Application/Context.hpp"
#include "OE/Application/Window.hpp"
#include "OE/Application/Loopeable.hpp"

#include "OE/Misc/Ticker.hpp"

namespace OrbitEngine {	namespace Application {

	class Looper {
	public:
		Looper(Context* context, Loopeable* loopeable = 0);
		virtual ~Looper();

		void setLoopeable(Loopeable* loopeable);

		virtual void stop();
		virtual void loop();
		const Misc::Ticker& getTicker() const;

	protected:

		virtual void frame();
#if OE_EMSCRIPTEN
		static void emscripten_frame(void* looper);
#endif

		bool m_Running;
		bool m_Initialized;
		Misc::Ticker m_Ticker;
		Loopeable* m_Loopeable;
		Window* m_Window;
		Context* m_Context;
	};
} }

#endif