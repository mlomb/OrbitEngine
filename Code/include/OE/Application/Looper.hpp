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
		~Looper();

		void stop();
		void loop();
		Misc::Ticker* getTicker() const;

	private:

		void frame();
#if OE_EMSCRIPTEN
		static void emscripten_frame(void* looper);
#endif

		bool m_Running;
		Loopeable* m_Loopeable;
		Misc::Ticker* m_Ticker;
		Window* m_Window;
		Context* m_Context;
	};
} }

#endif