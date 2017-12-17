#ifndef APPLICATION_LOOPEABLE_HPP
#define APPLICATION_LOOPEABLE_HPP

namespace OrbitEngine {	namespace Application {
	class Loopeable {
	public:
		Loopeable() {};
		~Loopeable() {};

		virtual void init() { };
		virtual void render() = 0;
		virtual void update(float delta) {};
		virtual void deinitialize() {};
	};
} }

#endif