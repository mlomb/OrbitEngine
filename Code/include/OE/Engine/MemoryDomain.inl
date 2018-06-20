#ifndef ENGINE_MEMORY_DOMAIN_INL
#define ENGINE_MEMORY_DOMAIN_INL

#include "OE/Engine/MemoryDomain.hpp"

#include "OE/Memory/Ptrs.hpp"
#include "OE/Memory/MemoryPool.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Engine {
	
	template<typename T>
	inline StrongPtr<T> MemoryDomain::Create()
	{
		Meta::NativeType* type = Meta::NativeTypeResolver<T>::Get();
		OE_ASSERT(type);

		Memory::MemoryPool* allocator = GetAllocator<T>();
		if (!allocator)
			allocator = CreateAllocator<T>();

		void* obj = allocator->Allocate();
		if (obj == 0)
			return StrongPtr<T>();
		type->Construct(obj);

		std::vector<WeakPtr<T>>& objs = GetObjects<T>();

		StrongPtr<T> ptr = StrongPtr<T>(static_cast<T*>(obj));

		/* -- */
		auto it = std::lower_bound(objs.begin(), objs.end(), WeakPtr<T>(ptr), [](const WeakPtr<T>& lhs, const WeakPtr<T>& rhs) {
			return lhs.Get() < rhs.Get();
		});
		objs.insert(it, ptr);
		/* -- */

		return ptr;
	}

	template<typename T>
	inline void MemoryDomain::Destroy(Ptr<T>& object)
	{
		Meta::NativeType* type = Meta::NativeTypeResolver<T>::Get();
		Memory::MemoryPool* allocator = GetAllocator<T>();
		if (!type || !allocator) {
			delete object.Get();
			return;
		}

		type->Destruct(object.Get());
		allocator->Deallocate(object.Get());

		std::vector<WeakPtr<T>>& objs = GetObjects<T>();

		/* -- */
		auto it = std::lower_bound(objs.begin(), objs.end(), WeakPtr<T>(object), [](const WeakPtr<T>& lhs, const WeakPtr<T>& rhs) {
			return lhs.Get() < rhs.Get();
		});
		objs.erase(it);
		/* -- */
	}

	template<typename T>
	inline const std::vector<WeakPtr<T>>& MemoryDomain::GetAll()
	{
		return GetObjects<T>();
	}

	template<typename T>
	inline Memory::MemoryPool* MemoryDomain::GetAllocator()
	{
		auto it = m_Allocators.find(Meta::NativeTypeResolver<T>::Get());
		if (it != m_Allocators.end())
			return static_cast<Memory::MemoryPool*>((*it).second);
		return nullptr;
	}

	template<typename T>
	inline Memory::MemoryPool* MemoryDomain::CreateAllocator()
	{
		const int capacity = 100;

		Memory::MemoryPool* allocator = new Memory::MemoryPool(sizeof(T), capacity);

		m_Allocators.insert(std::make_pair(Meta::NativeTypeResolver<T>::Get(), allocator));
		GetObjects<T>().reserve(capacity);

		return allocator;
	}

	template<typename T>
	inline std::vector<WeakPtr<T>>& MemoryDomain::GetObjects()
	{
		Meta::NativeType* type = Meta::NativeTypeResolver<T>::Get();
		OE_ASSERT(type);
		auto it = m_Objects.find(type);
		if (it == m_Objects.end())
			m_Objects[type] = new std::vector<WeakPtr<T>>();
		return *static_cast<std::vector<WeakPtr<T>>*>(m_Objects[type]);
	}
} }

#endif