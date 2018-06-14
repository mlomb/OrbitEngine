#include "OE/Engine/EngineDomain.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Engine/SceneObject.hpp"
#include "OE/Engine/MemoryDomain.hpp"

#include <fstream>
#include <chrono>

#include "OE/Meta/NativeReflection.hpp"
#include "OE/Meta/MonoReflection.hpp"

namespace OrbitEngine { namespace Engine {

	thread_local EngineDomain* EngineDomain::s_EngineDomain = 0;
	std::unordered_map<::MonoDomain*, EngineDomain*> EngineDomain::s_NativeMonoDomainToEngineDomain;

	EngineDomain::EngineDomain() {
		s_EngineDomain = this;

		m_MemoryDomain = new MemoryDomain();

		m_MonoDomain = new MonoDomain();
		m_ReflectionDatabase = new Meta::ReflectionDatabase();

		s_NativeMonoDomainToEngineDomain.insert(std::make_pair(m_MonoDomain->GetNativeDomain(), this));
		
		Meta::MonoReflection::ReflectAll(m_MonoDomain->GetMainAssembly()->GetMonoImage(), m_ReflectionDatabase);

		m_ActiveScene = new Scene();


		/* Run the Test */
		auto plugin = m_MonoDomain->LoadAssembly("Plugin.dll");
		Meta::MonoReflection::ReflectAll(plugin->GetMonoImage(), m_ReflectionDatabase);
		/*
		MonoClass* klass = plugin->GetClass("Plugin", "Test");
		MonoMethod* runTest = plugin->GetMethod(klass, "Run", 0);
		mono_runtime_invoke(runTest, NULL, NULL, NULL);
		*/
	}

	EngineDomain::~EngineDomain()
	{
		s_NativeMonoDomainToEngineDomain.erase(m_MonoDomain->GetNativeDomain());
		delete m_MonoDomain;

		s_EngineDomain = 0;
	}

	EngineDomain* EngineDomain::Get()
	{
		return s_EngineDomain;
	}

	MemoryDomain* EngineDomain::GetMemoryDomain()
	{
		return m_MemoryDomain;
	}

	MonoDomain* EngineDomain::GetMonoDomain()
	{
		return m_MonoDomain;
	}

	Meta::ReflectionDatabase* EngineDomain::GetReflectionDatabase()
	{
		return m_ReflectionDatabase;
	}

	EngineDomain* EngineDomain::GetEngineDomainFromNativeMonoDomain(::MonoDomain* native_md)
	{
		auto it = s_NativeMonoDomainToEngineDomain.find(native_md);
		if (it != s_NativeMonoDomainToEngineDomain.end())
			return (*it).second;
		return nullptr;
	}

	Scene* EngineDomain::GetActiveScene()
	{
		return m_ActiveScene;
	}
} }