#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

namespace OrbitEngine {	namespace Memory {
	class Allocator {
	public:
		Allocator() {};
		virtual ~Allocator() {};

		virtual void* Allocate() = 0;
		virtual void Deallocate(void* ptr) = 0;
	};
} }

#endif