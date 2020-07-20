#ifndef EDITOR_WINDOW_HPP
#define EDITOR_WINDOW_HPP

#include "Cef.hpp"
#include "CefBrowserClient.hpp"

#include <OE/Application/Window.hpp>


namespace OrbitEngine { namespace Editor {
	using namespace OrbitEngine;
	using namespace OrbitEngine::Application;

	/**
		@brief The base editor window with a CEF browser and rendering context
	*/
	class EditorWindow {
	public:
		EditorWindow();
		~EditorWindow();

		void create();
		void work();
		bool active();

	private:
		Window* m_Window;

		CefRefPtr<CefBrowserClient> m_Client;
		CefRefPtr<CefBrowser> m_Browser;

		void destroy();
	};
} };

#endif