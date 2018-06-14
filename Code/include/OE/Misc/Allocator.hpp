#ifndef MISC_ALLOCATOR_HPP
#define MISC_ALLOCATOR_HPP

namespace OrbitEngine {	namespace Misc {
	class Allocator {
	public:
		Allocator() {};
		virtual ~Allocator() {};

		virtual void* Allocate() = 0;
		virtual void Deallocate(void* ptr) = 0;
	};
} }

#endif