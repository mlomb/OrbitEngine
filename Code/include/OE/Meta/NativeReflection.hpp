#ifndef META_NATIVE_RELFECTION
#define META_NATIVE_RELFECTION

#include <stddef.h>

#include "OE/Meta/ReflectionDatabase.hpp"

#include "OE/Meta/NativeType.hpp"
#include "OE/Meta/NativeMember.hpp"

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Vec4.hpp"

namespace OrbitEngine { namespace Meta {
	namespace internal {

		template<typename T>
		NativeType* GetPrimitiveNativeType() { return 0; };

		#define NATIVE_TYPE(klass, name, kind) \
		class NativeType_##name : public Meta::NativeType { \
		public: \
			NativeType_##name() : Meta::NativeType(#klass, kind, sizeof(klass)) { \
			}  \
		}; \
		template <> \
		inline Meta::NativeType* GetPrimitiveNativeType<klass>() { \
			static NativeType_##name* type = 0; \
			if(type == 0) {  \
				type = new NativeType_##name();  \
				ReflectionDatabase::AddNativeType(type); \
			} \
			return type; \
		} \

		NATIVE_TYPE(std::string, string, STRING)
		NATIVE_TYPE(bool, bool, BOOL)
		NATIVE_TYPE(char, char, CHAR)
		NATIVE_TYPE(int, int, INT)
		NATIVE_TYPE(unsigned int, uint, UINT)
		NATIVE_TYPE(long, long, LONG)
		NATIVE_TYPE(long long, longlong, LONGLONG)
		NATIVE_TYPE(unsigned long, ulong, ULONG)
		NATIVE_TYPE(unsigned long long, ulonglong, ULONGLONG)
		NATIVE_TYPE(double, double, DOUBLE)
		NATIVE_TYPE(float, float, FLOAT)

		NATIVE_TYPE(Math::Vec2i, Vec2i, VEC2_INT)
		NATIVE_TYPE(Math::Vec3i, Vec3i, VEC3_INT)
		NATIVE_TYPE(Math::Vec4i, Vec4i, VEC4_INT)
		NATIVE_TYPE(Math::Vec2f, Vec2f, VEC2_FLOAT)
		NATIVE_TYPE(Math::Vec3f, Vec3f, VEC3_FLOAT)
		NATIVE_TYPE(Math::Vec4f, Vec4f, VEC4_FLOAT)

		struct NativeTypeResolver {
			template <typename T> static char func(decltype(&T::Reflection));
			template <typename T> static int func(...);
			template <typename T>
			struct IsReflected {
				enum {
					value = (sizeof(func<T>(nullptr)) == sizeof(char))
				};
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


#define NATIVE_REFLECTION \
	public: \
		static Meta::NativeType Reflection; \
	private: \
		static void meta_reflect(Meta::NativeType*); \
		static void meta_ctor(void* ptr); \
		static void meta_dctor(void* ptr); \

#define NATIVE_REFLECTION_BEGIN(type) \
	void type::meta_ctor (void* ptr) { new (ptr) type (); } \
	void type::meta_dctor (void* ptr) { static_cast<type*>(ptr)-> type ::~type (); } \
	Meta::NativeType type::Reflection{type::meta_reflect}; \
	void type::meta_reflect(Meta::NativeType* t) { \
		using T = type; \
		t->SetKind(Meta::Kind::CLASS); \
		t->SetCtorAndDctor(meta_ctor, meta_dctor); \
		t->SetName(#type); \
		t->SetSize(sizeof(T)); \

#define NATIVE_REFLECTION_MEMBER(member_name) \
		t->AddMember(new Meta::NativeMember(#member_name, offsetof(T, member_name), Meta::NativeTypeResolver<decltype(T::member_name)>::Get())); \

#define NATIVE_REFLECTION_END() \
		Meta::ReflectionDatabase::AddNativeType(t);	\
	} \

#endif