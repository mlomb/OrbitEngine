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
} }
