#include "OE/Scripting/Mono/MonoMapping.hpp"

#include "OE/Scripting/Mono/Mono.hpp"

#include "OE/Scripting/Mono/MonoAssembly.hpp"
#include "OE/Engine/EngineDomain.hpp"

#include "OE/Engine/Object.hpp"
#include "OE/Engine/Scene.hpp"
#include "OE/Engine/SceneObject.hpp"

namespace OrbitEngine {
	MONO_DEC_CLASS_CPP(Object)
		MONO_DEC_FIELD_CPP(Object, pointer)

	MONO_DEC_CLASS_CPP(Scene)

	MONO_DEC_CLASS_CPP(SceneObject)

	MONO_DEC_CLASS_CPP(EngineDomain)

	Engine::EngineDomain* GetEngineDomain() {
		::MonoDomain* native_md = mono_domain_get();
		return Engine::EngineDomain::GetEngineDomainFromNativeMonoDomain(native_md);
	}

	MonoDomain* GetMonoDomain() {
		return GetEngineDomain()->GetMonoDomain();
	}

	template<typename T> T* GetPointerFromMonoObject(MonoObject* _obj)
	{
		void* ptr;
		mono_field_get_value(_obj, MonoMapping::FIELD_Object_pointer, &ptr);
		Engine::Object* obj = reinterpret_cast<Engine::Object*>(ptr);
		return static_cast<T*>(obj);
	}

	void INTERNAL_CALL_GetActiveScene(MonoObject** s) {
		Engine::EngineDomain* ed = GetEngineDomain();

		Engine::Scene* scene = ed->GetActiveScene();
		*s = ed->GetMonoDomain()->WrapObject(scene, MonoMapping::CLASS_Scene);
	}

	void INTERNAL_CALL_GetRoot(MonoObject* caller, MonoObject** so) {
		//Engine::Scene* s = GetPointerFromMonoObject<Engine::Scene>(caller);
		//*so = GetMonoDomain()->WrapObject(s->GetRoot(), MonoMapping::CLASS_SceneObject);
	}

	MonoString* INTERNAL_CALL_get_Test(MonoObject* caller) {
		Engine::Scene* s = GetPointerFromMonoObject<Engine::Scene>(caller);
		return GetMonoDomain()->CreateString(std::string("Num of scene objects in root: ") + std::to_string(s->GetRoot()->GetChildCount()));
	}

	MonoString* INTERNAL_CALL_get_Name(MonoObject* caller) {
		Engine::SceneObject* so = GetPointerFromMonoObject<Engine::SceneObject>(caller);
		return GetMonoDomain()->CreateString(so->GetName());
	}

	MonoArray* INTERNAL_CALL_GetChildrens(MonoObject* caller) {
		Engine::SceneObject* so = GetPointerFromMonoObject<Engine::SceneObject>(caller);
		
		//std::vector<Engine::SceneObject*>& childs = so->getChildrens();
		//
		//MonoArray* arr = GetMonoDomain()->CreateArray(MonoMapping::CLASS_SceneObject, childs);
		//
		//return arr;
		return 0;
	}

	void MonoMapping::Map(MonoAssembly* assembly)
	{
		/* Definitions */
		MONO_DEF_CLASS(OrbitEngine, Object)
			MONO_DEF_FIELD(Object, pointer)

		// Scene
		MONO_DEF_CLASS(OrbitEngine, Scene)
			MONO_ADD_CALL(OrbitEngine, Scene, get_Test, get_Test)
			MONO_ADD_INTERNAL_CALL(OrbitEngine, Scene, GetRoot)

		// SceneObject
		MONO_DEF_CLASS(OrbitEngine, SceneObject)
			MONO_ADD_CALL(OrbitEngine, SceneObject, get_Name, get_Name)
			MONO_ADD_CALL(OrbitEngine, SceneObject, GetChildrens, GetChildrens)

		// EngineDomain
		MONO_DEF_CLASS(OrbitEngine, EngineDomain)
			MONO_ADD_INTERNAL_CALL(OrbitEngine, EngineDomain, GetActiveScene)
	}
};
