#include "OE/Graphics/2D/ImGuiRenderer.hpp"

#include "OE/Graphics/ShaderLoader.hpp"
#ifdef OE_OPENGL_ANY
#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

#include "OE/Application/Context.hpp"
#include "OE/Application/Window.hpp"
#include "OE/Application/InputManager.hpp"
#include "OE/System/System.hpp"
#include "OE/Graphics/API/States.hpp"
#include "OE/Graphics/Bitmap.hpp"
#include "OE/Math/Scissor.hpp"

namespace OrbitEngine { namespace Graphics {
	
	#define GET_PTRS \
		Application::Window*   window = static_cast<Application::Window*>(viewport->PlatformHandle); \
		Application::Context* context = static_cast<Application::Context*>(viewport->PlatformUserData); \
		ImGuiRenderer*       renderer = static_cast<ImGuiRenderer*>(viewport->RendererUserData);

	void ImGui_ImplOE_CreateWindow(ImGuiViewport* viewport) {
		Application::Window* window = new OrbitEngine::Application::Window();

		if(viewport->Flags & ImGuiViewportFlags_NoDecoration)
			window->setDisplayMode(Application::DisplayMode::BORDERLESS);

		viewport->PlatformHandle = window;
		viewport->PlatformHandleRaw = reinterpret_cast<void*>(window->getDisplayNativeHandle());
	}

	void ImGui_ImplOERenderer_CreateWindow(ImGuiViewport* viewport) {
		GET_PTRS;

		Application::Context* current_context = Application::Context::GetCurrent();

		// create a shared context
		viewport->PlatformUserData = new Application::Context(current_context->getAPI(), window, current_context);
		viewport->RendererUserData = ImGui::GetMainViewport()->RendererUserData; // copy the renderer pointer

		// restore
		current_context->makeCurrent();
	}

	void ImGui_ImplOE_UpdateWindow(ImGuiViewport* viewport) {
		GET_PTRS;

		// done in begin()
		//window->processEvents();

		viewport->PlatformRequestMove = true;
		viewport->PlatformRequestResize = true;
	}

	void ImGui_ImplOERenderer_RenderWindow(ImGuiViewport* viewport, void*) {
		GET_PTRS;

		Application::Context* last_ctx = Application::Context::GetCurrent();

		context->makeCurrent();
		context->resizeContext(window->getSize());
		context->prepare();

		renderer->draw(viewport->DrawData);

		context->present();

		// restore
		last_ctx->makeCurrent();
	}

	void ImGui_ImplOERenderer_DestroyWindow(ImGuiViewport* viewport) {
		GET_PTRS;

		Application::Context* last_ctx = Application::Context::GetCurrent();
		bool same = context == last_ctx;

		context->makeCurrent();
		delete context;

		// restore
		if (!same)
			last_ctx->makeCurrent();

		viewport->PlatformUserData = NULL;
		viewport->RendererUserData = NULL;
	}

	void ImGui_ImplOE_DestroyWindow(ImGuiViewport* viewport) {
		GET_PTRS;
		delete window;
		viewport->PlatformHandle = NULL;
		viewport->PlatformHandleRaw = NULL;
	}

	void   ImGui_ImplOE_SetWindowPos(ImGuiViewport* viewport, ImVec2 pos)   { GET_PTRS; window->setPosition(Math::Vec2i(pos.x, pos.y)); }
	ImVec2 ImGui_ImplOE_GetWindowPos(ImGuiViewport* viewport)               { GET_PTRS; return ImVec2(window->getPosition().x, window->getPosition().y); }
	void   ImGui_ImplOE_SetWindowSize(ImGuiViewport* viewport, ImVec2 size) { GET_PTRS; window->setSize(Math::Vec2i(size.x, size.y)); }
	ImVec2 ImGui_ImplOE_GetWindowSize(ImGuiViewport* viewport)              { GET_PTRS; return ImVec2(window->getSize().x, window->getSize().y); }
	void   ImGui_ImplOE_SetWindowTitle(ImGuiViewport* viewport, const char* title) { GET_PTRS; window->setTitle(std::string(title)); }
	void   ImGui_ImplOE_SetWindowAlpha(ImGuiViewport* viewport, float alpha) { GET_PTRS; window->setAlpha(alpha); }
	void   ImGui_ImplOE_ShowWindow(ImGuiViewport* viewport)                 { GET_PTRS; window->setVisibility(true); }

