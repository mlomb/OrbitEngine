#include "OE/Meta/Type.hpp"

#include "OE/Meta/Member.hpp"

namespace OrbitEngine { namespace Meta {
	Type::Type()
		: m_Name("_unknown")
	{
	}
	Type::Type(const std::string& name, const Kind kind)
		: m_Name(name), m_Kind(kind)
	{
	}

	void Type::SetKind(const Kind kind)
	{
		m_Kind = kind;
	}

	void Type::SetName(const std::string& name)
	{
		m_Name = name;
	}

	void Type::AddMember(Member* member)
	{
		m_Members.push_back(member);
	}

	Kind Type::GetKind() const
	{
		return m_Kind;
	}

	std::string Type::GetName() const
	{
		return m_Name;
	}

	Member* Type::GetMember(const std::string& member_name) const
	{
		for (Member* m : m_Members) {
			if (m->GetName() == member_name)
				return m;
		}
		return nullptr;
	}

	const std::vector<Member*>& Type::GetMembers() const {
		return m_Members;
	}
	
} }
