#include "EditorWindow.hpp"

namespace OrbitEngine { namespace Editor {
	EditorWindow::EditorWindow()
		: m_Window(nullptr)
	{
	}

	EditorWindow::~EditorWindow()
	{
		destroy();
	}

	void EditorWindow::create()	{
		m_Window = new Window();
		m_Window->setTitle("OrbitEngine Editor");

		CefWindowInfo windowInfo;
		CefBrowserSettings settings;
		windowInfo.SetAsWindowless(m_Window->getWindowNativeHandle());

		m_Client = new CefBrowserClient();
		m_Browser = CefBrowserHost::CreateBrowserSync(windowInfo, m_Client, "", settings, nullptr, nullptr);

		// ready
		m_Browser->GetMainFrame()->LoadURL("https://google.com");
		m_Window->setVisibility(true);
	}

	void EditorWindow::work()
	{
		if (m_Window->destroyRequested())
			destroy();
		else {
			m_Window->processEvents();
			// render
		}
	}

	bool EditorWindow::active()
	{
		return m_Window && m_Browser;
	}

	void EditorWindow::destroy()
	{
		if (active() && m_Browser->GetHost()->TryCloseBrowser()) {
			// -
			m_Client = nullptr;
			m_Browser = nullptr;
			delete m_Window;
			m_Window = nullptr;
		}
	}
} }