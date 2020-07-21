#include "EditorWindow.hpp"

#include <OE/Misc/Log.hpp>

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

		m_Context = new Context(RenderAPI::OPENGL, m_Window);

		CefWindowInfo windowInfo;
		CefBrowserSettings settings;
		windowInfo.SetAsWindowless(m_Window->getWindowNativeHandle());

		m_Client = new CefBrowserClient(this);
		m_Browser = CefBrowserHost::CreateBrowserSync(windowInfo, m_Client, "", settings, nullptr, nullptr);

		init();

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
			else {
				// waiting to close...
			}
		}
		else {
			m_Window->processEvents();

			Vec2i window_size = m_Window->getSize();
			if (window_size != m_LastSize) {
				m_Context->resizeContext(window_size);
				m_Browser->GetHost()->WasResized();
				m_LastSize = window_size;

				// recreate blit texture
				if (m_BlitBrowserTexture)
					delete m_BlitBrowserTexture;
				TextureProperties props;
				props.formatProperties.format = RGBA;
				props.width = m_LastSize.x;
				props.height = m_LastSize.y;
				props.textureBufferMode = DYNAMIC;
				m_BlitBrowserTexture = Texture::Create(props, nullptr);
			}

			m_Window->getSize();
			m_Context->makeCurrent();
			m_Context->prepare();
			render();
			m_Context->present();
		}
	}

	bool EditorWindow::active() const
	{
		return m_Window || m_Browser;
	}

	Texture* EditorWindow::getBrowserBlitTexture() const
	{
		m_Context->makeCurrent(); // TODO: don't
		return m_BlitBrowserTexture;
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
		m_SpriteBatcher = new SpriteBatcher(1000); // no need for many quads
	}

	void EditorWindow::render()
	{
		using namespace OrbitEngine::Math;

		m_SpriteBatcher->setPVMatrices(Mat4::Orthographic(0.0f, (float)m_LastSize.x, (float)m_LastSize.y, 0.0f, -1.0f, 1.0f));
		
		m_SpriteBatcher->begin();

		m_SpriteBatcher->bindColor(Color4f(1.0, 1.0, 1.0, 1.0));
		m_SpriteBatcher->bindTexture(m_BlitBrowserTexture);
		m_SpriteBatcher->rect(Vec2f(0, 0), Vec2f(500, 500));

		m_SpriteBatcher->end();
	}

	void EditorWindow::deinit()
	{
		delete m_SpriteBatcher;
		if (m_BlitBrowserTexture)
			delete m_BlitBrowserTexture;
		m_SpriteBatcher = nullptr;
		m_BlitBrowserTexture = nullptr;
	}

} }