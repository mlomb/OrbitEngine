#include "OE/Meta/NativeReflection.hpp"

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Vec4.hpp"

namespace OrbitEngine { namespace Meta {
	namespace internal {

		#define NATIVE_TYPE(klass, name, kind) \
		class NativeType_##name : public Meta::NativeType { \
		public: \
			NativeType_##name() : Meta::NativeType(#klass, kind, sizeof(klass)) { \
			}  \
		}; \
		template<> \
		Meta::NativeType* GetPrimitiveNativeType<klass>() { \
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

	}
} }
