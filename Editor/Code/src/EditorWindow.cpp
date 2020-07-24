#include "EditorWindow.hpp"

#include <iostream>

#include <OE/Misc/Log.hpp>
#include <OE/Graphics/API/FrameBuffer.hpp>

#if OE_WINDOWS
#include <OE/Platform/Windows/WindowWindows.hpp>
#endif

namespace OrbitEngine { namespace Editor {
	EditorWindow::EditorWindow()
		: m_Window(nullptr),
		m_Context(nullptr),
		m_BlitBrowserTexture(nullptr),
		m_SpriteBatcher(nullptr),
		m_LastSize(0, 0)
	{
	}

	EditorWindow::~EditorWindow()
	{
		destroy();
	}

	void EditorWindow::create() {
		m_Window = new Window();
		m_Window->setTitle("OrbitEngine Editor");

#if OE_WINDOWS
		priv::WindowWindows* winWindow = static_cast<priv::WindowWindows*>(m_Window->getPimpl());

		// HACK:
		// WndProc blocks execution while resizing, so we have to rely
		// on the WM_SIZING message to draw
		winWindow->setResizingCallback([&]() {
			work();
			present();
		});
#endif

		m_Context = new Context(RenderAPI::OPENGL, m_Window);
		FrameBuffer::GetCurrent()->setClearColor(Vec4f(0.08f, 0.08f, 0.08f, 1.0f));

		CefWindowInfo windowInfo = {};
		windowInfo.SetAsWindowless(nullptr);
#if OE_WINDOWS && OE_D3D
		// TODO: implement, seems to be broken in CEF 84

		// only supported on Windows with D3D11
		// windowInfo.shared_texture_enabled = 1;
		// windowInfo.windowless_rendering_enabled = 1;
#endif

		CefBrowserSettings settings = {};
		settings.windowless_frame_rate = 60;

		m_CefView = CefRefPtr<CefView>(new CefView());

		CefBrowserHost::CreateBrowser(windowInfo, m_CefView, "http://127.0.0.1:8080", settings, nullptr, nullptr);

		init();

		m_Window->setVisibility(true);
	}

	void EditorWindow::work()
	{
		if (!active())
			return;

		if (m_Window->destroyRequested()) {
			destroy();
		}
		else {
			m_Window->processEvents();

			Vec2i window_size = m_Window->getSize();
			if (window_size != m_LastSize) {
				m_LastSize = window_size;
				m_Context->resizeContext(m_Window->getSize());
				m_CefView->resize(window_size.x, window_size.y);
			}

			m_Context->makeCurrent();
			m_Context->prepare();

			m_CefView->getViewBuffer().upload(m_BlitBrowserTexture);

			render();
		}
	}

	bool EditorWindow::active() const
	{
		return m_Window || m_CefView;
	}

	void EditorWindow::present()
	{
		if(m_Context)
			m_Context->present();
	}

	void EditorWindow::destroy()
	{
		if (m_CefView) {
			m_CefView->close();
			m_CefView = nullptr;
		}
		deinit();
		delete m_Context;
		delete m_Window;
		m_LastSize = Vec2i(0, 0);
		m_Context = nullptr;
		m_Window = nullptr;
	}

	void EditorWindow::init()
	{
		m_SpriteBatcher = new SpriteBatcher(10); // no need for many quads
	}

	void EditorWindow::render()
	{
		using namespace OrbitEngine::Math;

		auto window_size = m_Window->getSize();

		m_SpriteBatcher->setPVMatrices(Mat4::Orthographic(0.0f, (float)window_size.x, (float)window_size.y, 0.0f, -1.0f, 1.0f));
		
		m_SpriteBatcher->begin();

		auto size = m_BlitBrowserTexture ? Vec2f(m_BlitBrowserTexture->getProperties().width, m_BlitBrowserTexture->getProperties().height) : Vec2f(100, 100);

		/*
		m_SpriteBatcher->bindColor(Color4f(1.0, 0.0, 1.0, 0.5));
		m_SpriteBatcher->bindTexture(NULL);
		m_SpriteBatcher->rect(Vec2f(0, 0), Vec2f(size.x, size.y));
		*/

		m_SpriteBatcher->bindColor(Color4f(1.0, 1.0, 1.0, 1.0));
		m_SpriteBatcher->bindTexture(m_BlitBrowserTexture);
		m_SpriteBatcher->rect(Vec2f(0, size.y), Vec2f(size.x, -size.y));

		m_SpriteBatcher->bindColor(Color4f(1.0, 0.0, 1.0, 1.0));
		m_SpriteBatcher->bindTexture(NULL);
		m_SpriteBatcher->rect(Vec2f(200, 200), Vec2f(100, 100));

		m_SpriteBatcher->end();
	}

	void EditorWindow::deinit()
	{
		if (m_BlitBrowserTexture) {
			delete m_BlitBrowserTexture;
			m_BlitBrowserTexture = nullptr;
		}
		delete m_SpriteBatcher;
		m_SpriteBatcher = nullptr;
	}

} }