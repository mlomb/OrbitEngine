#ifndef META_MONO_TYPE_HPP
#define META_MONO_TYPE_HPP

#include "OE/Scripting/Mono/Mono.hpp"

#include "Type.hpp"

namespace OrbitEngine { namespace Meta {

	class MonoType : public Type {
	public:
		MonoType(const std::string& name, const Kind kind, ::MonoType* type);

		::MonoType* GetMonoType();

	private:
		::MonoType* m_MonoType;
	};

} }

#endif