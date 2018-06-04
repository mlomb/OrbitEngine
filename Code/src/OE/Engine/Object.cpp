#include "OE/Engine/Object.hpp"

#include "OE/Scripting/Mono/MonoMapping.hpp"

namespace OrbitEngine { namespace Engine {
	/*
	Object::Object(MonoType* type) : Object(mono_ptr_class_get(type)) { }
	Object::Object(MonoClass* klass) : Object(mono_object_new(mono_domain_get(), klass)) { }

	Object::Object(MonoObject* obj) : m_Object(obj)
	{
		uintptr_t ptr = (uintptr_t)this;
		mono_field_set_value(obj, MonoMapping::FIELD_Object_pointer, &ptr);
	}
	*/
	NATIVE_REFLECTION_BEGIN(Object)
	NATIVE_REFLECTION_END()
} }
