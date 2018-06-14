#include "OE/Misc/Ptrs.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine {

	namespace internal {
		void RefCount::Decr()
		{
			//OE_LOG_DEBUG("DECR");
			strong--;
			if (strong + weak == 0) Destroy();
		}

		void RefCount::Incr()
		{
			//OE_LOG_DEBUG("INCR");
			strong++;
		}

		void RefCount::WDecr()
		{
			//OE_LOG_DEBUG("WDECR");
			weak--;
			if (strong + weak == 0) Destroy();
		}

		void RefCount::WIncr()
		{
			//OE_LOG_DEBUG("WINCR");
			weak++;
		}

		void RefCount::Destroy()
		{
			//OE_LOG_DEBUG("DESTROY REF COUNT");
			delete this;
		}
	}

}
