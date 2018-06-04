#ifndef META_NATIVE_MEMBER_HPP
#define META_NATIVE_MEMBER_HPP

#include "Type.hpp"
#include "Member.hpp"

namespace OrbitEngine { namespace Meta {

	class NativeMember : public Member {
	public:
		NativeMember(const std::string& name, const unsigned int offset, Type* type);

		Variant Get(void* object) const override;
		void Set(const Variant& value, void* object) const override;

	private:
		unsigned int m_Offset;
	};

} }

#endif