#include "WindowAndroid.hpp"

#include "OE/Math/Vec2.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Platform/Android/SystemAndroid.hpp"

#include "OE/Platform/OpenGL/EGLContext.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	WindowAndroid::WindowAndroid(WindowProperties properties)
		: WindowImpl(properties)
	{
		m_AndroidApp = ::OrbitEngine::System::priv::SystemAndroid::GetAndroidApp();

		m_AndroidApp->userData = this;
		m_AndroidApp->onAppCmd = WindowAndroid::OnAppCmd;

		p_Properties.resolution = Math::Vec2i(1, 1);
	}

	void WindowAndroid::processEvents() {
		int ident;
		int events;
		struct android_poll_source* source;

		while ((ident=ALooper_pollAll(0, NULL, &events,(void**)&source)) >= 0) {
			// Process this event.
			if (source != NULL) {
				source->process(m_AndroidApp, source);
			}

			// Check if we are exiting.
			if (m_AndroidApp->destroyRequested != 0) {
				requestDestroy();
				return;
			}
		}
		
		WindowImpl::processEvents();
	}

	void WindowAndroid::onAppCmd(int32_t cmd){
		switch (cmd) {
		case APP_CMD_SAVE_STATE:
			break;
		case APP_CMD_INIT_WINDOW:
			if (m_AndroidApp->window != NULL) {
	            int width = ANativeWindow_getWidth(m_AndroidApp->window);
	            int height = ANativeWindow_getHeight(m_AndroidApp->window);
				p_Properties.resolution = Math::Vec2i(width, height);
				if(p_Context)
					reinterpret_cast<EGLContext*>(p_Context)->createSurface(m_AndroidApp->window);
			}
			break;
		case APP_CMD_TERM_WINDOW:
			reinterpret_cast<EGLContext*>(p_Context)->destroySurface();
			break;
		case APP_CMD_LOST_FOCUS:
			break;
		}
	}

	void WindowAndroid::OnAppCmd(struct android_app* app, int32_t cmd){
		// Forward to instance
		reinterpret_cast<WindowAndroid*>(app->userData)->onAppCmd(cmd);
	}

	WindowAndroid::~WindowAndroid()
	{

	}

	WindowNativeHandle WindowAndroid::getWindowNativeHandle()
	{
		return m_AndroidApp->window;
	}

	DisplayNativeHandle WindowAndroid::getDisplayNativeHandle()
	{
		/* EGL_DEFAULT_DISPLAY */
		return 0;
	}

	void WindowAndroid::setTitle(const char* title)
	{
	}
} } }
