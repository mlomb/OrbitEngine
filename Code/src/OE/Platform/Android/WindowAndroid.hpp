#ifndef APPLICATION_WINDOW_ANDROID_HPP
#define APPLICATION_WINDOW_ANDROID_HPP

#include "OE/Application/Window.hpp"
#include "OE/Application/WindowImpl.hpp"
#include "OE/Application/NativeHandlers.hpp"

#include "OE/Platform/Android/Android.hpp"
#include "OE/Application/Context.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	class WindowAndroid : public WindowImpl {
	public:
		WindowAndroid(WindowProperties properties);
		~WindowAndroid();

		WindowNativeHandle getWindowNativeHandle() override;
		DisplayNativeHandle getDisplayNativeHandle() override;
		void processEvents() override;
		void setTitle(const char* title) override;
		bool userInterrupt();

		void onAppCmd(int32_t cmd);

		static void OnAppCmd(struct android_app* app, int32_t cmd);
	private:
		android_app* m_AndroidApp;
	};
} } }

#endif