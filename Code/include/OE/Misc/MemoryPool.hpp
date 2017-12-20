#ifndef MISC_MEMORY_POOL_HPP
#define MISC_MEMORY_POOL_HPP

#include <vector>

namespace OrbitEngine {	namespace Misc {
	class MemoryPool {
	public:
		MemoryPool(size_t size, int capacity);
		virtual ~MemoryPool();

		virtual void* allocate();
		virtual void deallocate(void* ptr);

	private:
		size_t m_Size;
		int m_Capacity;

		int m_Free;
		unsigned char* m_Memory;
		unsigned char* m_Next;
	};

	class TrackedMemoryPool : public MemoryPool {
	public:
		TrackedMemoryPool(size_t size, int capacity);
		~TrackedMemoryPool();

		void* allocate() override;
		void deallocate(void* ptr) override;

		typename std::vector<void*>::const_iterator begin() const;
		typename std::vector<void*>::const_iterator end() const;

	private:
		std::vector<void*> m_Used;
		unsigned int m_UsedCount;
	};
} }

#endif