#ifndef MISC_MEMORY_POOL_HPP
#define MISC_MEMORY_POOL_HPP

#include <vector>

#include "OE/Misc/Allocator.hpp"

namespace OrbitEngine {	namespace Misc {
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

	class TrackedMemoryPool : public MemoryPool {
	public:
		TrackedMemoryPool(unsigned long long size, int capacity);
		~TrackedMemoryPool();

		void* Allocate() override;
		void Deallocate(void* ptr) override;

		typename std::vector<void*>::const_iterator begin() const;
		typename std::vector<void*>::const_iterator end() const;

	private:
		std::vector<void*> m_Used;
	};
} }

#endif