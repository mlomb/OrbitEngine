#include "OE/Application/Looper.hpp"

#if OE_EMSCRIPTEN
	#include "OE/Platform/Emscripten/Emscripten.hpp"
#endif

namespace OrbitEngine { namespace Application {
	Looper::Looper(Context* context, Loopeable* loopeable)
		: m_Context(context), m_Loopeable(loopeable)
	{
		m_Window = m_Context->getWindow();
		m_Ticker = new Misc::Ticker();
	}

	Looper::~Looper()
	{
		delete m_Ticker;
	}

#if OE_EMSCRIPTEN
	void Looper::emscripten_frame(void* looper)
	{
		(static_cast<Looper*>(looper))->frame();
	}
#endif

	void Looper::loop()
	{
#if OE_EMSCRIPTEN
		emscripten_set_main_loop_arg(emscripten_frame, this, 0, false);
#else
		while (!m_Window || !m_Window->destroyRequested()) {
			frame();

			if (m_Window)
				m_Window->processEvents();
		}
#endif
	}

	void Looper::frame()
	{
		if (m_Context && m_Context->isReady()) {
			if (m_Window)
				m_Context->resizeContext(m_Window->getProperties().resolution);

			m_Context->makeCurrent();
			m_Context->prepare();

			if(m_Loopeable)
				m_Loopeable->render();

			m_Context->present();
			m_Ticker->tick();
		}
	}
	
	Misc::Ticker* Looper::getTicker() const
	{
		return m_Ticker;
	}
} }