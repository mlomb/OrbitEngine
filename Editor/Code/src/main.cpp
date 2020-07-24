#include <stdio.h>

#include "Cef.hpp"
#include "EditorWindow.hpp"

#include <OE/Misc/Log.hpp>
#include <OE/Misc/Ticker.hpp>

int main(int argc, char* argv[]) {
	CefEnableHighDPISupport();

#if OE_WINDOWS
	CefMainArgs args(GetModuleHandle(NULL));
#else
	CefMainArgs args(argc, argv);
#endif

	int exit_code = CefExecuteProcess(args, nullptr, nullptr);
	if (exit_code >= 0)
		return exit_code;
	
	OE_LOG_DEBUG("CEF " << CEF_VERSION);

	CefSettings settings;
	settings.windowless_rendering_enabled = true;
	settings.multi_threaded_message_loop = true;
#if OE_DEBUG
	settings.remote_debugging_port = 9222;
#endif
	
	/*class App : public CefApp {
	public:
		void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override {
			command_line->AppendSwitchWithValue("use-angle", "d3d11");
		}

	private:
		IMPLEMENT_REFCOUNTING(App);
	};*/

	if (!CefInitialize(args, settings, nullptr, NULL)) {
		OE_LOG_FATAL_SHOWBOX_AND_EXIT("Failed to initialize CEF")
	}
	
	using namespace OrbitEngine;
	using namespace OrbitEngine::Editor;
	using namespace OrbitEngine::Application;
	using namespace OrbitEngine::Misc;

	Ticker ticker;
	std::vector<EditorWindow*> windows;

	windows.push_back(new EditorWindow());
	windows.push_back(new EditorWindow());
	for (EditorWindow* w : windows)
		w->create();

	while (windows.size() > 0) {
		ticker.tick();

		for (auto it = windows.begin(); it != windows.end();) {
			EditorWindow* w = *it;
			if (w->active()) {
				w->work();
				it++;
			}
			else {
				it = windows.erase(it);
				delete w;
			}
		}

		for (EditorWindow* w : windows)
			w->present();
	}

	CefShutdown();

	return 0;
}
