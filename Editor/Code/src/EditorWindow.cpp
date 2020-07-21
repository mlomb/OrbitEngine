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

			m_Context->resizeContext(m_Window->getSize());
			m_Context->makeCurrent();
			m_Context->prepare();

			Vec2i window_size = m_Window->getSize();
			if (window_size != m_LastSize) {
				printf("%i x %i\n", m_Window->getSize().x, m_Window->getSize().y);

				m_Browser->GetHost()->WasResized();
				m_LastSize = window_size;

			}

			render();
			m_Context->present();
		}
	}

	bool EditorWindow::active() const
	{
		return m_Window || m_Browser;
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
				// WARNING: sizes doesn't match
				printf("%i > SIZES DONT MATCH\n", GetCurrentThreadId());
				delete m_BlitBrowserTexture;
				m_BlitBrowserTexture = nullptr;
				// TODO: is this the way to go?
			}
		}

		// recreate
		if (!m_BlitBrowserTexture) {
			TextureProperties props;
			props.formatProperties.format = RGBA;
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

		m_SpriteBatcher->setPVMatrices(Mat4::Orthographic(0.0f, (float)m_LastSize.x, (float)m_LastSize.y, 0.0f, -1.0f, 1.0f));
		
		m_SpriteBatcher->begin();

		m_SpriteBatcher->bindColor(Color4f(1.0, 1.0, 1.0, 1.0));
		m_SpriteBatcher->bindTexture(m_BlitBrowserTexture);
		auto size = m_BlitBrowserTexture ? Vec2f(m_BlitBrowserTexture->getProperties().width, m_BlitBrowserTexture->getProperties().height) : Vec2f();
		m_SpriteBatcher->rect(Vec2f(0, size.y), Vec2f(size.x, -size.y));

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