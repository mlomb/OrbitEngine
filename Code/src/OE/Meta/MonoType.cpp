#include "OE/Meta/MonoType.hpp"

namespace OrbitEngine { namespace Meta {
	MonoType::MonoType(const std::string& name, const Kind kind, ::MonoType* type)
		: Type(name, kind), m_MonoType(type)
	{
		
	}

	::MonoType* MonoType::GetMonoType()
	{
		return m_MonoType;
	}
} }
