#pragma once

#include "OE/Scripting/Mono/Mono.hpp"

#include "OE/Misc/Log.hpp"

/* -------- Declarations -------- */

// Class
#define MONO_DEC_CLASS(className) \
static MonoClass* CLASS_##className;
#define MONO_DEC_CLASS_CPP(className) \
MonoClass* MonoMapping::CLASS_##className = nullptr;

// Field
#define MONO_DEC_FIELD(className, fieldName) \
static MonoClassField* FIELD_##className##_##fieldName;
#define MONO_DEC_FIELD_CPP(className, fieldName) \
MonoClassField* MonoMapping::FIELD_##className##_##fieldName = nullptr;

/* -------- Definitions -------- */

// Class
#define MONO_DEF_CLASS(nameSpace, className) \
{ \
	CLASS_##className = assembly->GetClass(""#nameSpace, ""#className); \
	if(!CLASS_##className) \
		OE_LOG_FATAL_SHOWBOX_AND_EXIT("Couldn't resolve class '" << ""#nameSpace << "." << ""#className << "'") \
}

// Field
#define MONO_DEF_FIELD(className, fieldName) \
{ \
	FIELD_##className##_##fieldName = assembly->GetField(CLASS_##className, ""#fieldName); \
	if(!FIELD_##className##_##fieldName) \
		OE_LOG_FATAL_SHOWBOX_AND_EXIT("Couldn't get field '"#fieldName << "' from class '" << ""#className << "'") \
}







/* -------- ADD CALL -------- */
#define MONO_ADD_CALL(nameSpace, className, methodName, callName) \
mono_add_internal_call((std::string() + ""#nameSpace + "." + ""#className + "::"#methodName).c_str(), reinterpret_cast<const void*>(INTERNAL_CALL_##callName));

#define MONO_ADD_INTERNAL_CALL(nameSpace, className, methodName) \
MONO_ADD_CALL(nameSpace, className, INTERNAL_CALL_##methodName, methodName)

/* -------- STRING PROPERTY -------- */
#define MONO_PROPERTY_STRING_GET(className, cppFieldName, csFieldName) \
MonoString* ##className::INTERNAL_CALL_get_##csFieldName(MonoObject* _obj) \
{ \
	##className* obj = GetPointerFromMonoObject<##className>(_obj); \
	if (!obj) \
		return nullptr; \
	return mono_string_new(mono_domain_get(), obj->##cppFieldName.c_str()); \
}

#define MONO_PROPERTY_STRING_SET(className, cppFieldName, csFieldName) \
void  className::INTERNAL_CALL_set_##csFieldName(MonoObject* _obj, MonoString* str) \
{ \
	##className* obj = GetPointerFromMonoObject<##className>(_obj); \
	if (obj) \
		obj->##cppFieldName = mono_string_to_utf8(str); \
}

#define MONO_PROPERTY_STRING_GET_SET(className, cppFieldName, csFieldName) MONO_PROPERTY_STRING_GET(className, cppFieldName, csFieldName) MONO_PROPERTY_STRING_SET(className, cppFieldName, csFieldName)