#ifndef SYSTEM_SYSTEM_LINUX_HPP
#define SYSTEM_SYSTEM_LINUX_HPP

#include "OE/System/SystemImpl.hpp"

namespace OrbitEngine { namespace System { namespace priv {

	class SystemLinux : public SystemImpl {
	public:
		SystemLinux();
		~SystemLinux() {};

		const char* getName() override;
		long long currentNano() override;

	protected:

	};
} } }

#endif