#include "OE/Engine/MemoryDomain.hpp"

#include "OE/Meta/NativeReflection.hpp"

#include "OE/Engine/SceneObject.hpp"
#include "OE/Engine/EngineDomain.hpp"

namespace OrbitEngine { namespace Engine {

	MemoryDomain::MemoryDomain()
	{
	}
	
	MemoryDomain::~MemoryDomain()
	{
		for (auto& kv : m_Allocators)
			delete kv.second;
	}

	MemoryDomain* MemoryDomain::Get()
	{
		return EngineDomain::Get()->GetMemoryDomain();
	}

	void* MemoryDomain::Allocate(Meta::NativeType* type)
	{
		OE_ASSERT(type);

		Misc::MemoryPool* allocator = GetAllocator(type);
		if (!allocator)
			allocator = CreateAllocator(type);

		void* object = allocator->Allocate();
		if(object)
			type->Construct(object);
		return object;
	}

	void MemoryDomain::Deallocate(void* object, Meta::NativeType* type)
	{
		OE_ASSERT(type);

		Misc::MemoryPool* allocator = GetAllocator(type);
		OE_ASSERT(allocator);

		type->Destruct(object);
		allocator->Deallocate(object);
	}

	Misc::MemoryPool* MemoryDomain::GetAllocator(Meta::NativeType* type)
	{
		auto it = m_Allocators.find(type);
		if (it != m_Allocators.end())
			return (*it).second;
		return nullptr;
	}

	Misc::MemoryPool* MemoryDomain::CreateAllocator(Meta::NativeType* type)
	{
		Misc::MemoryPool* allocator = new Misc::TrackedMemoryPool(type->GetSize(), 10);
		m_Allocators.insert(std::make_pair(type, allocator));
		return allocator;
	}

} }
