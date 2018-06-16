#ifndef MEMORY_MEMORY_POOL_HPP
#define MEMORY_MEMORY_POOL_HPP

#include <vector>

#include "OE/Memory/Allocator.hpp"

namespace OrbitEngine {	namespace Memory {
	class MemoryPool : public Allocator {
	public:
		MemoryPool(unsigned long long size, int capacity);
		virtual ~MemoryPool();

		virtual void* Allocate() override;
		virtual void Deallocate(void* ptr) override;

	private:
		unsigned long long m_Size;
		int m_Capacity;

		int m_Free;
		unsigned char* m_Memory;
		unsigned char* m_Next;
	};
} }

#endif