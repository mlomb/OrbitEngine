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
		m_SpriteBatcher(nullptr),
		m_LastSize(0, 0)
	{
	}

	EditorWindow::~EditorWindow()
	{
		destroy();
	}

	void EditorWindow::create(Context* main_context, const std::string& initial_url) {
		m_Window = new Window();
		m_Window->setTitle("OrbitEngine Editor");
		m_Window->setVisibility(true);

		#if OE_WINDOWS
		priv::WindowWindows* winWindow = static_cast<priv::WindowWindows*>(m_Window->getPimpl());

		// HACK: WndProc blocks execution while resizing, so we have to rely
		//       on the WM_SIZING message to draw
		winWindow->setResizingCallback([&]() {
			work();
			present();
		});
		#endif

		main_context->makeCurrent();
		m_Context = new Context(RenderAPI::OPENGL, m_Window, main_context);
		m_Context->setSwapInterval(1); // disable sync

		init();

		CefWindowInfo windowInfo = {};
		windowInfo.SetAsWindowless(m_Window->getWindowNativeHandle());
#if OE_WINDOWS && OE_D3D
		// TODO: implement, seems to be broken in CEF 84
		// https://bitbucket.org/chromiumembedded/cef/issues/2575
		// https://bitbucket.org/chromiumembedded/cef/pull-requests/285/reimplement-shared-texture-support-for-viz

		// only supported on Windows with D3D11
		// windowInfo.shared_texture_enabled = 1;
#endif

		CefBrowserSettings settings = {};
		settings.windowless_frame_rate = 60;

		CefBrowserHost::CreateBrowser(windowInfo, this, initial_url, settings, nullptr, nullptr);
	}

	void EditorWindow::work()
	{
		if (!m_Context)
			return;

		if (m_Window->destroyRequested()) {
			destroy();
		}
		else {
			m_Context->makeCurrent();
			m_Window->processEvents();

			Vec2i window_size = m_Window->getSize();
			if (window_size != m_LastSize) {
				m_LastSize = window_size;
				m_Context->resizeContext(m_Window->getSize());
				CefView::resizeView(window_size.x, window_size.y);
			}

			m_Context->prepare();

			render();
		}
	}

	void EditorWindow::present()
	{
		if (m_Context)
			m_Context->present();
	}

	bool EditorWindow::active() const
	{
		return m_Window || isViewOpen();
	}

	void EditorWindow::onMessage(const std::string& data)
	{
		OE_LOG_DEBUG("Received: " << data);
		sendMessage(data);
	}

	void EditorWindow::destroy()
	{
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

		Texture* blit_tex = getViewBuffer().getTexture();
		auto size = blit_tex ? Vec2f(blit_tex->getProperties().width, blit_tex->getProperties().height) : Vec2f(100, 100);

		/*
		m_SpriteBatcher->bindColor(Color4f(1.0, 0.0, 1.0, 0.5));
		m_SpriteBatcher->bindTexture(NULL);
		m_SpriteBatcher->rect(Vec2f(0, 0), Vec2f(size.x, size.y));
		*/

		m_SpriteBatcher->bindColor(Color4f(1.0, 1.0, 1.0, 1.0));
		m_SpriteBatcher->bindTexture(blit_tex);
		m_SpriteBatcher->rect(Vec2f(0, size.y), Vec2f(size.x, -size.y));

		m_SpriteBatcher->bindColor(Color4f(1.0, 0.0, 1.0, 1.0));
		m_SpriteBatcher->bindTexture(NULL);
		m_SpriteBatcher->rect(Vec2f(200, 200), Vec2f(100, 100));

		m_SpriteBatcher->end();
	}

	void EditorWindow::deinit()
	{
		delete m_SpriteBatcher;
		m_SpriteBatcher = nullptr;
	}

} }