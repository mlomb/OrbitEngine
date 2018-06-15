#ifndef MEMORY_MEMORY_POOL_INL
#define MEMORY_MEMORY_POOL_INL

#include "OE/Memory/MemoryPool.hpp"

namespace OrbitEngine {	namespace Memory {

	template<typename T>
	inline TrackedMemoryPool<T>::TrackedMemoryPool(int capacity)
		: MemoryPool(sizeof(T), capacity)
	{
		m_Used.reserve(capacity);
	}

	template<typename T>
	inline TrackedMemoryPool<T>::~TrackedMemoryPool()
	{
		m_Used.clear();
	}

	template<typename T>
	inline StrongPtr<T> TrackedMemoryPool<T>::AllocatePtr()
	{
		void* obj = MemoryPool::Allocate();
		if (obj == 0)
			return StrongPtr<T>();

		StrongPtr<T> ptr = StrongPtr<T>(static_cast<T*>(obj));

		auto it = std::lower_bound(m_Used.begin(), m_Used.end(), WeakPtr<T>(ptr), [](const WeakPtr<T>& lhs, const WeakPtr<T>& rhs) {
			return lhs.Get() < rhs.Get();
		});

		m_Used.insert(it, ptr);

		return ptr;
	}

	template<typename T>
	inline void TrackedMemoryPool<T>::DeallocatePtr(Ptr<T>& ptr)
	{
		MemoryPool::Deallocate(ptr.Get());

		auto it = std::lower_bound(m_Used.begin(), m_Used.end(), WeakPtr<T>(ptr), [](const WeakPtr<T>& lhs, const WeakPtr<T>& rhs) {
			return lhs.Get() < rhs.Get();
		});
		m_Used.erase(it);
	}

	template<typename T>
	inline const std::vector<WeakPtr<T>>& TrackedMemoryPool<T>::GetAll() const
	{
		return m_Used;
	}
} }

#endif