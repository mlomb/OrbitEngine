#include "OE/Meta/MonoReflection.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Meta {

	::MonoClass* mono_image_get_classes(::MonoImage *image, void* iter)
	{
		try {
			int num_types;
			int *index;

			if (!iter)
				return NULL;

			index = (int *)iter;

			// skip the <Module>
			if (!*index)
				*index = 1;

			num_types = mono_image_get_table_rows(image, MONO_TABLE_TYPEDEF);

			if (*index < num_types) {
				(*index)++;
				return mono_class_get(image, *index | MONO_TOKEN_TYPE_DEF);
			}
			else {
				*index = 0;
				return NULL;
			}
		}
		catch (...) {
			return 0;
		}
	}


	void ReflectFields(MonoType* type, ::MonoClass* klass, ReflectionDatabase* reflectionDatabase) {
		::MonoClassField *field;
		void* iter = NULL;
		
		while ((field = mono_class_get_fields(klass, &iter))) {
			std::string name = mono_field_get_name(field);
			uint32_t flags = mono_field_get_flags(field);
			::MonoType* mt = mono_field_get_type(field);

			static uint32_t field_attr_static = 0x0010;
			static uint32_t field_attr_public = 0x0006;

			if (flags & field_attr_static) continue;
			if (!(flags & field_attr_public)) continue;

			type->AddMember(new MonoMember(name, MonoReflection::Reflect(mt, reflectionDatabase), field));
		}
	}

	MonoType* MonoReflection::Reflect(::MonoType* _type, ReflectionDatabase* reflectionDatabase)
	{
		OE_ASSERT(_type);
		
		std::string name = mono_type_get_name(_type);
		MonoType* type = reflectionDatabase->GetMonoType(name);
		if (type)
			return type;

		MonoTypeEnum monoType = (MonoTypeEnum)mono_type_get_type(_type);
		Kind kind ;
		switch (monoType) {
		case MonoTypeEnum::MONO_TYPE_CLASS: kind = CLASS; break;
		case MonoTypeEnum::MONO_TYPE_STRING: kind = STRING; break;

		case MonoTypeEnum::MONO_TYPE_BOOLEAN: kind = BOOL; break;
		case MonoTypeEnum::MONO_TYPE_CHAR: kind = CHAR; break;

		case MonoTypeEnum::MONO_TYPE_I4: kind = INT; break;
		case MonoTypeEnum::MONO_TYPE_I8: kind = LONGLONG; break;
		case MonoTypeEnum::MONO_TYPE_U4: kind = UINT; break;
		case MonoTypeEnum::MONO_TYPE_U8: kind = ULONGLONG; break;

		case MonoTypeEnum::MONO_TYPE_R4: kind = FLOAT; break;
		case MonoTypeEnum::MONO_TYPE_R8: kind = DOUBLE; break;
		default:
			OE_LOG_WARNING("MonoType not supported: " << monoType);
			return 0;
		}

		type = new MonoType(name, kind, _type);
		reflectionDatabase->AddMonoType(type);

		if (kind == CLASS) {
			ReflectFields(type, mono_type_get_class(_type), reflectionDatabase);
		}

		return type;
	}

	void MonoReflection::ReflectAll(::MonoImage* image, ReflectionDatabase* reflectionDatabase)
	{
		MonoClass* i_klass;
		void* iter = NULL;
		while ((i_klass = mono_image_get_classes(image, &iter))) {
			Reflect(mono_class_get_type(i_klass), reflectionDatabase);
		}
	}
} }

