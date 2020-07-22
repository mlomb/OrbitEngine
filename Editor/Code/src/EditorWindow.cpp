#include "EditorWindow.hpp"

#include <OE/Misc/Log.hpp>
#include <OE/Graphics/API/FrameBuffer.hpp>

#if OE_WINDOWS
#include <OE/Platform/Windows/WindowWindows.hpp>
#endif

namespace OrbitEngine { namespace Editor {
	EditorWindow::EditorWindow()
		: m_Window(nullptr), m_Context(nullptr), m_BlitBrowserTexture(nullptr), m_LastSize(0, 0)
	{
	}

	EditorWindow::~EditorWindow()
	{
		OE_ASSERT_MSG(!active(), "The editor window is not ready to be destroyed.");
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

		CefWindowInfo windowInfo;
		CefBrowserSettings settings;
		windowInfo.SetAsWindowless(m_Window->getWindowNativeHandle());
		settings.windowless_frame_rate = 60;

		m_Client = new CefBrowserClient(this);
		m_Browser = CefBrowserHost::CreateBrowserSync(windowInfo, m_Client, "", settings, nullptr, nullptr);

		init();

		// ready
		m_Browser->GetMainFrame()->LoadURL("https://www.testufo.com");
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
			else {
				// waiting to close...
			}
		}
		else {
			m_Window->processEvents();

			m_Context->resizeContext(m_Window->getSize());
			m_Context->makeCurrent();
			m_Context->prepare();

			Vec2i window_size = m_Window->getSize();
			if (window_size != m_LastSize) {
				m_Browser->GetHost()->WasResized();
				m_LastSize = window_size;
			}

			render();
		}
	}

	bool EditorWindow::active() const
	{
		return m_Window || m_Browser;
	}

	void EditorWindow::present()
	{
		if(m_Context)
			m_Context->present();
	}

	Vec2i EditorWindow::getSize() const
	{
		return m_Window ? m_Window->getSize() : Vec2i(100, 100);
	}

	void EditorWindow::blitBrowser(const void* data, int width, int height)
	{
		// printf("%i > frame %p %p\n", GetCurrentThreadId(), this, data);

		if (!m_Context || !active())
			return;

		m_Context->makeCurrent();

		// recreate blit texture
		if (m_BlitBrowserTexture) {
			if (m_BlitBrowserTexture->getProperties().width != width ||
				m_BlitBrowserTexture->getProperties().height != height) {
				delete m_BlitBrowserTexture;
				m_BlitBrowserTexture = nullptr;
			}
		}

		// recreate
		if (!m_BlitBrowserTexture) {
			TextureProperties props;
			props.sampleProperties.filter = NEAREST;
			props.formatProperties.format = BGRA;
			props.width = width;
			props.height = height;
			props.textureBufferMode = DYNAMIC;
			m_BlitBrowserTexture = Texture::Create(props, nullptr);
		}

		m_BlitBrowserTexture->setData(const_cast<void*>(data)); // should be const void*
	}

	void EditorWindow::destroy()
	{
		m_Client = nullptr;
		m_Browser = nullptr;
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