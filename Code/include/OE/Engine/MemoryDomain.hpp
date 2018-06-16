#ifndef ENGINE_MEMORY_DOMAIN_HPP
#define ENGINE_MEMORY_DOMAIN_HPP

#include <unordered_map>
#include <math.h>

#include "OE/Meta/NativeType.hpp"
#include "OE/Memory/MemoryPool.hpp"

namespace std {
	template <>
	struct hash<OrbitEngine::Meta::NativeType*>
	{
		std::size_t operator()(const OrbitEngine::Meta::NativeType* val) const
		{
			static const size_t shift = (size_t)log2(1 + sizeof(OrbitEngine::Meta::NativeType));
			return (size_t)(val) >> shift;
		}
	};
}

namespace OrbitEngine {
	template<typename T>
	class Ptr;
	template<typename T>
	class WeakPtr;
	template<typename T>
	class StrongPtr;
}

namespace OrbitEngine { namespace Engine {

	class MemoryDomain {
	public:
		MemoryDomain();
		~MemoryDomain();

		static MemoryDomain* Get();

		template<typename T>
		StrongPtr<T> Create();
		template<typename T>
		void Destroy(Ptr<T>& object);
		template<typename T>
		const std::vector<WeakPtr<T>>& GetAll();

	private:
		std::unordered_map<Meta::NativeType*, Memory::Allocator*> m_Allocators;
		std::unordered_map<Meta::NativeType*, void* /* std::vector<WeakPtr<T>> */> m_Objects;

		template<typename T>
		std::vector<WeakPtr<T>>& GetObjects();

		template<typename T>
		Memory::MemoryPool* GetAllocator();
		template<typename T>
		Memory::MemoryPool* CreateAllocator();
	};
} }

#include "OE/Engine/MemoryDomain.inl"

#endif