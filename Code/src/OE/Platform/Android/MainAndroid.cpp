#include "OE/Config.hpp"

extern int main(int argc, char* argv[]);

#include "OE/Platform/Android/Android.hpp"
#include "OE/Platform/Android/SystemAndroid.hpp"

void android_main(struct android_app* app) {
	app_dummy();

	OrbitEngine::System::priv::SystemAndroid::SetAndroidApp(app);
	
	__android_log_print(ANDROID_LOG_INFO, "TestApp", "Pointer %p", app);
	__android_log_print(ANDROID_LOG_INFO, "TestApp", "%s", "App Started!!!!!");
	
	main(0, NULL);

	ANativeActivity_finish(app->activity);
}