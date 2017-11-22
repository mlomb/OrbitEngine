#ifndef APPLICATION_LOOPEABLE_HPP
#define APPLICATION_LOOPEABLE_HPP

namespace OrbitEngine {	namespace Application {
	class Loopeable {
	public:
		Loopeable() {};
		~Loopeable() {};

		virtual void render() = 0;
	};
} }

#endif