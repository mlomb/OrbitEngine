#include <stdio.h>

#include "Cef.hpp"
#include "CefController.hpp"

#include "EngineEditor.hpp"

#include <OE/Misc/Log.hpp>

int main(int argc, char* argv[]) {
	using namespace OrbitEngine;
	using namespace OrbitEngine::Editor;

	CefEnableHighDPISupport();

	#if OE_WINDOWS
	CefMainArgs args(GetModuleHandle(NULL));
	#else
	CefMainArgs args(argc, argv);
	#endif

	CefRefPtr<CefController> controller = new CefController();

	int exit_code = CefExecuteProcess(args, controller, nullptr);
	if (exit_code >= 0)
		return exit_code;

	OE_LOG_DEBUG("CEF " << CEF_VERSION);

	CefSettings settings;
	settings.windowless_rendering_enabled = true;
	settings.multi_threaded_message_loop = false;
#if OE_DEBUG
	settings.remote_debugging_port = 9222;
#endif

	if (!CefInitialize(args, settings, controller, NULL)) {
		OE_LOG_FATAL_SHOWBOX_AND_EXIT("Failed to initialize CEF");
		// unreachable
	}

	{
		EngineEditor editor;
		editor.run();
	}

	CefShutdown();

	return 0;
}
