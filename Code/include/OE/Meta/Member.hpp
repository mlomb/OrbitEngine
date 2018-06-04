#ifndef META_MEMBER_HPP
#define META_MEMBER_HPP

#include <string>

#include "OE/Meta/Type.hpp"
#include "OE/Meta/Variant.hpp"

namespace OrbitEngine { namespace Meta {
	class Member {
	public:
		Member();
		Member(const std::string& name, Type* type);

		std::string GetName() const;
		Type* GetType() const;

		virtual Variant Get(void* object) const;
		virtual void Set(const Variant& value, void* object) const;

	protected:
		std::string m_Name;
		Type* m_Type;
	};

} }

#endif