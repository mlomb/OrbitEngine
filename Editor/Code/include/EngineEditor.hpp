#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <OE/Misc/Ticker.hpp>

#include "EditorWindow.hpp"

namespace OrbitEngine { namespace Editor {
	using namespace OrbitEngine::Misc;

	class EngineEditor
	{
	public:
		EngineEditor();
		~EngineEditor();

		void open(const std::string& uri);

		void run();

	private:
		Ticker m_Ticker;
		Window* m_DummyWindow;
		Context* m_Context;

		std::vector<EditorWindow*> m_Windows;
	};
} }

#endif