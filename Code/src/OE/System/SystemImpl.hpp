#ifndef SYSTEMIMPL_H
#define SYSTEMIMPL_H

namespace OrbitEngine {	namespace System { namespace priv {
	class SystemImpl {
	public:
		~SystemImpl() { };

		virtual const char* getName() = 0;
		virtual long long currentNano() = 0;

	protected:
		SystemImpl() { };

	private:

	};
} } }

#endif