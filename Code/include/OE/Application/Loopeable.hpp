#ifndef APPLICATION_LOOPEABLE_HPP
#define APPLICATION_LOOPEABLE_HPP

#include "OE/Application/Context.hpp"

namespace OrbitEngine {	namespace Application {
	class Loopeable {
	public:
		Loopeable() {};
		virtual ~Loopeable() {};

		virtual void init(OrbitEngine::Application::Context* context) { };
		virtual void render() = 0;
		virtual void update(float delta) {};
		virtual void deinitialize() {};
	};
} }

#endif