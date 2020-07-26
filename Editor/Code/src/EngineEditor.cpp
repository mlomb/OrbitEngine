#include "EngineEditor.hpp"

namespace OrbitEngine { namespace Editor {
	EngineEditor::EngineEditor()
	{
		m_DummyWindow = new Window();
		m_Context = new Context(OPENGL, m_DummyWindow);
	}

	EngineEditor::~EngineEditor()
	{
		OE_ASSERT(m_Windows.size() == 0);
		delete m_Context;
		delete m_DummyWindow;
	}

	void EngineEditor::open(const std::string& url)
	{
		EditorWindow* w = new EditorWindow();
		w->create(m_Context, url);
		m_Windows.push_back(w);
	}

	void EngineEditor::run()
	{
		open("http://127.0.0.1:8080");
		open("http://127.0.0.1:8080");
		open("http://127.0.0.1:8080");

		while (m_Windows.size() > 0) {
			CefDoMessageLoopWork();
			m_Ticker.tick();

			for (auto it = m_Windows.begin(); it != m_Windows.end();) {
				EditorWindow* w = *it;
				if (w->active()) {
					w->work();
					it++;
				}
				else {
					it = m_Windows.erase(it);
					delete w;
				}
			}

			for (EditorWindow* w : m_Windows)
				w->present();

			Sleep(6);
		}
	}
} }