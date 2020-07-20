#include "EditorWindow.hpp"

#include <OE/Misc/Log.hpp>

namespace OrbitEngine { namespace Editor {
	EditorWindow::EditorWindow()
		: m_Window(nullptr), m_Context(nullptr)
	{
	}

	EditorWindow::~EditorWindow()
	{
		OE_ASSERT_MSG(!active(), "The editor window is not ready to be destroyed.");
	}

	void EditorWindow::create() {
		m_Window = new Window();
		m_Window->setTitle("OrbitEngine Editor");

		m_Context = new Context(RenderAPI::OPENGL, m_Window);

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
		if (!active())
			return;

		if (m_Window->destroyRequested()) {
			// request close
			if (m_Browser->GetHost()->TryCloseBrowser()) {
				destroy();
			}
		}
		else {
			m_Window->processEvents();

			// render
			m_Context->makeCurrent();
			m_Context->present();
		}
	}

	bool EditorWindow::active()
	{
		return m_Window && m_Browser;
	}

	void EditorWindow::destroy()
	{
		m_Client = nullptr;
		m_Browser = nullptr;
		delete m_Context;
		delete m_Window;
		m_Context = nullptr;
		m_Window = nullptr;
	}
} }