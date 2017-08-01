#ifndef SYSTEM_SYSTEM_ANDROID_HPP
#define SYSTEM_SYSTEM_ANDROID_HPP

#include "OE/System/SystemImpl.hpp"
#include "OE/Platform/Android/Android.hpp"

namespace OrbitEngine { namespace System { namespace priv {

	class SystemAndroid : public SystemImpl {
	public:
		~SystemAndroid() {};

		static android_app* GetAndroidApp();
		static void SetAndroidApp(android_app* app);

	protected:
		SystemAndroid() {};

	private:
		static android_app* s_AndroidApp;
	};
} } }

#endif