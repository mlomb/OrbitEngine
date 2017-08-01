#ifndef APPLICATION_APPLICATION_HPP
#define APPLICATION_APPLICATION_HPP

namespace OrbitEngine {	namespace Application {
	namespace priv {
		class ApplicationImpl;
	}

	class Application : Misc::Pimpl<priv::ApplicationImpl> {
	public:
		Application();
		~Application();

	};
} }

#endif