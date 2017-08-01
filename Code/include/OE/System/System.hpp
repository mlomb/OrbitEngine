#ifndef SYSTEM_SYSTEM_HPP
#define SYSTEM_SYSTEM_HPP

#include "OE/Misc/Pimpl.hpp"
#include "OE/Misc/Singleton.hpp"

namespace OrbitEngine {	namespace System {
	namespace priv {
		class SystemImpl;
	}

	class System : public Misc::Pimpl<priv::SystemImpl>, public Misc::Singleton<System> {
	public:

		const char* getName();
		long long currentMillis();


	private:
		friend class Misc::Singleton<System>;

		System();
		~System();
	};
} }

#endif