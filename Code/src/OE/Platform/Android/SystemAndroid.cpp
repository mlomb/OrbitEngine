#include "SystemAndroid.hpp"

namespace OrbitEngine { namespace System { namespace priv {
	android_app* SystemAndroid::s_AndroidApp = 0;

	android_app* SystemAndroid::GetAndroidApp()
	{
		return s_AndroidApp;
	}

	void SystemAndroid::SetAndroidApp(android_app* app)
	{
		s_AndroidApp = app;
	}
} } }
