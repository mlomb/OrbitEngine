#include "OE/Misc/MemoryPool.hpp"

#include <iostream>

namespace OrbitEngine {	namespace Misc {
	MemoryPool::MemoryPool(size_t size, int capacity)
		: m_Size(size), m_Capacity(capacity)
	{
		m_Memory = new unsigned char[m_Size * capacity];
		m_Next = m_Memory;
		m_Free = m_Capacity;

		memset(m_Memory, 0, m_Size * capacity);

		for (int i = 0; i < capacity; i++) {
			unsigned int* p = (unsigned int*)(m_Memory + i * m_Size);
			*p = i + 1;
		}
	}

	MemoryPool::~MemoryPool()
	{
		delete[] m_Memory;
		m_Memory = 0;
	}

	void* MemoryPool::allocate()
	{
		void* ptr = 0;
		if (m_Free > 0) {
			ptr = m_Next;
			m_Free--;
			m_Next = m_Free > 0 ? (m_Memory + (*(unsigned int*)m_Next) * m_Size) : 0;
		}
		return ptr;
	}

	void MemoryPool::deallocate(void* ptr)
	{
		if (m_Next != 0)
			*((unsigned int*)ptr) = (unsigned int)(m_Next - m_Memory) / m_Size;
		else
			*((unsigned int*)ptr) = m_Capacity;
		m_Next = (unsigned char*)ptr;
		++m_Free;
	}

	TrackedMemoryPool::TrackedMemoryPool(size_t size, int capacity)
		: MemoryPool(size, capacity)
	{
		m_Used.resize(capacity);
		m_UsedCount = 0;
	}

	TrackedMemoryPool::~TrackedMemoryPool()
	{
		m_Used.clear();
	}

	void* TrackedMemoryPool::allocate()
	{
		void* obj = MemoryPool::allocate();
		if (obj == 0)
			return 0;
		m_Used[m_UsedCount++] = obj;
		return obj;
	}

	void TrackedMemoryPool::deallocate(void* ptr)
	{
		MemoryPool::deallocate(ptr);
		m_UsedCount--;
	}

	typename std::vector<void*>::const_iterator TrackedMemoryPool::begin() const
	{
		return m_Used.begin();
	}

	typename std::vector<void*>::const_iterator TrackedMemoryPool::end() const
	{
		return m_Used.begin() + m_UsedCount;
	}
} }