	ImGuiRenderer::ImGuiRenderer()
	{
		Application::Context* current_context = Application::Context::GetCurrent();
		m_ImGuiContext = ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

		ImGui::StyleColorsDark();

		std::string renderer_name = current_context->getName();
		char* renderer_name_c = new char[renderer_name.size() + 1];
		strcpy(renderer_name_c, &renderer_name[0]);

		io.BackendPlatformName = "OrbitEngine";
		io.BackendRendererName = renderer_name_c;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		if (
#if OE_OPENGL_ANY
			current_context->getAPI() == OPENGL ||
#if OE_OPENGL_ES
			current_context->getAPI() == OPENGL_ES ||
#endif
#endif
#if OE_D3D
			current_context->getAPI() == DIRECT3D
#endif
			) {
			// ImGuiRenderer supports viewports
			io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
		}
#if OE_WINDOWS
		// only the Windows platform supports viewports
		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
#endif

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
#if OE_WINDOWS
			// Windows support window->setAlpha
			io.ConfigDockingTransparentPayload = true;
#endif
		}

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			// Load monitor data
			platform_io.Monitors.resize(0);
			for (System::MonitorDesc& desc : System::System::Get()->getMonitors()) {
				ImGuiPlatformMonitor monitor;
				monitor.MainPos.x = desc.mainArea.x;
				monitor.MainPos.y = desc.mainArea.y;
				monitor.MainSize.x = desc.mainArea.z;
				monitor.MainSize.y = desc.mainArea.w;
				monitor.WorkPos.x = desc.workArea.x;
				monitor.WorkPos.y = desc.workArea.y;
				monitor.WorkSize.x = desc.workArea.z;
				monitor.WorkSize.y = desc.workArea.w;
				monitor.DpiScale = desc.dpi;
				platform_io.Monitors.push_back(monitor);
			}

			/////////////////////
			// Setup callbacks //
			/////////////////////

			/// PlatformHandle    = Application::Window*
			/// PlatformHandleRaw = HWND (for example)
			/// PlatformUserData  = Application::Context*
			/// RendererUserData  = ImGuiRenderer* (always this instance)

			/// PLATFORM
			platform_io.Platform_CreateWindow   = ImGui_ImplOE_CreateWindow;
			platform_io.Platform_DestroyWindow  = ImGui_ImplOE_DestroyWindow;

			platform_io.Platform_SetWindowPos   = ImGui_ImplOE_SetWindowPos;
			platform_io.Platform_GetWindowPos   = ImGui_ImplOE_GetWindowPos;
			platform_io.Platform_SetWindowSize  = ImGui_ImplOE_SetWindowSize;
			platform_io.Platform_GetWindowSize  = ImGui_ImplOE_GetWindowSize;
			platform_io.Platform_SetWindowTitle = ImGui_ImplOE_SetWindowTitle;
			platform_io.Platform_SetWindowAlpha = ImGui_ImplOE_SetWindowAlpha;
			platform_io.Platform_ShowWindow     = ImGui_ImplOE_ShowWindow;

			platform_io.Platform_UpdateWindow   = ImGui_ImplOE_UpdateWindow;

			/// RENDERER
			platform_io.Renderer_CreateWindow  = ImGui_ImplOERenderer_CreateWindow;
			platform_io.Renderer_DestroyWindow = ImGui_ImplOERenderer_DestroyWindow;
			platform_io.Renderer_RenderWindow  = ImGui_ImplOERenderer_RenderWindow;

			// set pointers for the default viewport
			ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			main_viewport->PlatformHandle    = current_context->getWindow();
			main_viewport->PlatformHandleRaw = reinterpret_cast<void*>(current_context->getWindow()->getDisplayNativeHandle());
			main_viewport->PlatformUserData  = current_context;
			main_viewport->RendererUserData  = this;
		}

