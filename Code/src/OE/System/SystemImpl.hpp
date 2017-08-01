#ifndef SYSTEMIMPL_H
#define SYSTEMIMPL_H

namespace OrbitEngine {	namespace System { namespace priv {
	class SystemImpl {
	public:
		~SystemImpl() { };
		
		static long long CurrentMillis();
	protected:
		SystemImpl() { };

	private:

	};
} } }

#endif