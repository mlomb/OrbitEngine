#ifndef EDITOR_WINDOW_HPP
#define EDITOR_WINDOW_HPP

#include <thread>

#include "Cef.hpp"
#include "CefView.hpp"

#include <OE/Application/Window.hpp>
#include <OE/Application/Context.hpp>
#include <OE/Graphics/API/Texture.hpp>
#include <OE/Graphics/2D/SpriteBatcher.hpp>

namespace OrbitEngine { namespace Editor {
	using namespace OrbitEngine;
	using namespace OrbitEngine::Application;
	using namespace OrbitEngine::Graphics;
	using namespace OrbitEngine::Math;
	
	/**
		@brief The base editor window with a CEF browser and a rendering context
	*/
	class EditorWindow {
	public:
		EditorWindow();
		~EditorWindow();

		void create();
		void work();
		bool active() const;
		void present();

	private:
		CefRefPtr<CefView> m_CefView;

		Vec2i m_LastSize;
		Window* m_Window;
		Context* m_Context;

		Texture* m_BlitBrowserTexture;
		SpriteBatcher* m_SpriteBatcher;

		void init();
		void render();
		void deinit();
		void destroy();
	};
} };

#endif