#ifndef SYSTEM_SYSTEM_EMSCRIPTEN_HPP
#define SYSTEM_SYSTEM_EMSCRIPTEN_HPP

#include "OE/System/SystemImpl.hpp"

namespace OrbitEngine { namespace System { namespace priv {

	class SystemEmscripten : public SystemImpl {
	public:
		SystemEmscripten() {};
		~SystemEmscripten() {};

		const char* getName() override;
		long long currentNano() override;

	protected:

	};
} } }

#endif