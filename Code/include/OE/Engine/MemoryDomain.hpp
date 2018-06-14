#ifndef ENGINE_MEMORY_DOMAIN_HPP
#define ENGINE_MEMORY_DOMAIN_HPP

#include <unordered_map>

#include "OE/Meta/NativeType.hpp"
#include "OE/Misc/MemoryPool.hpp"
#include "OE/Misc/Ptrs.hpp"

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

namespace OrbitEngine { namespace Engine {

	class MemoryDomain {
	public:
		MemoryDomain();
		~MemoryDomain();

		static MemoryDomain* Get();

		void* Allocate(Meta::NativeType* type);
		void Deallocate(void* object, Meta::NativeType* type);

		template<typename T> StrongPtr<T> Allocate();
		template<typename T> void Deallocate(StrongPtr<T>& object);

	private:
		std::unordered_map<Meta::NativeType*, Misc::MemoryPool*> m_Allocators;

		Misc::MemoryPool* GetAllocator(Meta::NativeType* type);
		Misc::MemoryPool* CreateAllocator(Meta::NativeType* type);
	};

	template<typename T>
	inline StrongPtr<T> MemoryDomain::Allocate()
	{
		return StrongPtr<T>(static_cast<T*>(Allocate(Meta::NativeTypeResolver<T>::Get())), this);
	}

	template<typename T>
	inline void MemoryDomain::Deallocate(StrongPtr<T>& object)
	{
		Deallocate(object.Get(), object.GetType());
	}
} }

#endif