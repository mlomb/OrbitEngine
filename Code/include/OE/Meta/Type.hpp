#ifndef META_TYPE_HPP
#define META_TYPE_HPP

#include <string>
#include <vector>

namespace OrbitEngine { namespace Meta {
	class Member;

	enum Kind {
		INVALID,
		POINTER,

		BOOL,
		CHAR,
		INT,
		UINT,
		LONG,
		LONGLONG,
		ULONG,
		ULONGLONG,
		DOUBLE,
		FLOAT,

		STRING,
		VEC2_INT,
		VEC2_FLOAT,
		VEC3_INT,
		VEC3_FLOAT,
		VEC4_INT,
		VEC4_FLOAT,

		CLASS
	};

	class Type {
	public:
		Type();
		Type(const std::string& name, const Kind kind);

		void SetKind(const Kind kind);
		void SetName(const std::string& name);
		void AddMember(Member* member);

		Kind GetKind() const;
		std::string GetName() const;
		Member* GetMember(const std::string& member_name) const;
		const std::vector<Member*>& GetMembers() const;

	private:
		Kind m_Kind;
		std::string m_Name;
		std::vector<Member*> m_Members;
	};

} }

#endif