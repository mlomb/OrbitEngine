#include <stdio.h>

#include "EditorWindow.hpp"

#include <OE/Misc/Log.hpp>

int main(int argc, char* argv[]) {
	CefEnableHighDPISupport();

#if _WIN32
	CefMainArgs args(GetModuleHandle(NULL));
#else
	CefMainArgs args(argc, argv);
#endif

	int exit_code = CefExecuteProcess(args, nullptr, nullptr);
	if (exit_code >= 0)
		return exit_code;

	CefSettings settings;
	//settings.multi_threaded_message_loop = true; // TODO: investigate this possibility
	settings.remote_debugging_port = 9222;

	if (!CefInitialize(args, settings, nullptr, NULL)) {
		OE_LOG_FATAL_SHOWBOX_AND_EXIT("Failed to initialize CEF")
	}

	using namespace OrbitEngine;
	using namespace OrbitEngine::Editor;
	using namespace OrbitEngine::Application;

	EditorWindow* ew1 = new EditorWindow();
	ew1->create();

	EditorWindow* ew2 = new EditorWindow();
	ew2->create();

	bool d1, d2;
	do {
		d1 = ew1->active();
		d2 = ew2->active();

		if(d1) ew1->work();
		if(d2) ew2->work();

		CefDoMessageLoopWork();
	} while (d1 || d2);

	delete ew1;
	delete ew2;
	/*
	Window* window = new Window();
	window->setTitle("OrbitEngine Editor");

	Context* context = new Context(RenderAPI::OPENGL, window);

	// ready
	window->setVisibility(true);

	while (!window->destroyRequested()) {
		window->processEvents();
	}
	*/

	CefShutdown();

	return 0;
}
