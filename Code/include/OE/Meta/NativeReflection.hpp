#ifndef META_NATIVE_RELFECTION
#define META_NATIVE_RELFECTION

#include <stddef.h>

#include "OE/Meta/ReflectionDatabase.hpp"

#include "OE/Meta/NativeType.hpp"
#include "OE/Meta/NativeMember.hpp"

namespace OrbitEngine { namespace Meta {
	namespace internal {
		template <typename T>
		NativeType* GetPrimitiveNativeType();

		struct NativeTypeResolver {
			template <typename T> static char func(decltype(&T::Reflection));
			template <typename T> static int func(...);
			template <typename T>
			struct IsReflected {
				enum { value = (sizeof(func<T>(nullptr)) == sizeof(char)) };
			};

			template <typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
			static NativeType* Get() {
				return &T::Reflection;
			}

			template <typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
			static NativeType* Get() {
				return GetPrimitiveNativeType<T>();
			}
		};
	}

	template<typename T>
	struct NativeTypeResolver {
		static NativeType* Get() {
			return internal::NativeTypeResolver::Get<T>();
		}
	};
}
}


#define NATIVE_REFLECTION() \
	public: \
		static Meta::NativeType Reflection; \
	private: \
		static void meta_reflect(Meta::NativeType*);

#define NATIVE_REFLECTION_BEGIN(type) \
	Meta::NativeType type::Reflection{type::meta_reflect}; \
	void type::meta_reflect(Meta::NativeType* t) { \
		using T = type; \
		t->SetKind(Meta::Kind::CLASS); \
		t->SetName(#type); \
		t->SetSize(sizeof(T)); \

#define NATIVE_REFLECTION_MEMBER(member_name) \
		t->AddMember(new Meta::NativeMember(#member_name, offsetof(T, member_name), Meta::NativeTypeResolver<decltype(T::member_name)>::Get())); \

#define NATIVE_REFLECTION_END() \
		Meta::ReflectionDatabase::AddNativeType(t);	\
	} \

#endif