		// Keyboard mapping
		io.KeyMap[ImGuiKey_Tab]         = Application::Key::TAB;
		io.KeyMap[ImGuiKey_LeftArrow]   = Application::Key::ARROW_LEFT;
		io.KeyMap[ImGuiKey_RightArrow]  = Application::Key::ARROW_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow]     = Application::Key::ARROW_UP;
		io.KeyMap[ImGuiKey_DownArrow]   = Application::Key::ARROW_DOWN;
		io.KeyMap[ImGuiKey_PageUp]      = Application::Key::PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown]    = Application::Key::PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home]        = Application::Key::HOME;
		io.KeyMap[ImGuiKey_End]         = Application::Key::END;
		io.KeyMap[ImGuiKey_Insert]      = Application::Key::INSERT;
		io.KeyMap[ImGuiKey_Delete]      = Application::Key::DELETE_;
		io.KeyMap[ImGuiKey_Backspace]   = Application::Key::BACKSPACE;
		io.KeyMap[ImGuiKey_Space]       = Application::Key::SPACEBAR;
		io.KeyMap[ImGuiKey_Enter]       = Application::Key::ENTER;
		io.KeyMap[ImGuiKey_Escape]      = Application::Key::ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = Application::Key::ENTER;
		io.KeyMap[ImGuiKey_A]           = Application::Key::A;
		io.KeyMap[ImGuiKey_C]           = Application::Key::C;
		io.KeyMap[ImGuiKey_V]           = Application::Key::V;
		io.KeyMap[ImGuiKey_X]           = Application::Key::X;
		io.KeyMap[ImGuiKey_Y]           = Application::Key::Y;
		io.KeyMap[ImGuiKey_Z]           = Application::Key::Z;

		// Generate font atlas (may change later on)
		{
			io.Fonts->AddFontFromFileTTF("Resources/Fonts/Roboto-Regular.ttf", 17.0f);

			unsigned char* pixels;
			int width, height;
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

			TextureSampleProperties s = {};
			s.mipmapping = false;
			s.filter = TextureFilter::LINEAR;
			s.wrap = TextureWrap::CLAMP;

			io.Fonts->TexID = BitmapRGBA(pixels, width, height).toTexture(s);
		}

		VertexLayout* layout = new VertexLayout();
		layout->push<Math::Vec2f>("POSITION");
		layout->push<Math::Vec2f>("TEXCOORD");
		layout->push<unsigned char>("COLOR", 4, true);
		m_Mesh = Mesh::Create(layout, true);

		m_Shader = ShaderLoader::LoadRawShader("Resources/Shaders/ImGui.oeshader");
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		ImGui::SetCurrentContext(m_ImGuiContext);

		ImGuiIO& io = ImGui::GetIO();

		delete io.BackendRendererName;
		if (io.Fonts->TexID)
			delete io.Fonts->TexID;

		ImGui::DestroyContext();
	}

	void ImGuiRenderer::begin()
	{
		Renderer2D::begin();

		ImGui::SetCurrentContext(m_ImGuiContext);
		ImGuiIO& io = ImGui::GetIO();
		ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

		Application::Context* current_context = Application::Context::GetCurrent();
		Application::Window* current_window = current_context->getWindow();
		io.DisplaySize.x = current_window->getSize().x;
		io.DisplaySize.y = current_window->getSize().y;

		//////////////////
		// Update input //
		//////////////////
		for (int i = 1; i < platform_io.Viewports.Size; i++) {
			ImGuiViewport* viewport = platform_io.Viewports[i];
			GET_PTRS;
			window->processEvents();
		}

		Application::InputManager* input_manager = Application::InputManager::Get();

		/// Cursor position
		Math::Vec2f cursor_pos = Math::Vec2f(-FLT_MAX, -FLT_MAX);
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			// absolute coords
			cursor_pos = input_manager->getCursorPosition();
		}
		else {
			// relative coords
			cursor_pos = current_window->getCursorPosition();
		}
		io.MousePos = ImVec2(cursor_pos.x, cursor_pos.y);

		/// Cursor style
		if (!(io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)) {
			Application::Cursor target_cursor_style;
			switch (io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor()) {
			default:
			case ImGuiMouseCursor_None:         target_cursor_style = Application::Cursor::HIDDEN;   break;
			case ImGuiMouseCursor_Arrow:        target_cursor_style = Application::Cursor::ARROW;    break;
			case ImGuiMouseCursor_TextInput:    target_cursor_style = Application::Cursor::IBEAM;    break;
			case ImGuiMouseCursor_ResizeAll:    target_cursor_style = Application::Cursor::SIZEALL;  break;
			case ImGuiMouseCursor_ResizeEW:     target_cursor_style = Application::Cursor::SIZEWE;   break;
			case ImGuiMouseCursor_ResizeNS:     target_cursor_style = Application::Cursor::SIZENS;   break;
			case ImGuiMouseCursor_ResizeNESW:   target_cursor_style = Application::Cursor::SIZENESW; break;
			case ImGuiMouseCursor_ResizeNWSE:   target_cursor_style = Application::Cursor::SIZENWSE; break;
			case ImGuiMouseCursor_Hand:         target_cursor_style = Application::Cursor::HAND;     break;
			}

			for (int i = 0; i < platform_io.Viewports.Size; i++) {
				ImGuiViewport* viewport = platform_io.Viewports[i];
				GET_PTRS;
				window->setCursor(target_cursor_style);
			}
		}

		/// other input states
		for (int i = 0; i < std::min(OE_MAX_CBUTTONS, 5); i++)
			io.MouseDown[i] = input_manager->isButtonDown(static_cast<Application::Button>(i));
		io.MouseWheel = input_manager->getWheelDelta();
		for (int i = 0; i < std::min(OE_MAX_KEYS, 512); i++) {
			Application::Key k = static_cast<Application::Key>(i);
			io.KeysDown[i] = input_manager->isKeyDown(k);

			// TODO: Replace this
			if (input_manager->isKeyPressed(k))
				io.AddInputCharacter(k);
		}
		io.KeyCtrl= input_manager->isKeyDown(Application::Key::CONTROL);
		io.KeyAlt = input_manager->isKeyDown(Application::Key::ALT);
		io.KeyShift= input_manager->isKeyDown(Application::Key::SHIFT);
		io.KeySuper = input_manager->isKeyDown(Application::Key::LWIN); // TODO: test in other platforms

		/// start frame
		ImGui::NewFrame();
	}

	void ImGuiRenderer::end()
	{
		ImGui::SetCurrentContext(m_ImGuiContext);
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();

		draw(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiRenderer::draw(ImDrawData* draw_data)
	{
		ImVec2 clip_off = draw_data->DisplayPos;
		ImVec2 clip_scale = draw_data->FramebufferScale;

		Renderer2D::end();

		States* states = Application::Context::GetCurrent()->getGlobalStates();

		states->setBlending(BlendState::SRC_ALPHA);
		states->setDepthTest(FunctionMode::DISABLED);
		states->setStencil(FunctionMode::DISABLED);
		states->setCullMode(CullMode::FRONT);
		
		m_Shader->bind();
		m_UPVMatrix->bind(0, Graphics::ShaderType::VERTEX);
		m_UPVMatrix->setData(Math::Mat4::Orthographic(
			draw_data->DisplayPos.x,
			draw_data->DisplayPos.x + draw_data->DisplaySize.x,
			draw_data->DisplayPos.y + draw_data->DisplaySize.y,
			draw_data->DisplayPos.y,
			-1.0f, 1.0f
		));

		for (int n = 0; n < draw_data->CmdListsCount; n++) {
			const ImDrawList* cmd_list = draw_data->CmdLists[n];

			m_Mesh->getVBO()->setData((GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data);
			m_Mesh->getIBO()->setData((GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data);

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

				if (pcmd->UserCallback == NULL) {
					ImVec4 clip_rect;
					clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
					clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
					clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
					clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

					Math::Scissor* s = new Math::Scissor(
						(int)(clip_rect.x),
						(int)(clip_rect.y),
						(int)(clip_rect.z - clip_rect.x),
						(int)(clip_rect.w - clip_rect.y)
					);

					states->setScissor(s);
					delete s;

					if (pcmd->TextureId)
						static_cast<Texture*>(pcmd->TextureId)->bind(0);
					m_Mesh->drawIndexed(pcmd->ElemCount, pcmd->IdxOffset);
				}
				else {
					OE_LOG_WARNING("ImDrawList::AddCallback not supported yet");
				}
			}
		}

		states->setScissor(nullptr);
	}
} }