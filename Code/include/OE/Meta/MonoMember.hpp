#ifndef META_MONO_MEMBER_HPP
#define META_MONO_MEMBER_HPP

#include <string>

#include "Member.hpp"
#include "MonoType.hpp"

#include "OE/Scripting/Mono/Mono.hpp"

namespace OrbitEngine { namespace Meta {

	class MonoMember : public Member {
	public:
		MonoMember(const std::string& name, MonoType* type, ::MonoClassField* field);

		Variant Get(void* object) const override;
		void Set(const Variant& value, void* object) const override;

	private:
		::MonoClassField* m_Field;
	};

} }

#endif