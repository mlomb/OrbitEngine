#pragma once

#include <string>
#include <vector>

#include "OE/Scripting/Mono/Mono.hpp"
#include "OE/Scripting/Mono/MonoAssembly.hpp"

namespace OrbitEngine {
	namespace Engine {
		class Object;
	}

	class MonoDomain {
	public:
		MonoDomain();
		~MonoDomain();

		static void SetMonoPath(std::string monoPath);

		MonoAssembly* LoadAssembly(const char* filepath);
		MonoAssembly* GetMainAssembly();
		::MonoDomain* GetNativeDomain();

		::MonoObject* Allocate(MonoClass* klass);
		::MonoObject* WrapObject(Engine::Object* obj, MonoClass* klass);
		::MonoString* CreateString(const std::string& str = "");
		::MonoArray* CreateArray(MonoClass* klass, const int count);

		template<typename T>
		::MonoArray* CreateArray(MonoClass* klass, const std::vector<T*>& objs);


	private:
		::MonoDomain* m_Domain;

		std::vector<MonoAssembly*> m_Assemblies;
	};

	template<typename T>
	inline ::MonoArray* MonoDomain::CreateArray(MonoClass* klass, const std::vector<T*>& objs)
	{
		MonoArray* arr = CreateArray(klass, int(objs.size()));

		int i = 0;
		for (T* t_obj : objs) {
			Engine::Object* e_obj = static_cast<Engine::Object*>(t_obj);
			MonoObject* m_obj = WrapObject(e_obj, klass);
			mono_array_setref(arr, i++, m_obj);
		}

		return arr;
	}
}