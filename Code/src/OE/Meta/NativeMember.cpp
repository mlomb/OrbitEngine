#include "OE/Meta/NativeMember.hpp"

#include "OE/Meta/Variant.hpp"
#include "OE/Engine/SceneObject.hpp"

namespace OrbitEngine { namespace Meta {
	NativeMember::NativeMember(const std::string& name, const unsigned int offset, Type* type)
		: Member(name, type), m_Offset(offset)
	{
	}

	Variant NativeMember::Get(void* object) const
	{
		Kind kind = m_Type->GetKind();
		if (kind == Kind::INVALID || object == 0)
			return Variant();

		void* ptr = (char*)object + m_Offset;

#define NATIVE_MEMBER_GET(kind, type) \
		case Kind::kind: return Variant(*static_cast< type *>(ptr)); \

		switch (kind) {
		NATIVE_MEMBER_GET(BOOL, bool);
		NATIVE_MEMBER_GET(CHAR, char);
		NATIVE_MEMBER_GET(INT, int);
		NATIVE_MEMBER_GET(UINT, unsigned int);
		NATIVE_MEMBER_GET(LONG, long);
		NATIVE_MEMBER_GET(LONGLONG, long long);
		NATIVE_MEMBER_GET(ULONG, unsigned long);
		NATIVE_MEMBER_GET(ULONGLONG, unsigned long long);
		NATIVE_MEMBER_GET(DOUBLE, double);
		NATIVE_MEMBER_GET(FLOAT, float);
		NATIVE_MEMBER_GET(STRING, std::string);
		NATIVE_MEMBER_GET(VEC2_INT, Math::Vec2i);
		NATIVE_MEMBER_GET(VEC3_INT, Math::Vec3i);
		NATIVE_MEMBER_GET(VEC4_INT, Math::Vec4i);
		NATIVE_MEMBER_GET(VEC2_FLOAT, Math::Vec2f);
		NATIVE_MEMBER_GET(VEC3_FLOAT, Math::Vec3f);
		NATIVE_MEMBER_GET(VEC4_FLOAT, Math::Vec4f);

		case Kind::CLASS:
			return Variant(ptr);
		default:
		case Kind::INVALID:
			return Variant();
		}
	}

	void NativeMember::Set(const Variant& value, void* object) const
	{
		Kind kind = m_Type->GetKind();
		if (kind == Kind::INVALID || object == 0)
			return;
		if (!value.IsValid())
			return;

		void* ptr = (char*)object + m_Offset;

#define NATIVE_MEMBER_SET(kind, type, getter) \
		case Kind::kind: *static_cast< type *>(ptr) = value.getter(); break; \

		switch (kind) {
		NATIVE_MEMBER_SET(BOOL, bool, GetBool);
		NATIVE_MEMBER_SET(CHAR, char, GetChar);
		NATIVE_MEMBER_SET(INT, int, GetInt);
		NATIVE_MEMBER_SET(UINT, unsigned int, GetUInt);
		NATIVE_MEMBER_SET(LONG, long, GetLong);
		NATIVE_MEMBER_SET(LONGLONG, long long, GetLongLong);
		NATIVE_MEMBER_SET(ULONG, unsigned long, GetULong);
		NATIVE_MEMBER_SET(ULONGLONG, unsigned long long, GetULongLong);
		NATIVE_MEMBER_SET(DOUBLE, double, GetDouble);
		NATIVE_MEMBER_SET(FLOAT, float, GetFloat);
		NATIVE_MEMBER_SET(VEC2_INT, Math::Vec2i, GetVec2i);
		NATIVE_MEMBER_SET(VEC3_INT, Math::Vec3i, GetVec3i);
		NATIVE_MEMBER_SET(VEC4_INT, Math::Vec4i, GetVec4i);
		NATIVE_MEMBER_SET(VEC2_FLOAT, Math::Vec2f, GetVec2f);
		NATIVE_MEMBER_SET(VEC3_FLOAT, Math::Vec3f, GetVec3f);
		NATIVE_MEMBER_SET(VEC4_FLOAT, Math::Vec4f, GetVec4f);
		case Kind::STRING:
			std::string* str = static_cast<std::string*>(ptr);
			*str = value.GetString();
			break;
		}
	}
} }
