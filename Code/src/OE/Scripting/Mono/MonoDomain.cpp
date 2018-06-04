#include "OE/Scripting/Mono/MonoDomain.hpp"

#include "OE/Scripting/Mono/MonoMapping.hpp"

#include "OE/Engine/Object.hpp"

namespace OrbitEngine {

	MonoDomain::MonoDomain()
	{
		SetMonoPath(std::string(MONO_PATH));

		m_Domain = mono_jit_init_version("OrbitEngineEmbedded", "v4.0.30319");

		// Load the core C# wrapper.
		MonoMapping::Map(LoadAssembly("OrbitEngine.dll"));
	}

	MonoDomain::~MonoDomain()
	{
		m_Assemblies.clear();
		mono_jit_cleanup(m_Domain);
		m_Domain = nullptr;
	}

	void MonoDomain::SetMonoPath(std::string monoPath)
	{
		std::string MONO_PATH_LIB = monoPath + "/lib";
		std::string MONO_PATH_ETC = monoPath + "/etc";

		mono_set_dirs(MONO_PATH_LIB.c_str(), MONO_PATH_ETC.c_str());
	}
	
	MonoAssembly* MonoDomain::LoadAssembly(const char* filepath)
	{
		MonoAssembly* assembly = new MonoAssembly(("D:/Users/Desktop/Mono Test/CS/" + std::string(filepath)).c_str()); // TODO This is a temp path
		m_Assemblies.push_back(assembly);
		return assembly;
	}

	MonoAssembly* MonoDomain::GetMainAssembly()
	{
		return m_Assemblies.at(0);
	}

	::MonoDomain* MonoDomain::GetNativeDomain()
	{
		return m_Domain;
	}

	MonoObject* MonoDomain::Allocate(MonoClass* klass)
	{
		return mono_object_new(m_Domain, klass);
	}

	::MonoObject* MonoDomain::WrapObject(Engine::Object* obj, MonoClass* klass)
	{
		if (obj->m_Object == 0) {
			obj->m_Object = Allocate(klass);

			mono_runtime_object_init(obj->m_Object);
			mono_field_set_value(obj->m_Object, MonoMapping::FIELD_Object_pointer, &obj);
		}
		return obj->m_Object;
	}

	::MonoString* MonoDomain::CreateString(const std::string& str)
	{
		return mono_string_new(m_Domain, str.c_str());
	}

	::MonoArray* MonoDomain::CreateArray(MonoClass* klass, const int count)
	{
		return mono_array_new(m_Domain, klass, count);
	}
}