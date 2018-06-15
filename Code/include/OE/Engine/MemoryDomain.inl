#ifndef ENGINE_MEMORY_DOMAIN_INL
#define ENGINE_MEMORY_DOMAIN_INL

#include "OE/Engine/MemoryDomain.hpp"

#include "OE/Memory/Ptrs.hpp"
#include "OE/Memory/MemoryPool.hpp"

namespace OrbitEngine { namespace Engine {
	
	template<typename T>
	inline StrongPtr<T> MemoryDomain::Allocate()
	{
		Meta::NativeType* type = Meta::NativeTypeResolver<T>::Get();
		Memory::TrackedMemoryPool<T>* allocator = GetAllocator<T>();
		if (!allocator)
			allocator = CreateAllocator<T>();

		StrongPtr<T> object = allocator->AllocatePtr();
		if (object)
			type->Construct(object.Get());
		return object;
	}

	template<typename T>
	inline void MemoryDomain::Deallocate(Ptr<T>& object)
	{
		Meta::NativeType* type = Meta::NativeTypeResolver<T>::Get();
		Memory::TrackedMemoryPool<T>* allocator = GetAllocator<T>();
		OE_ASSERT(allocator)

		type->Destruct(object.Get());
		allocator->DeallocatePtr(object);
	}

	template<typename T>
	inline const std::vector<WeakPtr<T>>& MemoryDomain::GetAll()
	{
		return GetAllocator<T>()->GetAll();
	}

	template<typename T>
	inline Memory::TrackedMemoryPool<T>* MemoryDomain::GetAllocator()
	{
		Meta::NativeType* type = Meta::NativeTypeResolver<T>::Get();
		OE_ASSERT(type)
		auto it = m_Allocators.find(type);
		if (it != m_Allocators.end())
			return static_cast<Memory::TrackedMemoryPool<T>*>((*it).second);
		return nullptr;
	}

	template<typename T>
	inline Memory::TrackedMemoryPool<T>* MemoryDomain::CreateAllocator()
	{
		Meta::NativeType* type = Meta::NativeTypeResolver<T>::Get();
		OE_ASSERT(type)
			Memory::TrackedMemoryPool<T>* allocator = new Memory::TrackedMemoryPool<T>(100);

		m_Allocators.insert(std::make_pair(type, allocator));

		return allocator;
	}
} }

#endif