#ifndef MEMORY_MEMORY_POOL_HPP
#define MEMORY_MEMORY_POOL_HPP

#include <vector>

#include "OE/Memory/Allocator.hpp"
#include "OE/Memory/Ptrs.hpp"

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
	
	template<typename T>
	class TrackedMemoryPool : public MemoryPool {
	public:
		TrackedMemoryPool(int capacity);
		virtual ~TrackedMemoryPool();

		StrongPtr<T> AllocatePtr();
		void DeallocatePtr(Ptr<T>& ptr);

		const std::vector<WeakPtr<T>>& GetAll() const;

	private:
		std::vector<WeakPtr<T>> m_Used;
	};
} }

#include "OE/Memory/MemoryPool.inl"

#endif