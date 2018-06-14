#include "OE/Misc/MemoryPool.hpp"

#include <iostream>
#include <algorithm>
#include <string.h>

namespace OrbitEngine {	namespace Misc {
	MemoryPool::MemoryPool(unsigned long long size, int capacity)
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

	void* MemoryPool::Allocate()
	{
		void* ptr = 0;
		if (m_Free > 0) {
			ptr = m_Next;
			m_Free--;
			m_Next = m_Free > 0 ? (m_Memory + (*(unsigned int*)m_Next) * m_Size) : 0;
		}
		return ptr;
	}

	void MemoryPool::Deallocate(void* ptr)
	{
		if (m_Next != 0)
			*((unsigned int*)ptr) = (unsigned int)(m_Next - m_Memory) / (unsigned int)m_Size;
		else
			*((unsigned int*)ptr) = m_Capacity;
		m_Next = (unsigned char*)ptr;
		++m_Free;
	}

	TrackedMemoryPool::TrackedMemoryPool(unsigned long long size, int capacity)
		: MemoryPool(size, capacity)
	{
		m_Used.reserve(capacity);
	}

	TrackedMemoryPool::~TrackedMemoryPool()
	{
		m_Used.clear();
	}

	void* TrackedMemoryPool::Allocate()
	{
		void* obj = MemoryPool::Allocate();
		if (obj == 0)
			return 0;

		auto it = std::lower_bound(m_Used.begin(), m_Used.end(), obj);
		m_Used.insert(it, obj);

		return obj;
	}

	void TrackedMemoryPool::Deallocate(void* ptr)
	{
		MemoryPool::Deallocate(ptr);

		auto it = std::lower_bound(m_Used.begin(), m_Used.end(), ptr);
		m_Used.erase(it);
	}

	typename std::vector<void*>::const_iterator TrackedMemoryPool::begin() const
	{
		return m_Used.begin();
	}

	typename std::vector<void*>::const_iterator TrackedMemoryPool::end() const
	{
		return m_Used.begin() + m_Used.size();
	}
} }