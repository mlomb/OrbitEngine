#ifndef SYSTEMIMPL_H
#define SYSTEMIMPL_H

#include "OE/System/System.hpp"

namespace OrbitEngine {	namespace System { namespace priv {
	class SystemImpl {
	public:
		~SystemImpl() { };

		virtual std::vector<MonitorDesc> getMonitors() = 0;
		virtual const char* getName() = 0;
		virtual long long currentNano() = 0;

	protected:
		SystemImpl() { };

	private:

	};
} } }

#endif