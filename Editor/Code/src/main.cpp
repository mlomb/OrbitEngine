
#include <OE/Application/Window.hpp>
#include <OE/Application/Context.hpp>
#include <OE/Misc/Ticker.hpp>

int main() {
	using namespace OrbitEngine;
	using namespace OrbitEngine::Application;
	using namespace OrbitEngine::Misc;

	Window* window = new Window();
	window->setTitle("UI Testing");
	window->setVisibility(true);
	
	Context* ctx = new Context(RenderAPI::OPENGL, window);

	Ticker t;

	while (!window->destroyRequested()) {
		window->processEvents();
		t.tick();
		ctx->prepare();

		// render

		ctx->present();
	}

	return 0;
}