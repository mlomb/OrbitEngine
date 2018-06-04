#ifndef ENGINE_ENGINE_DOMAIN_HPP
#define ENGINE_ENGINE_DOMAIN_HPP

#include <unordered_map>

#include "OE/Engine/Scene.hpp"

#include "OE/Scripting/Mono/MonoDomain.hpp"

#include "OE/Meta/ReflectionDatabase.hpp"

namespace OrbitEngine { namespace Engine {
	class EngineDomain {
	public:
		EngineDomain();
		~EngineDomain();

		MonoDomain* GetMonoDomain();
		Meta::ReflectionDatabase* GetReflectionDatabase();
		static EngineDomain* GetEngineDomainFromNativeMonoDomain(::MonoDomain* native_md);

		Scene* GetActiveScene();

	private:
		MonoDomain* m_MonoDomain;
		Meta::ReflectionDatabase* m_ReflectionDatabase;

		Scene* m_ActiveScene;

		static std::unordered_map<::MonoDomain*, EngineDomain*> s_NativeMonoDomainToEngineDomain;
	};
} }

#endif