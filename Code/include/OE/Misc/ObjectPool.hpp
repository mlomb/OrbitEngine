#ifndef MISC_OBJECT_POOL_HPP
#define MISC_OBJECT_POOL_HPP

#include <vector>

#include "OE/Misc/MemoryPool.hpp"

namespace OrbitEngine {	namespace Misc {
	template<typename T>
	class ObjectPool : public TrackedMemoryPool {
	public:
		ObjectPool(int capacity);
		virtual ~ObjectPool();

		virtual T* acquire();
		virtual void release(T* obj);
	};
	
	template<typename T>
	inline ObjectPool<T>::ObjectPool(int capacity)
		: TrackedMemoryPool(sizeof(T), capacity)
	{
	}

	template<typename T>
	inline ObjectPool<T>::~ObjectPool()
	{
	}

	template<typename T>
	inline T* ObjectPool<T>::acquire()
	{
		void* ptr = allocate();
		if (ptr == 0)
			return 0;

		return new (ptr) T;
	}

	template<typename T>
	inline void ObjectPool<T>::release(T* obj)
	{
		obj->~T();

		deallocate(obj);
	}
} }

#endif