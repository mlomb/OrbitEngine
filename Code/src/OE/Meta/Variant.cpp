#include "OE/Meta/Variant.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Meta {
	Variant::Variant()
		: m_Kind(INVALID), m_Null(true)
	{
	}

	Variant::Variant(const Variant& v)
	{
		construct(&v.m_Data, sizeof(Data), v.m_Kind);
	}

	Variant::Variant(const Kind kind)
		: m_Kind(kind), m_Null(true)
	{
	}

	Variant::Variant(const void* data, const unsigned int size, const Kind kind)
	{
		construct(data, size, kind);
	}

	Variant::Variant(const bool b)
		: m_Kind(BOOL), m_Null(false)
	{
		m_Data.b = b;
	}

	Variant::Variant(const char c)
		: m_Kind(CHAR), m_Null(false)
	{
		m_Data.c = c;
	}

	Variant::Variant(const int i)
		: m_Kind(INT), m_Null(false)
	{
		m_Data.i = i;
	}

	Variant::Variant(const unsigned int ui)
		: m_Kind(UINT), m_Null(false)
	{
		m_Data.ui = ui;
	}

	Variant::Variant(const long l)
		: m_Kind(LONG), m_Null(false)
	{
		m_Data.l = l;
	}

	Variant::Variant(const long long ll)
		: m_Kind(LONGLONG), m_Null(false)
	{
		m_Data.ll = ll;
	}

	Variant::Variant(const unsigned long ul)
		: m_Kind(ULONG), m_Null(false)
	{
		m_Data.ul = ul;
	}

	Variant::Variant(const unsigned long long ull)
		: m_Kind(ULONGLONG), m_Null(false)
	{
		m_Data.ull = ull;
	}

	Variant::Variant(const double d)
		: m_Kind(DOUBLE), m_Null(false)
	{
		m_Data.d = d;
	}

	Variant::Variant(const float f)
		: m_Kind(FLOAT), m_Null(false)
	{
		m_Data.f = f;
	}

	Variant::Variant(const std::string& str)
		: m_Kind(STRING), m_Null(false)
	{
		m_Data.str = new std::string(str);
	}

	Variant::Variant(const Math::Vec2i& v2i)
		: m_Kind(VEC2_INT), m_Null(false)
	{
		m_Data.v2i = v2i;
	}

	Variant::Variant(const Math::Vec3i& v3i)
		: m_Kind(VEC3_INT), m_Null(false)
	{
		m_Data.v3i = v3i;
	}

	Variant::Variant(const Math::Vec4i& v4i)
		: m_Kind(VEC4_INT), m_Null(false)
	{
		m_Data.v4i = v4i;
	}

	Variant::Variant(const Math::Vec2f& v2f)
		: m_Kind(VEC2_FLOAT), m_Null(false)
	{
		m_Data.v2f = v2f;
	}

	Variant::Variant(const Math::Vec3f& v3f)
		: m_Kind(VEC3_FLOAT), m_Null(false)
	{
		m_Data.v3f = v3f;
	}

	Variant::Variant(const Math::Vec4f& v4f)
		: m_Kind(VEC4_FLOAT), m_Null(false)
	{
		m_Data.v4f = v4f;
	}

	Variant::Variant(void* ptr)
		: m_Kind(POINTER), m_Null(ptr == 0)
	{
		m_Data.ptr = ptr;
	}

	Variant& Variant::operator=(const Variant& v) {
		if (this == &v)
			return *this;

		clear();
		construct(&v.m_Data, sizeof(Data), v.m_Kind);

		return *this;
	}

	Variant::~Variant()
	{
		clear();
	}

	Kind Variant::GetKind() const
	{
		return m_Kind;
	}

	bool Variant::IsValid() const
	{
		return m_Kind != INVALID;
	}

	bool Variant::IsNull() const
	{
		return m_Null;
	}

	bool Variant::GetBool() const
	{
		return m_Data.b;
	}

	char Variant::GetChar() const
	{
		return m_Data.c;
	}

	int Variant::GetInt() const
	{
		return m_Data.i;
	}

	unsigned int Variant::GetUInt() const
	{
		return m_Data.ui;
	}

	long Variant::GetLong() const
	{
		return m_Data.l;
	}

	long long Variant::GetLongLong() const
	{
		return m_Data.ll;
	}

	unsigned long Variant::GetULong() const
	{
		return m_Data.ul;
	}

	unsigned long long Variant::GetULongLong() const
	{
		return m_Data.ull;
	}

	double Variant::GetDouble() const
	{
		return m_Data.d;
	}

	float Variant::GetFloat() const
	{
		return m_Data.f;
	}

	Math::Vec2i Variant::GetVec2i() const
	{
		return m_Data.v2i;
	}

	Math::Vec3i Variant::GetVec3i() const
	{
		return m_Data.v3i;
	}

	Math::Vec4i Variant::GetVec4i() const
	{
		return m_Data.v4i;
	}

	Math::Vec2f Variant::GetVec2f() const
	{
		return m_Data.v2f;
	}

	Math::Vec3f Variant::GetVec3f() const
	{
		return m_Data.v3f;
	}

	Math::Vec4f Variant::GetVec4f() const
	{
		return m_Data.v4f;
	}

	void* Variant::GetPointer() const
	{
		if (m_Null)
			return 0;
		return m_Data.ptr;
	}

	std::string Variant::GetString() const
	{
		if (m_Null)
			return "<null>";

		switch (m_Kind) {
		case Kind::STRING:
			return std::string(*m_Data.str);
		case Kind::BOOL:
			return m_Data.b ? "true" : "false";
		case Kind::CHAR:
			return std::to_string(m_Data.c);
		case Kind::INT:
			return std::to_string(m_Data.i);
		case Kind::UINT:
			return std::to_string(m_Data.ui);
		case Kind::LONG:
			return std::to_string(m_Data.l);
		case Kind::LONGLONG:
			return std::to_string(m_Data.ll);
		case Kind::ULONG:
			return std::to_string(m_Data.ul);
		case Kind::ULONGLONG:
			return std::to_string(m_Data.ull);
		case Kind::DOUBLE:
			return std::to_string(m_Data.d);
		case Kind::FLOAT:
			return std::to_string(m_Data.f);
		case Kind::VEC2_INT:
			return "(" + std::to_string(m_Data.v2i.x) + ", " + std::to_string(m_Data.v2i.y) + ")";
		case Kind::VEC2_FLOAT:
			return "(" + std::to_string(m_Data.v2f.x) + ", " + std::to_string(m_Data.v2f.y) + ")";
		case Kind::VEC3_INT:
			return "(" + std::to_string(m_Data.v3i.x) + ", " + std::to_string(m_Data.v3i.y) + ", " + std::to_string(m_Data.v3i.z) + ")";
		case Kind::VEC3_FLOAT:
			return "(" + std::to_string(m_Data.v3f.x) + ", " + std::to_string(m_Data.v3f.y) + ", " + std::to_string(m_Data.v3f.z) + ")";
		case Kind::VEC4_INT:
			return "(" + std::to_string(m_Data.v4i.x) + ", " + std::to_string(m_Data.v4i.y) + ", " + std::to_string(m_Data.v4i.z) + ", " + std::to_string(m_Data.v4i.w) + ")";
		case Kind::VEC4_FLOAT:
			return "(" + std::to_string(m_Data.v4f.x) + ", " + std::to_string(m_Data.v4f.y) + ", " + std::to_string(m_Data.v4f.z) + ", " + std::to_string(m_Data.v4f.w) + ")";
		default:
			return std::string();
		}
	}

	void* Variant::GetDataPointer() const
	{
		return (void*)&m_Data;
	}

	void Variant::construct(const void* data, const unsigned int size, const Kind kind)
	{
		clear();

		m_Kind = kind;
		m_Null = data == 0 || size == 0;

		if (!m_Null) {
			const Data* d = static_cast<const Data*>(data);
			if (m_Kind == Kind::STRING) {
				m_Data.str = new std::string(*d->str);
			}
			else {
				// raw copy
				memcpy(&m_Data, d, std::min(size, (unsigned int)sizeof(Data)));
			}
		}
	}

	void Variant::clear()
	{
		if (!m_Null) {
			if (m_Kind == STRING)
				delete m_Data.str;
		}

		m_Kind = INVALID;
		m_Null = true;
	}

	bool operator==(const Variant& lhs, const Variant& rhs)
	{
		if (lhs.m_Kind != rhs.m_Kind) return false;
		if (lhs.m_Null != rhs.m_Null) return false;
		if (lhs.m_Null)
			return true;
		else {
			if (lhs.m_Kind == STRING) {
				return (*lhs.m_Data.str) == (*rhs.m_Data.str);
			} else
				// TODO Change
				return memcmp(&lhs.m_Data, &rhs.m_Data, sizeof(Variant::Data)) == 0;
		}
	}

	bool operator!=(const Variant& lhs, const Variant& rhs)
	{
		return !(lhs == rhs);
	}
} }