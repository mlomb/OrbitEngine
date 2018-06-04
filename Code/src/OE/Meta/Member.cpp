#include "OE/Meta/Member.hpp"

#include "OE/Meta/Variant.hpp"

namespace OrbitEngine { namespace Meta {
	Member::Member()
		: m_Name(""), m_Type(NULL)
	{
	}
	Member::Member(const std::string& name, Type* type)
		: m_Name(name), m_Type(type)
	{
	}

	Type* Member::GetType() const
	{
		return m_Type;
	}

	std::string Member::GetName() const
	{
		return m_Name;
	}

	Variant Member::Get(void* object) const
	{
		return Variant();
	}

	void Member::Set(const Variant& value, void* object) const
	{
	}
} }
