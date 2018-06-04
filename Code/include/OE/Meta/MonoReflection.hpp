#ifndef META_MONO_REFLECTION_HPP
#define META_MONO_REFLECTION_HPP

#include "OE/Meta/MonoType.hpp"
#include "OE/Meta/MonoMember.hpp"
#include "OE/Meta/ReflectionDatabase.hpp"

namespace OrbitEngine { namespace Meta {
	class MonoReflection {
	public:
		static MonoType* Reflect(::MonoType* _type, ReflectionDatabase* reflectionDatabase);
		static void ReflectAll(::MonoImage* image, ReflectionDatabase* reflectionDatabase);
	};
} }

#endif